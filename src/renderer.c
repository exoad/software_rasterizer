#include "renderer.h"
#include "external/stb_ds.h"
#include "utils.h"
#include "geometry.h"

JM_Vec2 jm_transform_world_to_screen(
    JM_Vec3 world_pos,
    JM_Mat4 model_matrix,
    JM_Mat4 view_matrix,
    JM_Mat4 projection_matrix,
    I32 screen_width,
    I32 screen_height
) {
    JM_Mat4 mvMatrix = jm_mat4_mul(view_matrix, model_matrix);
    JM_Mat4 mvpMatrix = jm_mat4_mul(projection_matrix, mvMatrix);
    JM_Vec4 clipPosHomo = jm_mat4_mul_vec4(mvpMatrix, (JM_Vec4) { .x = world_pos.x, .y = world_pos.y, .z = world_pos.z, .w = 1.f });
    if (clipPosHomo.w == 0.f)
    {
        return (JM_Vec2) { .x = -1.f, .y = -1.f };
    }
    F32 inv_w = 1.f / clipPosHomo.w;
    JM_Vec3 ndcPos = {
        .x = clipPosHomo.x * inv_w,
        .y = clipPosHomo.y * inv_w,
        .z = clipPosHomo.z * inv_w
    };
    JM_Vec2 screen_pos;
    screen_pos.x = (ndcPos.x + 1.f) * .5f * (F32)screen_width;
    screen_pos.y = (1.f - ndcPos.y) * .5f * (F32)screen_height;
    return screen_pos;
}


U0 jm_rasterize_model(const JM_RasterBuffer* buffer, const JM_Model* model, JM_Mat4 modelMatrix, JM_Mat4 viewMatrix, JM_Mat4 projectionMatrix)
{
    const I32 trianglesCount = arrlen(model->pts);
    for(I32 i = 0; i < trianglesCount; i += 3)
    {
        const JM_Color color = jm_vec3_to_color(model->triangles[i / 3]);
        const JM_Vec2 a = jm_transform_world_to_screen(model->pts[i], modelMatrix, viewMatrix, projectionMatrix, buffer->width, buffer->height);
        const JM_Vec2 b = jm_transform_world_to_screen(model->pts[i + 1], modelMatrix, viewMatrix, projectionMatrix, buffer->width, buffer->height);
        const JM_Vec2 c = jm_transform_world_to_screen(model->pts[i + 2], modelMatrix, viewMatrix, projectionMatrix, buffer->width, buffer->height);
        for(
            I32 y = (I32) jm_clampf(floorf(jm_min3f(a.y, b.y, c.y)), 0.f, (F32) buffer->height - 1.f);
            y <= (I32) jm_clampf(ceilf(jm_max3f(a.y, b.y, c.y)), 0.f, (F32) buffer->height - 1.f);
            y++
        )
            for(
                I32 x = (I32) jm_clampf(floorf(jm_min3f(a.x, b.x, c.x)), 0.f, (F32) buffer->width - 1.f);
                x <= (I32) jm_clampf(ceilf(jm_max3f(a.x, b.x, c.x)), 0.f, (F32) buffer->width - 1.f);
                x++
            )
                if(jm_triangles_encloses((JM_Vec2){ (F32) x, (F32) y }, a, b, c))
                    jm_set_raster_pixel(buffer, x, y, color);
    }
}