#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "allocator.h"
#include "def.h"

/// @brief 3 component vector (3d vector)
typedef struct JM_Vec3
{
    float x;
    float y;
    float z;
} JM_Vec3;
/// @brief 2 component vector (2d vector)
typedef struct JM_Vec2
{
    float x;
    float y;
} JM_Vec2;
/// @brief A vertex in 3D space
typedef struct JM_Vertex
{
    JM_Vec3 position;
    JM_Vec3 normal;
    JM_Vec2 textureCoordinates;
} JM_Vertex;
/// @brief A collection of vertices to form a mesh
typedef struct JM_Mesh {
    char name[64];
    JM_Vertex* vertices;
    uint32 verticesCount;
    uint32* indices;
    uint32 indicesCount;
} JM_Mesh;
/// @brief A collection of meshes
typedef struct JM_Scene {
    char name[256];
    JM_Mesh** meshes;
    uint32 meshesCount;
    JM_Arena* arena;
} JM_Scene;

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
float jm_clampf(float value, float min, float max);
/// @brief Checks whether the point "pt" lies inside of the triangle formed by the last 3 parameters.
/// @param pt point to check
/// @param a triangle point 1
/// @param b triangle point 2
/// @param c triangle point 3
/// @return a boolean value of whether it does or does not
bool jm_triangles_encloses(JM_Vec2 pt, JM_Vec2 a, JM_Vec2 b, JM_Vec2 c);
/// @brief Calculates the signed area of the formed triangle
/// @param p1 point 1
/// @param p2 point 2
/// @param p3 point 3
/// @return positive - p3 lies to the left of p1 and p2; negative - p3 lies to the right of p1 and p2; 0 - p1, p2, and p3 are collinear
float jm_triangles_sign(JM_Vec2 p1, JM_Vec2 p2, JM_Vec2 p3);

char* jm_vec2_to_string(const JM_Vec2* vec);
char* jm_vec3_to_string(const JM_Vec3* vec);


#endif