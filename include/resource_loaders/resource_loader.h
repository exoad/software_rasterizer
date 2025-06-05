#ifndef __RESOURCE_LOADER_H__
#define __RESOURCE_LOADER_H__

#include "geometry.h"
#include "def.h"
#include "obj_reader.h"

typedef struct JM_Model
{
    JM_Vec3* pts;
    JM_Vec3* triangles;
} JM_Model;

JM_Model* jm_res_constructobj(const JM_ObjData* face);

U0 jm_res_freemodel(JM_Model* model);

#endif