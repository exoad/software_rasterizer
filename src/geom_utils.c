#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "geometry.h"
#include "utils.h"

F32 jm_vec2_dot(const JM_Vec2* a, const JM_Vec2* b)
{
    if(a == NULL || b == NULL)
    {
        printerr("%s", "Cannot perform a dot product with a null poI32er.");
        return 0.f;
    }
    return (a->x * b->x) + (a->y * b->y);
}

F32 jm_vec3_dot(const JM_Vec3* a, const JM_Vec3* b)
{
    if(a == NULL || b == NULL)
    {
        printerr("%s", "Cannot perform a dot product with a null poI32er.");
        return 0.f;
    }
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

F32 jm_vec2_cross(const JM_Vec2* a, const JM_Vec2* b)
{
    if(a == NULL || b == NULL)
    {
        printerr("%s", "Cannot perform a cross product with a null poI32er.");
        return 0.f;
    }
    return (a->x * b->y) - (a->y * b->x);
}

JM_Vec3 jm_vec3_cross(const JM_Vec3* a, const JM_Vec3* b)
{
    if(a == NULL || b == NULL)
    {
        printerr("%s", "Cannot perform a cross product with a null poI32er.");
        return JM_VEC3_ZERO;
    }
    return (JM_Vec3)
    {
        (a->y * b->z) - (a->z * b->y),
        (a->z * b->x) - (a->x * b->z),
        (a->x * b->y) - (a->y * b->x)
    };
}

F32 jm_triangles_sign(const JM_Vec2 p1, const JM_Vec2 p2, const JM_Vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

// implementation credits to: https://stackoverflow.com/questions/2049582/how-to-determine-if-a-poI32-is-in-a-2d-triangle
// barycentric coordinates :O
bool jm_triangles_encloses(const JM_Vec2 pt, const JM_Vec2 a, const JM_Vec2 b, const JM_Vec2 c)
{
    const F32 d1 = jm_triangles_sign(pt, a, b);
    const F32 d2 = jm_triangles_sign(pt, b, c);
    const F32 d3 = jm_triangles_sign(pt, c, a);
    return !(((d1 < 0) || (d2 < 0) || (d3 < 0)) && ((d1 > 0) || (d2 > 0) || (d3 > 0)));
}

I8* jm_vec2_tostring(const JM_Vec2* vec)
{
    if (vec == NULL)
        return NULL;
    const I32 buffer_size = 50;
    I8* str = malloc((Sz) buffer_size * sizeof(I8));
    if (str == NULL)
    {
        printerr("%s", "Failed to allocate memory for JM_Vec2 string");
        return NULL;
    }
    if (snprintf(str, (Sz) buffer_size, "(%.2f, %.2f)", vec->x, vec->y) < 0)
    {
        free(str);
        return NULL;
    }
    return str;
}

I8* jm_vec3_tostring(const JM_Vec3* vec)
{
    if (vec == NULL)
        return NULL;
    I8* str = (I8*) malloc(70 * sizeof(I8));
    if (str == NULL)
    {
        printerr("%s", "Failed to allocate memory for JM_Vec3 string");
        return NULL;
    }
    const I32 written = snprintf(str, 70, "(%.2f, %.2f, %.2f)", vec->x, vec->y, vec->z);
    if (written < 0)
    {
        free(str);
        return NULL;
    }
    return str;
}