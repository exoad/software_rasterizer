#ifndef __JM_OBJ_READER_H__
#define __JM_OBJ_READER_H__

#include "def.h"
#include "geometry.h"

typedef struct JM_ObjFaceCorner
{
    I64 vIndex;
    I64 vtIndex;
    I64 vnIndex;
} JM_ObjFaceCorner;

typedef struct JM_ObjData
{
    JM_ObjFaceCorner** faces;
    JM_Vec3* vertices;
    JM_Vec3* normals;
    JM_Vec2* uvs;
} JM_ObjData;

U0 jm_obj_freedata(JM_ObjData* data);

I8* jm_obj_facecorner_tostring(const JM_ObjFaceCorner* face);

JM_ObjData* jm_obj_parsefile(const I8* pathToFile);

#endif