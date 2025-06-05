#ifndef __UTILS_H__
#define __UTILS_H__

#include <math.h>

#define println(format, ...) printf(format "\n", ##__VA_ARGS__)

#define JM_VEC2_ZERO ((JM_Vec2){0.0f, 0.0f})
#define JM_VEC3_ZERO ((JM_Vec3){0.0f, 0.0f, 0.0f})

/// @brief min of three F32 values
static F32 jm_min3f(const F32 a, const F32 b, const F32 c)
{
    return fminf(a, fminf(b, c));
}
/// @brief max of three F32 values
static F32 jm_max3f(const F32 a, const F32 b, const F32 c)
{
    return fmaxf(a, fmaxf(b, c));
}

JM_Vec3 jm_color_to_vec3(const JM_Color color);

JM_Color jm_vec3_to_color(const JM_Vec3 vec3);

static F32 jm_randomf32(const F32 min, const F32 max)
{
    return min + (F32) rand() / RAND_MAX * (max - min); // NOLI32(cert-msc30-c, cert-msc50-cpp)
}

#endif