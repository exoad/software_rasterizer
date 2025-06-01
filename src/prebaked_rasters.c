#include "prebaked_rasters.h"

JM_RasterBuffer* jm_prebaked_uvmap(const int width, const int height)
{
    JM_RasterBuffer* buffer = jm_create_raster_buffer(width, height);
    for(int i = 0; i < buffer->height; i++)
        for(int j = 0; j < buffer->width; j++)
            jm_set_raster_pixel(buffer, j, i, (JM_Color) {
                .r = (uint8) ((float) j / ((float) buffer->width - 1.0f) * 255.0f),
                .g = (uint8) ((float) i / ((float) buffer->height - 1.0f) * 255.0f),
                .b = 0,
                .a = 255
            });
    return buffer;
}

JM_RasterBuffer* jm_prebaked_filled(const JM_Color color, const int width, const int height)
{
    JM_RasterBuffer* buffer = jm_create_raster_buffer(width, height);
    for(int i = 0; i < buffer->height; i++)
        for(int j = 0; j < buffer->width; j++)
            jm_set_raster_pixel(buffer, j, i, color);
    return buffer;
}