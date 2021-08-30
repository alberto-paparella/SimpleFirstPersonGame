#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

#include "vertex.h"
//PARALLELEPIPEDO
void fillParVertexArray(float l1, float l2, float h, Vertex parVertices[8]);
void fillParIndices(unsigned int parIndices[8]);
void fillParCounts(int parCounts[8]);
void fillParOffsets(void* parOffsets[8]);

void fillPar(float l1, float l2, float h, Vertex parVertices[8], unsigned int parIndices[8], int parCounts[8], void* parOffsets[8]);

void fillSquVertexArray(float l1, float l2, Vertex squVertices[4]);
void fillSqu(float l1, float l2, Vertex squVertices[4]);

#endif