#include "prebaked_rasters.h"

JM_RasterBuffer *jm_prebaked_uvmap(const I32 width, const I32 height)
{
    JM_RasterBuffer *buffer = jm_create_raster_buffer(width, height);
    for(I32 i = 0;i < buffer->height;i++)
        for(I32 j = 0;j < buffer->width;j++)
            jm_set_raster_pixel(buffer, j, i, (JM_Color){
                                    .r = (uint8) ((F32) j / ((F32) buffer->width - 1.0f) * 255.0f),
                                    .g = (uint8) ((F32) i / ((F32) buffer->height - 1.0f) * 255.0f),
                                    .b = 0,
                                    .a = 255
                                });
    return buffer;
}

JM_RasterBuffer *jm_prebaked_filled(const JM_Color color, const I32 width, const I32 height)
{
    JM_RasterBuffer *buffer = jm_create_raster_buffer(width, height);
    for(I32 i = 0;i < buffer->height;i++)
        for(I32 j = 0;j < buffer->width;j++)
            jm_set_raster_pixel(buffer, j, i, color);
    return buffer;
}
