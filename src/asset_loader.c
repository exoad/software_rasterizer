#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "resource_loaders/resource_loader.h"
#include "managed_array.h"

// ** .OBJ FILE LOADING **
// A lot of the file format described from this great article: https://paulbourke.net/dataformats/obj/

typedef struct _ObjFaceCorner
{
    int vIndex;
    int vtIndex;
    int vnIndex;
} _ObjFaceCorner;

JM_Scene* jm_resources_load_scene(const char* filePath, const JM_ResourceFormat format, JM_Arena* arena)
{

    ASSERT_NOT_NULL(filePath);
    ASSERT_NOT_NULL(arena);
    if(format != JM_RES_FORMAT_OBJ)
    {
        fprintf(stderr, "jm_resources_load_scene: Only OBJ format is supported in this implementation.\n");
        return NULL;
    }
    FILE* file = fopen(filePath, "r");
    if(file == NULL)
    {
        fprintf(stderr, "jm_resources_load_scene: Failed to open the file %s\n", filePath);
        return NULL;
    }
    JM_Scene* scene = (JM_Scene*) jm_arena_alloc(arena, sizeof(JM_Scene));
    if(scene == NULL)
    {
        fprintf(stderr, "jm_resources_load_scene: Failed to allocate the scene with the supplied arena.\n");
        fclose(file);
        return NULL;
    }
    memset(scene, 0, sizeof(JM_Scene));
    scene->arena = arena;
    strncpy(scene->name, filePath, sizeof(scene->name) - 1);
    scene->name[sizeof(scene->name) - 1] = '\0';
    JM_ManagedArray tempPositions;
    JM_ManagedArray tempTextureCoordinates;
    JM_ManagedArray tempNormals;
    JM_ManagedArray tempFaces;
    jm_managed_array_create(&tempPositions, sizeof(JM_Vec3), arena);
    jm_managed_array_create(&tempTextureCoordinates, sizeof(JM_Vec2), arena);
    jm_managed_array_create(&tempNormals, sizeof(JM_Vec3), arena);
    jm_managed_array_create(&tempFaces, sizeof(JM_ManagedArray), arena);
    JM_Vec3 dummy_vec3 = { 0 };
    JM_Vec2 dummy_vec2 = { 0 };
    jm_managed_array_push(&tempPositions, &dummy_vec3);
    jm_managed_array_push(&tempTextureCoordinates, &dummy_vec2);
    jm_managed_array_push(&tempNormals, &dummy_vec3);
    char line[1024];
    while(fgets(line, sizeof(line), file) != NULL)
    {
        char* curr = line;
        while(isspace((unsigned char) *curr))
        {
            curr++;
        }
        if (*curr == '#' || *curr == '\n' || *curr == '\0')
        {
            continue;
        }
        char command[32];
        int scannedCount = sscanf(curr, "%s", command);
        if(scannedCount != 1)
        {
            continue;
        }
        if(strcmp(command, "v") == 0)
        {
            JM_Vec3 pos;
            if(sscanf(curr, "v %f %f %f", &pos.x, &pos.y, &pos.z) == 3)
            {
                if(!jm_managed_array_push(&tempPositions, &pos))
                {
                    goto error_cleanup;
                }
            }
        }
        else if(strcmp(command, "vt") == 0)
        {
            JM_Vec2 uv;
            if(sscanf(curr, "vt %f %f", &uv.x, &uv.y) >= 2)
            {

                if(!jm_managed_array_push(&tempTextureCoordinates, &uv))
                {
                    goto error_cleanup;
                }
            }
        }
        else if(strcmp(command, "vn") == 0)
        {
            JM_Vec3 norm;
            if(sscanf(curr, "vn %f %f %f", &norm.x, &norm.y, &norm.z) == 3)
            {
                if(!jm_managed_array_push(&tempNormals, &norm))
                {
                    goto error_cleanup;
                }
            }
        }
        else if(strcmp(command, "f") == 0)
        {
            JM_ManagedArray current_face_corners;
            jm_managed_array_create(&current_face_corners, sizeof(_ObjFaceCorner), arena);
            char *tokenPtr = curr + strlen(command);
            char *token;
            char *rest_of_line = tokenPtr;
            while((token = strtok(rest_of_line, " \t\r\n")) != NULL)
            {
                rest_of_line = NULL;
                _ObjFaceCorner fv = { 0, 0, 0 };
                char* first_slash = strchr(token, '/');
                if(first_slash)
                {
                    *first_slash = '\0';
                    fv.vIndex = atoi(token);

                    char* second_slash = strchr(first_slash + 1, '/');
                    if(second_slash)
                    {
                        *second_slash = '\0';
                        if(*(first_slash + 1) != '\0')
                        {
                            fv.vtIndex = atoi(first_slash + 1);
                        }
                        fv.vnIndex = atoi(second_slash + 1);
                    }
                    else
                    {
                        fv.vtIndex = atoi(first_slash + 1);
                    }
                }
                else
                {
                    fv.vIndex = atoi(token);
                }
                if(fv.vIndex > 0)
                {
                    fv.vIndex--;
                }
                else if(fv.vIndex < 0)
                {
                    fv.vIndex = (int) tempPositions.count + fv.vIndex;
                }
                if(fv.vtIndex > 0)
                {
                    fv.vtIndex--;
                }
                else if(fv.vtIndex < 0)
                {
                    fv.vtIndex = (int) tempTextureCoordinates.count + fv.vtIndex;
                }
                if(fv.vnIndex > 0)
                {
                    fv.vnIndex--;
                }
                else if(fv.vnIndex < 0)
                {
                    fv.vnIndex = (int) tempNormals.count + fv.vnIndex;
                }
                if(!jm_managed_array_push(&current_face_corners, &fv))
                {
                    goto error_cleanup;
                }
            }
            if(current_face_corners.count > 0)
            {
                if(!jm_managed_array_push(&tempFaces, &current_face_corners))
                {
                    goto error_cleanup;
                }
            }
            else
            {
                jm_managed_array_free(&current_face_corners);
            }
        }

    }
    fclose(file);
    if(tempFaces.count == 0)
    {
        fprintf(stderr, "Warning: OBJ file '%s' contained no faces. Returning empty scene.\n", filePath);
        goto success_cleanup;
    }
    JM_Mesh* mesh = (JM_Mesh*) jm_arena_alloc(arena, sizeof(JM_Mesh));
    if(!mesh)
    {
        fprintf(stderr, "Error: Failed to allocate mesh for OBJ.\n");
        goto error_cleanup;
    }
    memset(mesh, 0, sizeof(JM_Mesh));
    strncpy(mesh->name, "default_obj_mesh", sizeof(mesh->name) - 1);
    mesh->name[sizeof(mesh->name) - 1] = '\0';
    usize total_final_vertices = 0;
    usize total_final_indices = 0;
    for(usize i = 0; i < tempFaces.count; ++i)
    {
        JM_ManagedArray* face_corners_array = (JM_ManagedArray*) tempFaces.data + i;
        if(face_corners_array->count < 3)
        {
            continue;
        }
        total_final_vertices += (face_corners_array->count - 2) * 3;
        total_final_indices += (face_corners_array->count - 2) * 3;
    }

    mesh->vertices = (JM_Vertex*) jm_arena_alloc(arena, total_final_vertices * sizeof(JM_Vertex));
    mesh->indices = (uint32_t*) jm_arena_alloc(arena, total_final_indices * sizeof(uint32_t));
    if(!mesh->vertices || !mesh->indices)
    {
        fprintf(stderr, "Error: Failed to allocate final vertex/index buffers for mesh.\n");
        goto error_cleanup;
    }
    usize current_vertex_idx = 0;
    usize current_index_idx = 0;
    for(usize i = 0; i < tempFaces.count; ++i)
    {
        JM_ManagedArray* face_corners_array = (JM_ManagedArray*) tempFaces.data + i;
        if(face_corners_array->count < 3)
        {
            jm_managed_array_free(face_corners_array);
            continue;
        }
        for(usize j = 0; j < face_corners_array->count - 2; ++j)
        {
            _ObjFaceCorner triangleCorners[3];
            triangleCorners[0] = ((_ObjFaceCorner*) face_corners_array->data)[0];
            triangleCorners[1] = ((_ObjFaceCorner*) face_corners_array->data)[j + 1];
            triangleCorners[2] = ((_ObjFaceCorner*) face_corners_array->data)[j + 2];
            for(int k = 0; k < 3; ++k)
            {
                _ObjFaceCorner current_corner = triangleCorners[k];
                assert(current_corner.vIndex >= 0 && (usize) current_corner.vIndex < tempPositions.count);
                assert(current_corner.vtIndex >= 0 && (usize) current_corner.vtIndex < tempTextureCoordinates.count);
                assert(current_corner.vnIndex >= 0 && (usize) current_corner.vnIndex < tempNormals.count);
                JM_Vertex new_vertex;
                new_vertex.position = ((JM_Vec3*) tempPositions.data)[current_corner.vIndex];
                new_vertex.textureCoordinates = ((JM_Vec2*) tempTextureCoordinates.data)[current_corner.vtIndex];
                new_vertex.normal = ((JM_Vec3*) tempNormals.data)[current_corner.vnIndex];
                mesh->vertices[current_vertex_idx] = new_vertex;
                mesh->indices[current_index_idx] = (uint32_t) current_vertex_idx;
                current_vertex_idx++;
                current_index_idx++;
            }
        }
        jm_managed_array_free(face_corners_array);
    }
    mesh->verticesCount = (uint32_t) total_final_vertices;
    mesh->indicesCount = (uint32_t) total_final_indices;
    scene->meshesCount = 1;
    scene->meshes = (JM_Mesh**) jm_arena_alloc(arena, sizeof(JM_Mesh*));
    if(!scene->meshes)
    {
        fprintf(stderr, "Error: Failed to allocate mesh array for scene.\n");
        goto error_cleanup;
    }
    scene->meshes[0] = mesh;

success_cleanup:
    jm_managed_array_free(&tempPositions);
    jm_managed_array_free(&tempTextureCoordinates);
    jm_managed_array_free(&tempNormals);
    jm_managed_array_free(&tempFaces);
    fclose(file);
    return scene;

error_cleanup:
    if(file)
    {
        fclose(file);
    }
    return NULL;
}

void jm_resources_destroy_scene(JM_Scene* scene)
{
    if(!scene)
    {
        return;
    }
    if(scene->arena)
    {
        jm_arena_destroy(scene->arena);
    }
    else
    {
        fprintf(stderr, "Warning: jm_resources_destroy_scene called on a scene without an associated arena. Attempting individual free.\n");
        for(uint32_t i = 0; i < scene->meshesCount; ++i)
        {
            if(scene->meshes[i])
            {
                free(scene->meshes[i]->vertices);
                free(scene->meshes[i]->indices);
                free(scene->meshes[i]);
            }
        }
        free(scene->meshes);
        free(scene);
    }
}