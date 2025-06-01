#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "geometry.h"

float jm_vec2_dot(const JM_Vec2* a, const JM_Vec2* b)
{
    assert(a != NULL && b != NULL);
    return (a->x * b->x) + (a->y * b->y);
}

float jm_vec3_dot(const JM_Vec3* a, const JM_Vec3* b)
{
    assert(a != NULL && b != NULL);
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

float jm_vec2_cross(const JM_Vec2* a, const JM_Vec2* b)
{
    assert(a != NULL && b != NULL);
    return (a->x * b->y) - (a->y * b->x);
}

JM_Vec3 jm_vec3_cross(const JM_Vec3* a, const JM_Vec3* b)
{
    assert(a != NULL && b != NULL);
    return (JM_Vec3)
    {
        (a->y * b->z) - (a->z * b->y),
        (a->z * b->x) - (a->x * b->z),
        (a->x * b->y) - (a->y * b->x)
    };
}

float jm_triangles_sign(const JM_Vec2 p1, const JM_Vec2 p2, const JM_Vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

// implementation credits to: https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
// barycentric coordinates :O
bool jm_triangles_encloses(const JM_Vec2 pt, const JM_Vec2 a, const JM_Vec2 b, const JM_Vec2 c)
{
    const float d1 = jm_triangles_sign(pt, a, b);
    const float d2 = jm_triangles_sign(pt, b, c);
    const float d3 = jm_triangles_sign(pt, c, a);
    return !(((d1 < 0) || (d2 < 0) || (d3 < 0)) && ((d1 > 0) || (d2 > 0) || (d3 > 0)));
}

char* jm_vec2_to_string(const JM_Vec2* vec)
{
    if (vec == NULL)
        return NULL;
    const int buffer_size = 50;
    char* str = (char*) malloc((usize) buffer_size * sizeof(char));
    if (str == NULL)
    {
        printerr("%s", "Failed to allocate memory for JM_Vec2 string");
        return NULL;
    }
    if (snprintf(str, (usize) buffer_size, "(%.2f, %.2f)", vec->x, vec->y) < 0)
    {
        free(str);
        return NULL;
    }
    return str;
}

char* jm_vec3_to_string(const JM_Vec3* vec)
{
    if (vec == NULL)
        return NULL;
    char* str = (char*)malloc(70 * sizeof(char));
    if (str == NULL)
    {
        printerr("%s", "Failed to allocate memory for JM_Vec3 string");
        return NULL;
    }
    const int chars_written = snprintf(str, 70, "(%.2f, %.2f, %.2f)", vec->x, vec->y, vec->z);
    if (chars_written < 0)
    {
        free(str);
        return NULL;
    }
    return str;
}