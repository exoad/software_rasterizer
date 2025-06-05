#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"

JM_RasterBuffer *jm_create_raster_buffer(const I32 width, const I32 height)
{
    assert(width > 0 && "jm_create_raster_buffer: Raster buffer width must be positive");
    assert(height > 0 && "jm_create_raster_buffer: Raster buffer height must be positive!");
    JM_Color *data = malloc((usize) (width * height) * sizeof(JM_Color));
    if(data == NULL)
    {
        printerr("Failed to allocate memory to fit a new raster buffer with size %d, %d", width, height);
        return NULL;
    }
    JM_RasterBuffer *buffer = malloc(sizeof(JM_RasterBuffer));
    if(buffer == NULL)
    {
        printerr("%s", "Failed to allocate memory for JM_RasterBuffer struct.\n");
        free(data);
        return NULL;
    }
    buffer->data = data;
    buffer->width = width;
    buffer->height = height;
    return buffer;
}

U0 jm_destroy_raster_buffer(JM_RasterBuffer *buffer)
{
    if(buffer != NULL)
    {
        free(buffer->data);
        buffer->data = NULL;
        free(buffer);
    }
}

JM_Color jm_get_raster_pixel(const JM_RasterBuffer *buffer, const I32 x, const I32 y)
{
    assert(buffer != NULL && "jm_get_raster_pixel: Buffer cannot be NULL!");
    assert(buffer->data != NULL && "jm_get_raster_pixel: Buffer data cannot be NULL!");
    assert(x >= 0 && x < buffer->width && "jm_get_raster_pixel: X coordinate out of bounds!");
    assert(y >= 0 && y < buffer->height && "jm_get_raster_pixel: Y coordinate out of bounds!");
    return buffer->data[y * buffer->width + x];
}

U0 jm_set_raster_pixel(const JM_RasterBuffer *buffer, const I32 x, const I32 y, const JM_Color value)
{
    ensure(buffer);
    ensure(buffer->data);
    if(x >= 0 && x < buffer->width)
    {
        printerr("%s", "X coordinate is out of bounds!");
        return;
    }
    if(y >= 0 && y < buffer->height)
    {
        printerr("%s", "Y coordinate is out of bounds!");
        return;
    }
    buffer->data[y * buffer->width + x] = value;
}
