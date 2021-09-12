#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <GL/glew.h>
#include <cglm/cglm.h>

typedef struct {
    size_t x;
    size_t y;
    float flex[];
} array2d_tf;

typedef struct {
    size_t x;
    size_t y;
    unsigned int flex[];
} array2d_tui;


typedef struct collision_box{
    vec3 center_position;
    float X_size;
    float Y_size;
    float Z_size;
} CollisionBox;

typedef struct object{
    Vertex objVertices;
    int *objCounts;
    void *objOffsets;
} Object;

#endif
