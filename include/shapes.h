#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

#include "vertex.h"

/**
 * Parallelepiped
*/
void fillParVertexArray(vec3 center_position, float X, float Y, float Z, Vertex parVertices[4*4]);
void fillParIndices(unsigned int parIndices[4][4]);
void fillParCounts(int parCounts[4]);
void fillParOffsets(void* parOffsets[4]);
void fillPar(vec3 center_position, float X, float Y, float Z, Vertex parVertices[4*4], unsigned int parIndices[4][4], int parCounts[4], void* parOffsets[4]);

/**
 * Square
*/
void fillSquVertexArray(Vertex squVertices[4]);
void fillSquIndices(unsigned int squIndices[1][4]);
void fillSquCounts(int squCounts[1]);
void fillSquOffsets(void* squOffsets[1]); 
void fillSqu(Vertex squVertices[4], unsigned int squIndices[1][4], int squCounts[1], void* squOffsets[1]);

#endif