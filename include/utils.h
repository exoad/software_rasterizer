#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>
#include <math.h>

#include "geometry.h"

#define println(format, ...) printf(format "\n", ##__VA_ARGS__)

#define JM_VEC2_ZERO ((GM_Vec2){0.0f, 0.0f})
#define JM_VEC3_ZERO ((GM_Vec3){0.0f, 0.0f, 0.0f})


/// @brief min of three float values
static inline float jm_min3f(const float a, const float b, const float c)
{
    return fminf(a, fminf(b, c));
}
/// @brief max of three float values
static inline float jm_max3f(const float a, const float b, const float c)
{
    return fmaxf(a, fmaxf(b, c));
}

#endif