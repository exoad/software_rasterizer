#ifndef __RESOURCE_LOADER_H__
#define __RESOURCE_LOADER_H__

#include "allocator.h"
#include "geometry.h"

typedef enum JM_ResourceFormat
{
    JM_RES_FORMAT_UNKNOWN,
    JM_RES_FORMAT_OBJ
} JM_ResourceFormat;

JM_Scene* jm_resources_load_scene(const I8* filePath, JM_ResourceFormat format, JM_Arena* arena);

U0 jm_resources_destroy_scene(JM_Scene* scene);

#endif