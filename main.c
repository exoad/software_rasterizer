// ---------------------------------------


#define RASTER_WIDTH 1000
#define RASTER_HEIGHT 1000
#define TRIANGLE_COUNT 256


// ---------------------------------------
#include <math.h>
#include <stdio.h>

#define STB_DS_IMPLEMENTATION

#include "external/stb_ds.h"
#include "bmp_writer.h"
#include "chronos.h"
#include "prebaked_rasters.h"
#include "renderer.h"
#include "utils.h"
#include "resource_loaders/resource_loader.h"

static JM_RasterBuffer* buffer = NULL;
static JM_Model* model = NULL;
// static F32 random_f32(const F32 min, const F32 max)
// {
//     return min + (F32) rand() / RAND_MAX * (max - min); // NOLI32(cert-msc30-c, cert-msc50-cpp)
// }

// static U0 render()
// {
//     for(I32 k = 0;k < TRIANGLE_COUNT;k++)
//     {
//         const JM_Vec2 a = {
//             random_f32(0.f, (F32) buffer->width - 1.f),
//             random_f32(0.f, (F32) buffer->height - 1.f)
//         };
//         const JM_Vec2 b = {
//             random_f32(0.f, (F32) buffer->width - 1.f),
//             random_f32(0.f, (F32) buffer->height - 1.f)
//         };
//         const JM_Vec2 c = {
//             random_f32(0.f, (F32) buffer->width - 1.f),
//             random_f32(0.f, (F32) buffer->height - 1.f)
//         };
//         const JM_Color color = jm_color_random();
//         for(
//             I32 y = (I32) jm_clampf(floorf(jm_min3f(a.y, b.y, c.y)), 0.f, (F32) buffer->height - 1.f);
//             y <= (I32) jm_clampf(ceilf(jm_max3f(a.y, b.y, c.y)), 0.f, (F32) buffer->height - 1.f);
//             y++
//         )
//             for(
//                 I32 x = (I32) jm_clampf(floorf(jm_min3f(a.x, b.x, c.x)), 0.f, (F32) buffer->width - 1.f);
//                 x <= (I32) jm_clampf(ceilf(jm_max3f(a.x, b.x, c.x)), 0.f, (F32) buffer->width - 1.f);
//                 x++
//             )
//                 if(jm_triangles_encloses((JM_Vec2){ (F32) x, (F32) y }, a, b, c))
//                     jm_set_raster_pixel(buffer, x, y, color);
//     }
// }

// static U0 _test_write_bmp()
// {
//     srand((U32) time(NULL)); // NOLI32(cert-msc30-c, cert-msc51-cpp)
//     buffer = jm_prebaked_filled(JM_COLOR_BLACK, RASTER_WIDTH, RASTER_HEIGHT);
//     ensure(buffer);
//     println("Rendering took %.0f ms (CPU Time)", JM_TIMED_BLOCK({ render(); }) * 1000);
//     jm_bmp_write(
//                  &(JM_BMP_Write_ArgDesc)
//                  {
//                      .fileName = "output.bmp",
//                      .width = buffer->width,
//                      .height = buffer->height,
//                      .bitsPerPixel = 24,
//                      .data = jm_raster_buffer_to_bmp_data(buffer)
//                  });
// }

static JM_Vec3 cameraPos = (JM_Vec3) { 0.f, 0.f, 5.f };
static const JM_Vec3 cameraTarget = (JM_Vec3) { 0.f, 0.f, 0.f };
static const JM_Vec3 cameraUp = (JM_Vec3) { 0.f, 1.f, 0.f };

static U0 render_frame(JM_RasterBuffer* framebuffer, JM_Model* modelToRender) 
{
    F32 fov = 45.f;
    F32 aspectRatio = (F32) framebuffer->width / (F32) framebuffer->height;
    F32 nearPlane = .1f;
    F32 farPlane = 100.f;
    JM_Mat4 viewMatrix = jm_mat4_lookat(cameraPos, cameraTarget, cameraUp);
    JM_Mat4 projectionMatrix = jm_mat4_perspective(
        jm_deg_to_rad(fov),
        aspectRatio,
        nearPlane,
        farPlane
    );
    JM_Mat4 modelMatrix = jm_mat4_identity();
    static F32 rotationAngle = 0.f;
    modelMatrix = jm_mat4_rotate_y(rotationAngle);
    rotationAngle += jm_deg_to_rad(1.f);
    jm_rasterize_model(
        framebuffer,
        modelToRender,
        modelMatrix,
        viewMatrix, 
        projectionMatrix
    );
}

U0 render(const I8* outFile)
{
    render_frame(buffer, model);
    jm_bmp_write(&(JM_BMP_Write_ArgDesc)
                {
                    .fileName = outFile,
                    .width = buffer->width,
                    .height = buffer->height,
                    .bitsPerPixel = 24,
                    .data = jm_raster_buffer_to_bmp_data(buffer)
                });
}

U0 load(const I8* objFile)
{
    model = jm_res_constructobj(jm_obj_parsefile(objFile));
    buffer = jm_prebaked_filled((JM_Color) { 0 }, RASTER_WIDTH, RASTER_HEIGHT);
    ensure(buffer);
}

I32 main()
{
    load("resources/cube.obj");
    println("%s", "+--- Type 'quit' or 'q' to exit ---+");
    println("%s", "Hello World!");
    render("cube.bmp");
    I8 lineBuffer[256];
    while(true)
    {
        printf("> ");
        if(fgets(lineBuffer, sizeof(lineBuffer), stdin) == NULL)
        {
            break;
        }
        lineBuffer[strcspn(lineBuffer, "\n")] = 0;
        if(strcmp(lineBuffer, "quit") == 0)
        {
            break;
        }
        else if(strcmp(lineBuffer, "r") == 0)
        {
            println("%s", "Resetting & Rerendering...");
            cameraPos = (JM_Vec3) { 0.f, 0.f, 5.f };
            render("cube.bmp");
        }
        else if(strlen(lineBuffer) >= 1)
        {
            char action = lineBuffer[0];
            I32 amount = atoi(&lineBuffer[1]);  // Parse amount from the rest
            switch(action)
            {
                case 'W':
                case 'w':
                    cameraPos.z -= (F32) amount;
                    println("Camera moved forward: (%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y, cameraPos.z);
                    render("cube.bmp");
                    break;
                case 'S':
                case 's':
                    cameraPos.z += (F32) amount;
                    println("Camera moved backward: (%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y, cameraPos.z);
                    render("cube.bmp");
                    break;
                case 'A':
                case 'a':
                    cameraPos.x -= (F32) amount;
                    println("Camera moved left: (%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y, cameraPos.z);
                    render("cube.bmp");
                    break;
                case 'D':
                case 'd':
                    cameraPos.x += (F32) amount;
                    println("Camera moved right: (%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y, cameraPos.z);
                    render("cube.bmp");
                    break;
                default:
                    println("%s", "Unknown command.");
                    break;
            }
        }
        else
        {
            println("%s", "Unknown command.");
        }
    }
    return 0;
}
