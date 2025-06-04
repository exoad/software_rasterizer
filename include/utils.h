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

#endif