#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "painting.h"
#include "resource_loaders/resource_loader.h"

typedef struct JM_RasterBuffer
{
    JM_Color* data;
    I32 width;
    I32 height;
} JM_RasterBuffer;

/// @brief Acquires a raster buffer to use for writing graphical information to. Returns NULL if the function fails
/// @param width width of the raster
/// @param height height of the raster
/// @return A ptr to a raster.
JM_RasterBuffer* jm_create_raster_buffer(I32 width, I32 height);
/// @brief Frees the memory allocated to this raster buffer. Call this when operations are done!
/// @param buffer ptr to destroy
U0 jm_destroy_raster_buffer(JM_RasterBuffer* buffer);
/// @brief Gets the pixel value at a specific (x, y) coordinate in the raster buffer
/// @param buffer pointer to the JM_RasterBuffer
/// @param x x-coordinate (column)
/// @param y y-coordinate (row)
/// @return value of the pixel at (x, y). Returns 0.0f if buffer is invalid or coordinates are out of bounds
JM_Color jm_get_raster_pixel(const JM_RasterBuffer* buffer, I32 x, I32 y);
/// @brief Sets the pixel value at a specific (x, y) coordinate in the raster buffer
/// @param buffer pointer to the JM_RasterBuffer
/// @param x x-coordinate (column)
/// @param y y-coordinate (row)
/// @param value the value to set the pixel to
U0 jm_set_raster_pixel(const JM_RasterBuffer* buffer, I32 x, I32 y, JM_Color value);

U0 jm_rasterize_model(const JM_RasterBuffer* buffer, const JM_Model* model, JM_Mat4 modelMatrix, JM_Mat4 viewMatrix, JM_Mat4 projectionMatrix);

JM_Vec2 jm_transform_world_to_screen();

#endif