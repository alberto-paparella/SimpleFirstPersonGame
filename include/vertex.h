#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <cglm/cglm.h>
#include <cglm/types-struct.h>

typedef struct Vertex {
   vec4s coords;
   vec3s normal;
   vec2s texCoords;
} Vertex;

#endif // VERTEX_H_INCLUDED
