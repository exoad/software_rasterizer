#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "renderer.h"
#include "bmp_writer.h"
#include "prebaked_rasters.h"
#include "chronos.h"

#define RASTER_WIDTH 1920
#define RASTER_HEIGHT 1920
#define TRIANGLE_COUNT 256

static JM_RasterBuffer* buffer = NULL;

static float random_float(float min, float max)
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
            random_float(0.f, (float) buffer->width - 1.0f),
            random_float(0.f, (float) buffer->height - 1.0f)
        };
        const JM_Vec2 c = {
            random_float(0.f, (float) buffer->width - 1.f),
            random_float(0.f, (float) buffer->height - 1.f)
        };
        const JM_Color tri_color = jm_color_random();
        println("Triangle %d: A=(%.2f, %.2f) B=(%.2f, %.2f) C=(%.2f, %.2f) Color=(%d,%d,%d)",
               k + 1, a.x, a.y, b.x, b.y, c.x, c.y, tri_color.r, tri_color.g, tri_color.b);
        for(int y = 0; y < buffer->height; y++)
        {
            for(int x = 0; x < buffer->width; x++)
            {

                if(jm_triangles_encloses((JM_Vec2) { (float)x, (float)y }, a, b, c))
                    jm_set_raster_pixel(buffer, x, y, tri_color);
            }
        }
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