#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>
#include <math.h>

#define println(format, ...) printf(format "\n", ##__VA_ARGS__)

/// @brief 3 component vector (3d vector)
typedef struct
{
    float x;
    float y;
    float z;
} JM_Vec3;

/// @brief 2 component vector (2d vector)
typedef struct
{
    float x;
    float y;
} JM_Vec2;

#define JM_VEC2_ZERO ((GM_Vec2){0.0f, 0.0f})
#define JM_VEC3_ZERO ((GM_Vec3){0.0f, 0.0f, 0.0f})

typedef char* (*JMVec2ToStringFn)(const JM_Vec2* vec);
typedef char* (*JMVec3ToStringFn)(const JM_Vec3* vec);

char* jm_vec2_to_string(const JM_Vec2* vec);
char* jm_vec3_to_string(const JM_Vec3* vec);
/// @brief Dot product of 2 2D vectors
/// @param a vector 1
/// @param b vector 2
/// @return scalar
float jm_vec2_dot(const JM_Vec2* a, const JM_Vec2* b);
/// @brief Dot product of 2 3D vectors
/// @param a vector 1
/// @param b vector 2
/// @return scalar
float jm_vec3_dot(const JM_Vec3* a, const JM_Vec3* b);
/// @brief Cross product of 2 2D vectors
/// @param a vector 1
/// @param b vector 2
/// @return scalar
float jm_vec2_cross(const JM_Vec2* a, const JM_Vec2* b);
/// @brief Cross product of 2 3D vectors
/// @param a vector 1
/// @param b vector 2
/// @return a 3d vector
JM_Vec3 jm_vec3_cross(const JM_Vec3* a, const JM_Vec3* b);
/// @brief clamps value between a min and max value
/// @param value value to clamp
/// @param min min bound
/// @param max max bound
/// @return min if value < min, max if value > max, else value
float jm_clampf(const float value, const float min, const float max);
/// @brief Checks whether the point "pt" lies inside of the triangle formed by the last 3 parameters.
/// @param pt point to check
/// @param a triangle point 1
/// @param b triangle point 2
/// @param c triangle point 3
/// @return a boolean value of whether it does or does not
bool jm_triangles_encloses(const JM_Vec2 pt, const JM_Vec2 a, const JM_Vec2 b, const JM_Vec2 c);
/// @brief Calculates the signed area of the formed triangle
/// @param p1 point 1
/// @param p2 point 2
/// @param p3 point 3
/// @return positive - p3 lies to the left of p1 and p2; negative - p3 lies to the right of p1 and p2; 0 - p1, p2, and p3 are collinear
float jm_triangles_sign(const JM_Vec2 p1, const JM_Vec2 p2, const JM_Vec2 p3);
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