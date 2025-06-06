#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "geometry.h"
#include "utils.h"

F32 jm_deg_to_rad(F32 degrees)
{
    return degrees * (PI / 180.0f);
}

F32 jm_vec2_dot(JM_Vec2 a, JM_Vec2 b)
{
    return (a.x * b.x) + (a.y * b.y);
}

F32 jm_vec3_dot(JM_Vec3 a, JM_Vec3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

F32 jm_vec2_cross(JM_Vec2 a, JM_Vec2 b)
{
    return (a.x * b.y) - (a.y * b.x);
}

JM_Vec3 jm_vec3_cross(JM_Vec3 a, JM_Vec3 b)
{
    return (JM_Vec3)
    {
        (a.y * b.z) - (a.z * b.y),
        (a.z * b.x) - (a.x * b.z),
        (a.x * b.y) - (a.y * b.x)
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
    {
        return NULL;
    }
    const I32 buffer_size = 50;
    I8* str = malloc((Sz) buffer_size * sizeof(I8));
    if (str == NULL)
    {
        panic("%s", "Failed to allocate memory for JM_Vec2 string");
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
    {
        return NULL;
    }
    I8* str = (I8*) malloc(70 * sizeof(I8));
    if (str == NULL)
    {
        panic("%s", "Failed to allocate memory for JM_Vec3 string");
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

JM_Mat4 jm_mat4_identity()
{
    JM_Mat4 mat = { 0 };
    mat.m[0] = 1.0f;
    mat.m[5] = 1.0f;
    mat.m[10] = 1.0f;
    mat.m[15] = 1.0f;
    return mat;
}

JM_Mat4 jm_mat4_mul(JM_Mat4 a, JM_Mat4 b)
{
    JM_Mat4 result = { 0 };
    for(I32 col = 0; col < 4; col++)
    {
        for(I32 row = 0; row < 4; row++)
        {
            result.m[col * 4 + row] = 0.f;
            for(I32 k = 0; k < 4; k++)
            {
                result.m[col * 4 + row] += a.m[k * 4 + row] * b.m[col * 4 + k];
            }
        }
    }
    return result;
}

JM_Vec4 jm_mat4_mul_vec4(JM_Mat4 m, JM_Vec4 v)
{
    JM_Vec4 res;
    res.x = m.m[0] * v.x + m.m[4] * v.y + m.m[8] * v.z + m.m[12] * v.w;
    res.y = m.m[1] * v.x + m.m[5] * v.y + m.m[9] * v.z + m.m[13] * v.w;
    res.z = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14] * v.w;
    res.w = m.m[3] * v.x + m.m[7] * v.y + m.m[11] * v.z + m.m[15] * v.w;
    return res;
}

JM_Mat4 jm_mat4_translate(JM_Vec3 t)
{
    JM_Mat4 mat = jm_mat4_identity();
    mat.m[12] = t.x;
    mat.m[13] = t.y;
    mat.m[14] = t.z;
    return mat;
}

JM_Mat4 jm_mat4_rotate_x(F32 angleRadians)
{
    JM_Mat4 mat = jm_mat4_identity();
    F32 c = cosf(angleRadians);
    F32 s = sinf(angleRadians);
    mat.m[5] = c;
    mat.m[6] = s;
    mat.m[9] = -s;
    mat.m[10] = c;
    return mat;
}

JM_Mat4 jm_mat4_rotate_y(F32 angleRadians)
{
    JM_Mat4 mat = jm_mat4_identity();
    F32 c = cosf(angleRadians);
    F32 s = sinf(angleRadians);
    mat.m[0] = c;
    mat.m[2] = -s;
    mat.m[8] = s;
    mat.m[10] = c;
    return mat;
}

JM_Mat4 jm_mat4_rotate_z(F32 angleRadians)
{
    JM_Mat4 mat = jm_mat4_identity();
    F32 c = cosf(angleRadians);
    F32 s = sinf(angleRadians);
    mat.m[0] = c;
    mat.m[1] = s;
    mat.m[4] = -s;
    mat.m[5] = c;
    return mat;
}

JM_Mat4 jm_mat4_scale(JM_Vec3 s)
{
    JM_Mat4 mat = jm_mat4_identity();
    mat.m[0] = s.x;
    mat.m[5] = s.y;
    mat.m[10] = s.z;
    return mat;
}

JM_Mat4 jm_mat4_perspective(F32 fovRad, F32 aspect, F32 nearPlane, F32 farPlane)
{
    JM_Mat4 mat = { 0 };
    F32 f = 1.f / tanf(fovRad / 2.f);
    mat.m[0] = f / aspect;                                             // c0r0
    mat.m[5] = f;                                                      // c1r1
    mat.m[10] = (farPlane + nearPlane) / (nearPlane - farPlane);       // c2r2
    mat.m[11] = -1.f;                                                  // c2r3
    mat.m[14] = (2.f * farPlane * nearPlane) / (nearPlane - farPlane); // c3r2
    return mat;
}

JM_Vec3 jm_vec3_normalize(JM_Vec3 v)
{
    F32 length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length > 0)
    {
        return (JM_Vec3) { v.x / length, v.y / length, v.z / length };
    }
    return (JM_Vec3) { 0, 0, 0 };
}

JM_Vec3 jm_vec3_sub(JM_Vec3 a, JM_Vec3 b)
{
    return (JM_Vec3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

JM_Mat4 jm_mat4_lookat(JM_Vec3 eye, JM_Vec3 target, JM_Vec3 up)
{
    const JM_Vec3 normalizedUp = jm_vec3_normalize(up);
    JM_Vec3 zAxis = jm_vec3_normalize(jm_vec3_sub(eye, target));
    JM_Vec3 xAxis = jm_vec3_normalize(jm_vec3_cross(normalizedUp, zAxis));
    JM_Vec3 yAxis = jm_vec3_cross(zAxis, xAxis);
    JM_Mat4 mat = { 0 };
    mat.m[0] = xAxis.x;
    mat.m[1] = yAxis.x;
    mat.m[2] = zAxis.x;
    mat.m[3] = 0.f;
    mat.m[4] = xAxis.y;
    mat.m[5] = yAxis.y;
    mat.m[6] = zAxis.y;
    mat.m[7] = 0.f;
    mat.m[8] = xAxis.z;
    mat.m[9] = yAxis.z;
    mat.m[10] = zAxis.z;
    mat.m[11] = 0.f;
    mat.m[12] = -jm_vec3_dot(xAxis, eye);
    mat.m[13] = -jm_vec3_dot(yAxis, eye);
    mat.m[14] = -jm_vec3_dot(zAxis, eye);
    mat.m[15] = 1.0f;
    return mat;
}