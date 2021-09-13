#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <cglm/cglm.h>

typedef struct CollisionBox
{
    vec3 center_position;
    float X_size;
    float Y_size;
    float Z_size;
} CollisionBox;

#endif
