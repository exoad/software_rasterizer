#include "resource_loaders/obj_reader.h";
#include "external/stb_ds.h"
#include "def.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

U0 _skip_whitespace(I8** ptr)
{
    while(isspace((U8) **ptr) && **ptr != '\0')
    {
       (*ptr)++;
    }
}

U0 jm_obj_freedata(JM_ObjData* data)
{
    if (data == NULL)
    {
        return;
    }
    for (Sz i = 0; i < arrlen(data->faces); ++i)
    {
        arrfree(data->faces[i]);
    }
    arrfree(data->faces);
    arrfree(data->vertices);
    arrfree(data->normals);
    arrfree(data->uvs);
    free(data);
}

I8* jm_obj_facecorner_tostring(const JM_ObjFaceCorner* face)
{
    if(face == NULL)
    {
        return NULL;
    }
    I8* buffer = malloc((Sz) 40 * sizeof(I8));
    if(buffer == NULL)
    {
        printerr("%s", "Faild to alloc memory :(");
        return NULL;
    }
    if(snprintf(buffer, (Sz) 40, "%lld/%lld/%lld", face->vIndex, face->vtIndex, face->vnIndex) < 0)
    {
        free(buffer);
        return NULL;
    }
    return buffer;
}

JM_ObjData* jm_obj_parsefile(const I8* pathToFile)
{
    FILE* file = fopen(pathToFile, "r");
    if(!file)
    {
        printerr("%s", "Failed to open file");
        return NULL;
    }
    I8 buffer[512];
    I64 lineNumber = 1;
    JM_ObjData* data = malloc(sizeof(JM_ObjData));
    if(data == NULL)
    {
        printerr("Failed to allocate memory to parse Wavefront OBJ File: %s", pathToFile);
        fclose(file);
        return NULL;
    }
    data->faces = NULL;    // f
    data->normals = NULL;  // vn
    data->uvs = NULL;      // vt
    data->vertices = NULL; // v
    while(fgets(buffer, sizeof(buffer), file))
    {
        I8* line = buffer;
        skip_whitespace(&line);
        if(*line != '#' && *line != '\n' && *line != '\0') // skip comments and empty lines
        {
            I8 commandBuffer[16]; // might not need to fit in 16 letters, for now support vertices and all of that no material and lighting
            if(sscanf(line, "%15s", commandBuffer) == 1)
            {
                line += strlen(commandBuffer); // move past the size of the command
                skip_whitespace(&line);
                // geometric vertices in the format of "v <x> <y> <z>"
                if(strcmp(commandBuffer, "v") == 0)
                {
                    JM_Vec3 position;
                    // +--- much better to use strotf to enable conversion error reporting ---+
                    // +--- the following commented out implementation is not the best way ---+
                    //
                    // if(sscanf(line, "v %f %f %f", &position.x, &position.y, &position.z) == 3)
                    // {
                    //     println("%d: Position at { %f, %f, %f }", i, position.x, position.y, position.z);
                    // }
                    I8* ptr;
                    position.x = strtof(line, &ptr);
                    skip_whitespace(&line);
                    position.y = strtof(ptr, &ptr);
                    skip_whitespace(&line);
                    position.z = strtof(ptr, NULL);
                    arrput(data->vertices, position);
                }
                // texture coordinates in the format of "vt <u> <v>"
                else if(strcmp(commandBuffer, "vn") == 0)
                {
                    JM_Vec3 normal;
                    I8* ptr;
                    normal.x = strtof(line, &ptr);
                    skip_whitespace(&line);
                    normal.y = strtof(ptr, &ptr);
                    skip_whitespace(&line);
                    normal.z = strtof(ptr, NULL);
                    arrput(data->normals, normal);
                }
                // texture normals in the format of "vn <dx> <dy> <dz>"
                else if(strcmp(commandBuffer, "vt") == 0)
                {
                    JM_Vec2 uv;
                    I8* ptr;
                    uv.x = strtof(line, &ptr);
                    skip_whitespace(&line);
                    uv.y = strtof(ptr, NULL);
                    arrput(data->uvs, uv);
                }
                // faces with different formats we need to support
                //
                // default           1. f <v_1> <v_2> <v_3> ... <v_n>
                // texture coords    2. f <v_1>/<t_1> <v_2>/<t_2> ... <v_n>/<t_n>
                // vertex normals    3. f <v_1>//<n_1> <v_2>//<n_2> ... <v_n>//<n_n>
                // texture + normals 4. f <v_1>/<t_1>/<n_1> <v_2>/<t_2>/<n_2> ... <v_n>/<t_n>/<n_n>
                else if(strcmp(commandBuffer, "f") == 0)
                {
                    I8* ptr;
                    I8* tokens = strtok_r(line, " ", &ptr);
                    JM_ObjFaceCorner* corners = NULL;
                    while(tokens != NULL) // first split the string by spaces to denote the individual faces
                    {
                        JM_ObjFaceCorner corner = { 0 };
                        I32 result_v = sscanf(tokens, "%lld", &corner.vIndex);
                        I32 result_vt = sscanf(tokens, "%lld/%lld", &corner.vIndex, &corner.vtIndex);
                        I32 result_vn = sscanf(tokens, "%lld//%lld", &corner.vIndex, &corner.vnIndex);
                        I32 result_vtn = sscanf(tokens, "%lld/%lld/%lld", &corner.vIndex, &corner.vtIndex, &corner.vnIndex);
                        if (!(result_vtn == 3 || result_vn == 2 || result_vt == 2 || result_v == 1))
                        {
                            printerr("Failed to part face corner token at %d. Got %s", lineNumber, tokens);
                            return EXIT_FAILURE;
                        }
                        //
                        // +--- original approach using strtol without using sscanf ---+
                        //
                        // // if counter reaches 2 it means either uv coords OR normals
                        // // if counter reaches 3 it means uv coords and normals
                        // // if counter reaches 1 it means only vertex
                        // I32 counter = 0;
                        // // take each token from the previous delimiting move and
                        // // use forward slashes ("/") as the next delimiter to split the string further.
                        // I8* pos = tokens;
                        // I8* slashDelimPtr;
                        // I64 value = 0;
                        // // this part loops through on single face corner
                        // // AKA the part delimited by a space (see the definition of FaceCorner)
                        // while(*pos != '\0') // strtok and strok_r insert the null terminator for the parts where it
                        // 					// the string up and is an easy way to check :)
                        // {
                        // 	counter++; // count how many numbers are parsed
                        // 	slashDelimPtr = strstr(pos, "/");
                        //     bool requiresBreak = false;
                        // 	if(slashDelimPtr == NULL)
                        // 	{
                        // 		value = parse_single_face_value(&pos);
                        //         requiresBreak = true;
                        // 	}
                        //     else
                        //     {
                        //         Sz tokenLength = slashDelimPtr - pos;
                        //         if(tokenLength == 0)
                        //         {
                        //             // empty token, found nothing
                        //             // for example with the accepted format of only vertex normals
                        //             //
                        //             // "4//5"
                        //             //   ^
                        //             // empty texture uv
                        //             printf("%s", " ,");
                        //         }
                        //         else
                        //         {
                        //             I8* nextToken = malloc(tokenLength + 1);
                        //             strncpy(nextToken, pos, tokenLength);
                        //             nextToken[tokenLength] = '\0';
                        //             value = parse_single_face_value(&nextToken);
                        //         }
                        // 	    pos = slashDelimPtr + 1; // move the delimiter pointer for the next set
                        //     }
                        // 	if(counter == 1)
                        // 	{
                        // 		corner.vIndex = value;
                        // 	}
                        // 	else if(counter == 2)
                        // 	{
                        // 		corner.vtIndex = value;
                        // 	}
                        // 	else if(counter == 3)
                        // 	{
                        // 		corner.vnIndex = value;
                        // 	}
                        //     if(requiresBreak)
                        //     {
                        //         break;
                        //     }
                        //     value = 0;
                        // }
                        tokens = strtok_r(NULL, " ", &ptr);
                        arrput(corners, corner);
                    }
                    arrput(data->faces, corners);
                    printf("\n");
                }
                else
                {
                    // ignore any other commands like mtllib, g, o, s, l, usemtl for now
                    println("Ignoring unsupported command '%s' at line %d", commandBuffer, lineNumber);
                }
            }
            else if(ferror(file))
            {
                printerr("Program panicked while reading obj file at line %lld", lineNumber);
                jm_obj_freedata(data);
                fclose(file);
                return NULL;
            }
        }
        lineNumber++;
    }
    fclose(file);
    return data;
}