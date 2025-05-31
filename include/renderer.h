#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "utils.h"
#include "painting.h"

typedef struct
{
    JM_Color* data;
    int width;
    int height;
} JM_RasterBuffer;

/// @brief Acquires a raster buffer to use for writing graphical information to. Returns NULL if the function fails
/// @param width width of the raster
/// @param height height of the raster
/// @return A ptr to a raster.
JM_RasterBuffer* jm_create_raster_buffer(const int width, const int height);
/// @brief Frees the memmory allocated to this raster buffer. Call this when operations are done!
/// @param buffer ptr to destroy
void jm_destroy_raster_buffer(JM_RasterBuffer* buffer);
/// @brief Gets the pixel value at a specific (x, y) coordinate in the raster buffer
/// @param buffer pointer to the JM_RasterBuffer
/// @param x x-coordinate (column)
/// @param y y-coordinate (row)
/// @return value of the pixel at (x, y). Returns 0.0f if buffer is invalid or coordinates are out of bounds
JM_Color jm_get_raster_pixel(const JM_RasterBuffer* buffer, const int x, const int y);
/// @brief Sets the pixel value at a specific (x, y) coordinate in the raster buffer
/// @param buffer pointer to the JM_RasterBuffer
/// @param x x-coordinate (column)
/// @param y y-coordinate (row)
/// @param value the value to set the pixel to
void jm_set_raster_pixel(JM_RasterBuffer* buffer, const int x, const int y, const JM_Color value);

#endif