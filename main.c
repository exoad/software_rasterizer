#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "renderer.h"
#include "bmp_writer.h"
#include "prebaked_rasters.h"
#include "chronos.h"

#define RASTER_WIDTH 1920
#define RASTER_HEIGHT 1080
#define TRIANGLE_COUNT 256

static JM_RasterBuffer* buffer = NULL;

static inline float random_float(float min, float max)
{
    return min + (float) rand() / RAND_MAX * (max - min);
}

static void render()
{
   for (int k = 0; k < TRIANGLE_COUNT; k++)
   {
        const JM_Vec2 a = {
            random_float(0.f, (float) buffer->width - 1.f),
            random_float(0.f, (float) buffer->height - 1.f)
        };
        const JM_Vec2 b = {
            random_float(0.f, (float) buffer->width - 1.f),
            random_float(0.f, (float) buffer->height - 1.f)
        };
        const JM_Vec2 c = {
            random_float(0.f, (float) buffer->width - 1.f),
            random_float(0.f, (float) buffer->height - 1.f)
        };
        const JM_Color color = jm_color_random();
        for(
            int y = (int) jm_clampf(floorf(jm_min3f(a.y, b.y, c.y)), 0.f, (float) buffer->height - 1.f);
            y <= (int) jm_clampf(ceilf(jm_max3f(a.y, b.y, c.y)), 0.f, (float) buffer->height - 1.f);
            y++
        )
            for(
                int x = (int) jm_clampf(floorf(jm_min3f(a.x, b.x, c.x)), 0.f, (float) buffer->width - 1.f);
                x <= (int) jm_clampf(ceilf(jm_max3f(a.x, b.x, c.x)), 0.f, (float) buffer->width - 1.f);
                x++
            )
                if(jm_triangles_encloses((JM_Vec2) { (float) x, (float) y }, a, b, c))
                    jm_set_raster_pixel(buffer, x, y, color);
    }
}

int main()
{
    srand((unsigned int)time(NULL));
    buffer = jm_prebaked_filled(JM_COLOR_BLACK, RASTER_WIDTH, RASTER_HEIGHT);
    ASSERT_NOT_NULL(buffer);
    println("Rendering took %.0f ms (CPU Time)", JM_TIMED_BLOCK({ render(); }) * 1000);
    jm_bmp_write(
        &(JM_BMP_Write_ArgDesc)
    {
        .fileName = "output.bmp",
        .width = buffer->width,
        .height = buffer->height,
        .bitsPerPixel = 24,
        .data = jm_raster_buffert_to_bmp_data(buffer)
    });
}