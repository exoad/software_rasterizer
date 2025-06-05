#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "def.h"

/// @brief 3 component vector (3d vector)
typedef struct JM_Vec3
{
    F32 x;
    F32 y;
    F32 z;
} JM_Vec3;
/// @brief 2 component vector (2d vector)
typedef struct JM_Vec2
{
    F32 x;
    F32 y;
} JM_Vec2;
/// @brief Dot product of 2 2D vectors
/// @param a vector 1
/// @param b vector 2
/// @return scalar
F32 jm_vec2_dot(const JM_Vec2* a, const JM_Vec2* b);
/// @brief Dot product of 2 3D vectors
/// @param a vector 1
/// @param b vector 2
/// @return scalar
F32 jm_vec3_dot(const JM_Vec3* a, const JM_Vec3* b);
/// @brief Cross product of 2 2D vectors
/// @param a vector 1
/// @param b vector 2
/// @return scalar
F32 jm_vec2_cross(const JM_Vec2* a, const JM_Vec2* b);
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
F32 jm_clampf(F32 value, F32 min, F32 max);
/// @brief Checks whether the point "pt" lies inside of the triangle formed by the last 3 parameters.
/// @param pt point to check
/// @param a triangle point 1
/// @param b triangle point 2
/// @param c triangle point 3
/// @return a boolean value of whether it does or does not
Bool jm_triangles_encloses(JM_Vec2 pt, JM_Vec2 a, JM_Vec2 b, JM_Vec2 c);
/// @brief Calculates the signed area of the formed triangle
/// @param p1 point 1
/// @param p2 point 2
/// @param p3 point 3
/// @return positive - p3 lies to the left of p1 and p2; negative - p3 lies to the right of p1 and p2; 0 - p1, p2, and p3 are collinear
F32 jm_triangles_sign(JM_Vec2 p1, JM_Vec2 p2, JM_Vec2 p3);

I8* jm_vec2_tostring(const JM_Vec2* vec);
I8* jm_vec3_tostring(const JM_Vec3* vec);


#endif