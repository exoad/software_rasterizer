#include "renderer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

JM_RasterBuffer* jm_create_raster_buffer(const int width, const int height)
{
    assert(width > 0 && "jm_create_raster_buffer: Raster buffer width must be positive");
    assert(height > 0 && "jm_create_raster_buffer: Raster buffer height must be positive!");
    JM_Color* data = (JM_Color*) malloc((usize) (width * height) * sizeof(JM_Color));
    if (data == NULL)
    {
        fprintf(stderr, "Failed to allocate memory to fit a new raster buffer with size %d, %d", width, height);
        return NULL;
    }
    JM_RasterBuffer* buffer = (JM_RasterBuffer*) malloc(sizeof(JM_RasterBuffer));
    if (buffer == NULL)
    {
        fprintf(stderr, "ERROR: jm_create_raster_buffer: Failed to allocate memory for JM_RasterBuffer struct.\n");
        free(data);
        return NULL;
    }
    buffer->data = data;
    buffer->width = width;
    buffer->height = height;
    return buffer;
}

void jm_destroy_raster_buffer(JM_RasterBuffer* buffer)
{
    if (buffer != NULL)
    {
        free(buffer->data);
        buffer->data = NULL;
        free(buffer);
    }
}

JM_Color jm_get_raster_pixel(const JM_RasterBuffer* buffer, const int x, const int y)
{
    assert(buffer != NULL && "jm_get_raster_pixel: Buffer cannot be NULL!");
    assert(buffer->data != NULL && "jm_get_raster_pixel: Buffer data cannot be NULL!");
    assert(x >= 0 && x < buffer->width && "jm_get_raster_pixel: X coordinate out of bounds!");
    assert(y >= 0 && y < buffer->height && "jm_get_raster_pixel: Y coordinate out of bounds!");
    if (buffer == NULL || buffer->data == NULL ||
        x < 0 || x >= buffer->width ||
        y < 0 || y >= buffer->height)
    {
        fprintf(stderr, "WARNING: jm_get_raster_pixel: Invalid buffer or out of bounds coordinates (%d, %d) for %dx%d buffer. Returning 0.0f.\n", x, y, (buffer ? buffer->width : 0), (buffer ? buffer->height : 0));
        return (JM_Color) { 0.0f, 0.0f, 0.0f, 0.0f };
    }
    return buffer->data[y * buffer->width + x];
}

void jm_set_raster_pixel(JM_RasterBuffer* buffer, const int x, const int y, const JM_Color value)
{
    assert(buffer != NULL && "jm_set_raster_pixel: Buffer cannot be NULL!");
    assert(buffer->data != NULL && "jm_set_raster_pixel: Buffer data cannot be NULL!");
    assert(x >= 0 && x < buffer->width && "jm_set_raster_pixel: X coordinate out of bounds!");
    assert(y >= 0 && y < buffer->height && "jm_set_raster_pixel: Y coordinate out of bounds!");
    if (buffer == NULL || buffer->data == NULL ||
        x < 0 || x >= buffer->width ||
        y < 0 || y >= buffer->height)
    {
        fprintf(stderr, "WARNING: jm_set_raster_pixel: Invalid buffer or out of bounds coordinates (%d, %d) for %dx%d buffer. Value not set.\n",
            x, y, (buffer ? buffer->width : 0), (buffer ? buffer->height : 0));
        return;
    }
    buffer->data[y * buffer->width + x] = value;
}