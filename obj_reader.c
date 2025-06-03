#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "def.h"

#define TARGET_FILE "cube.obj"
#define BUFFER_SIZE 512

// -----------------------------------------------------

typedef struct Vec3 {
    F32 x;
    F32 y;
    F32 z;
} Vec3;

#define VEC3_ZERO (Vec3) { 0.f, 0.f, 0.f }

I8* vec3_to_string(const Vec3* vec)
{
    if(vec == NULL)
    {
        return NULL;
    }
    I8* buffer = malloc((Sz) 70 * sizeof(char));
    if(buffer == NULL)
    {
        printerr("%s", "Failed to alloc memory :(");
        return NULL;
    }
    if(snprintf(buffer, (Sz) 70, "(%f, %f, %f)", vec->x, vec->y, vec->z) < 0)
    {
        free(buffer);
        return NULL;
    }
    return buffer;
}

// -----------------------------------------------------

typedef struct Vec2 {
    F32 x;
    F32 y;
} Vec2;

#define VEC2_ZERO (Vec2) { 0.f, 0.f }

I8* vec2_to_string(const Vec2* vec)
{
    if(vec == NULL)
    {
        return NULL;
    }
    I8* buffer = malloc((Sz) 50 * sizeof(char));
    if(buffer == NULL)
    {
        printerr("%s", "Failed to alloc memory :(");
        return NULL;
    }
    if(snprintf(buffer, (Sz) 50, "(%f, %f)", vec->x, vec->y) < 0)
    {
        free(buffer);
        return NULL;
    }
    return buffer;
}

// -----------------------------------------------------

typedef struct FaceCorner
{
    I64 vIndex;
    I64 vtIndex;
    I64 vnIndex;
} FaceCorner;

I8* facecorner_to_string(const FaceCorner* face)
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
    if(snprintf(buffer, (Sz) 40, "%lld/%lld/%lld", face->vIndex, face->vtIndex, face->vnIndex))
    {
        free(buffer);
        return NULL;
    }
    return buffer;
}

// -----------------------------------------------------

static void skip_whitespace(I8** ptr)
{
    while(isspace((U8) **ptr) && **ptr != '\0')
    {
       (*ptr)++;
    }
}

static I64 parse_single_face_value(I8** ptr)
{
    I8* end;
    errno = 0;
    const I64 res = strtol(*ptr, &end, 10);
    if(*ptr == end)
    {
        printerr("Found no integer. Got '%s'", *ptr);
        return 0;
    }
    if(errno == ERANGE || res > INT_MAX || res < INT_MIN)
    {
        printerr("Received invalid integer. Got: '%s'", *ptr);
        return 0;
    }
    *ptr = end;
    return   res;
}

// -----------------------------------------------------

I32 main(U0)
{
    println("%s", "Hello World!");
    FILE* file = fopen(TARGET_FILE, "r");
    if (!file)
    {
        printerr("%s", "Failed to open file");
        return EXIT_FAILURE;
    }
    I8 buffer[BUFFER_SIZE];
    I32 i = 1;
    // +--- helpful resource for describing the layout of the wavefront obj file ---+
    //
    // 1) https://people.computing.clemson.edu/~dhouse/courses/405/docs/brief-obj-file-format.html
    while(fgets(buffer, sizeof(buffer), file))
    {
        I8* line = buffer;
        skip_whitespace(&line);
        if(*line != '#' && *line != '\n' && *line != '\0') // skip comments and empty lines
        {
            I8 commandBuffer[16]; // might not need to fit in 16 letters, for now support vertices and all of that no material and lighting
            if(sscanf(line, "%s", commandBuffer) == 1)
            {
                line += strlen(commandBuffer); // move past the size of the command
                skip_whitespace(&line);
                // geometric vertices in the format of "v <x> <y> <z>"
                if(strcmp(commandBuffer, "v") == 0)
                {
                    Vec3 position;
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
                    println("%d: v %s", i, vec3_to_string(&position));
                }
                // texture coordinates in the format of "vt <u> <v>"
                else if(strcmp(commandBuffer, "vn") == 0)
                {
                    Vec3 normal;
                    I8* ptr;
                    normal.x = strtof(line, &ptr);
                    skip_whitespace(&line);
                    normal.y = strtof(ptr, &ptr);
                    skip_whitespace(&line);
                    normal.z = strtof(ptr, NULL);
                    println("%d: vn %s", i, vec3_to_string(&normal));
                }
                // texture normals in the format of "vn <dx> <dy> <dz>"
                else if(strcmp(commandBuffer, "vt") == 0)
                {
                    Vec2 uv;
                    I8* ptr;
                    uv.x = strtof(line, &ptr);
                    skip_whitespace(&line);
                    uv.y = strtof(ptr, NULL);
                	println("%d: vt %s", i, vec2_to_string(&uv));
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
					I32 facesCounter = 0;
                   	while(tokens != NULL) // first split the string by spaces to denote the individual faces
                   	{
						FaceCorner corner = { 0 };
						// if counter reaches 2 it means either uv coords OR normals
						// if counter reaches 3 it means uv coords and normals
						// if counter reaches 1 it means only vertex
						I32 counter = 0;
						// take each token from the previous delimiting move and
						// use forward slashes ("/") as the next delimiter to split the string further.
						I8* pos = tokens;
						I8* slashDelimPtr;
						I64 value = 0;
						// this part loops through on single face corner
						// AKA the part delimited by a space (see the definition of FaceCorner)
						while(*pos != '\0') // strtok and strok_r insert the null terminator for the parts where it
											// the string up and is an easy way to check :)
						{
							counter++; // count how many numbers are parsed
							slashDelimPtr = strstr(pos, "/");
							if(slashDelimPtr == NULL)
							{
								value = parse_single_face_value(&pos);
								break;
							}
							Sz tokenLength = slashDelimPtr - pos;
							if(tokenLength == 0)
							{
								// empty token, found nothing
								// for example with the accepted format of only vertex normals
								//
								// "4//5"
								//   ^
								// empty texture uv
								printf("%s", " ,");
							}
							else
							{
								I8* nextToken = malloc(tokenLength + 1);
								strncpy(nextToken, pos, tokenLength);
								nextToken[tokenLength] = '\0';
								value = parse_single_face_value(&nextToken);
							}
							pos = slashDelimPtr + 1; // move the delimiter pointer for the next set
							if(counter == 1)
							{
								corner.vIndex = value;
							}
							else if(counter == 2)
							{
								corner.vnIndex = value;
							}
							else if(counter == 3)
							{
								corner.vtIndex = value;
							}
						}
						printf("%s ", facecorner_to_string(&corner));
						tokens = strtok_r(NULL, " ", &ptr);
						facesCounter++;
						counter = 0;
                   	}
					println("\n%s (%d)", "-------------------------", facesCounter);
                }
                else
                {
                    // ignore any other commands like mtllib, g, o, s, l, usemtl for now
                    println("Ignoring unsupported command '%s' at line %d", commandBuffer, i);
                }
            }
        }
        i++;
    }
    fclose(file);
    return EXIT_SUCCESS;
}