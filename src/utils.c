#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

char* jm_vec2_to_string(const JM_Vec2* vec)
{
    if (vec == NULL)
        return NULL;
    int buffer_size = 50;
    char* str = (char*) malloc((size_t) buffer_size * sizeof(char));
    if (str == NULL)
    {
        perror("Failed to allocate memory for JM_Vec2 string");
        return NULL;
    }
    if (snprintf(str, (size_t) buffer_size, "(%.2f, %.2f)", vec->x, vec->y) < 0)
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
        perror("Failed to allocate memory for JM_Vec3 string");
        return NULL;
    }
    int chars_written = snprintf(str, 70, "(%.2f, %.2f, %.2f)", vec->x, vec->y, vec->z);
    if (chars_written < 0)
    {
        free(str);
        return NULL;
    }
    return str;
}