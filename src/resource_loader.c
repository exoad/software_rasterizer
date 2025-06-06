#include "resource_loader.h"
#include "external/stb_ds.h"
#include "painting.h"
#include "utils.h"

U0 jm_res_freemodel(JM_Model* model)
{
    if(model != NULL)
    {
        arrfree(model->pts);
        arrfree(model->colors);
        free(model);
    }
}

/// @brief connects as much given points using triangle fans (https://en.wikipedia.org/wiki/Fan_triangulation)
JM_Vec3* _stitch_triangles(const JM_ObjData* face)
{
    JM_Vec3* triangles = NULL;
    const I32 facesLength = arrlen(face->faces);
    for(I32 i = 0; i < facesLength; i++)
    {
        const I32 groupLength = arrlen(face->faces[i]);
        if (groupLength < 3)
            continue;
        const JM_ObjFaceCorner pivot = face->faces[i][0];
        for (I32 j = 1; j < groupLength - 1; j++)
        {
            arrput(triangles, face->vertices[pivot.vIndex - 1]);
            arrput(triangles, face->vertices[face->faces[i][j].vIndex - 1]);
            arrput(triangles, face->vertices[face->faces[i][j + 1].vIndex - 1]);
        }
    }
    return triangles;
}

JM_Model* jm_res_constructobj(const JM_ObjData* face)
{
    JM_Vec3* triangles = _stitch_triangles(face);
    JM_Model* model = malloc(sizeof(JM_Model));
    model->pts = triangles;
    model->colors = NULL;
    const I32 len =  arrlen(triangles) / 3;
    arrsetlen(model->colors, len);
    for(I32 i = 0; i < len; i++)
    {
        model->colors[i] = jm_color_random();

    }
    println("Triangles Constructed: %d", len);
    return model;
}
