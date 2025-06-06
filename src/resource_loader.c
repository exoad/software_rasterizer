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

/// @brief connects as much given points using triangle fans
JM_Vec3* _form_triangles(const JM_ObjData* face)
{
    JM_Vec3* triangles = NULL;
    const I32 facesLength = arrlen(face->faces);
    for(I32 i = 0; i < facesLength; i++)
    {
        const I32 groupLength = arrlen(face->faces[i]);
        for(I32 j = 0; j < groupLength; j++)
        {
            if(j >= 3)
            {
                arrput(triangles, triangles[groupLength - (3 * j - 6)]);
                arrput(triangles, triangles[groupLength - 2]);
            }
            arrput(triangles, face->vertices[face->faces[i][j].vIndex - 1]);
        }
    }
    return triangles;
}

JM_Model* jm_res_constructobj(const JM_ObjData* face)
{
    JM_Vec3* triangles = _form_triangles(face);
    JM_Model* model = malloc(sizeof(JM_Model));
    model->pts = triangles;
    model->colors = NULL;
    const I32 len =  arrlen(triangles) / 3;
    arrsetlen(model->colors, len);
    for(I32 i = 0; i < len; i++)
    {
        const JM_Color color = jm_color_random();
        println("Color %d: %d %d %d", i, color.r, color.g, color.b);
        model->colors[i] = color;

    }
    println("Triangles Constructed: %d", len);
    return model;
}
