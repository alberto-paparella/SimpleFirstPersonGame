/**
 * File to generate basic shapes.
 *
 * IN DETAIL: the functions in this file fill the arrays given
 * with point coordinates.
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cglm/cglm.h>
#include <cglm/types-struct.h>
#include <stdio.h>
#include <math.h>

#include "shapes.h"

//FLOOR
/**
 * Fills the arrays for coordinates, normals and texture-coordinates to generate a rectangle.
 * @return Nothing.
 */
void fillSquVertexArray(Vertex squVertices[4]){

    //VERTEX 0
    //coordinate
    squVertices[0].coords.x = -200.0;
    squVertices[0].coords.y = -5.0;
    squVertices[0].coords.z = -200.0;
    squVertices[0].coords.w = 1.0;
    //normali
    squVertices[0].normal.x = 0.0;
    squVertices[0].normal.y = 1.0;
    squVertices[0].normal.z = 0.0;
    //texture
    squVertices[0].texCoords.x = 0.0;
    squVertices[0].texCoords.y = 0.0;

    squVertices[1].coords.x = -200.0;
    squVertices[1].coords.y = -5.0;
    squVertices[1].coords.z = 200.0;
    squVertices[1].coords.w = 1.0;
    squVertices[1].normal.x = 0.0;
    squVertices[1].normal.y = 1.0;
    squVertices[1].normal.z = 0.0;
    squVertices[1].texCoords.x = 100.0;
    squVertices[1].texCoords.y = 0.0;

    squVertices[2].coords.x = 200.0;
    squVertices[2].coords.y = -5.0;
    squVertices[2].coords.z = -200.0;
    squVertices[2].coords.w = 1.0;
    squVertices[2].normal.x = 0.0;
    squVertices[2].normal.y = 1.0;
    squVertices[2].normal.z = 0.0;
    squVertices[2].texCoords.x = 100.0;
    squVertices[2].texCoords.y = 100.0;

    squVertices[3].coords.x = -200.0;
    squVertices[3].coords.y = -5.0;
    squVertices[3].coords.z = -200.0;
    squVertices[3].coords.w = 1.0;
    squVertices[3].normal.x = 0.0;
    squVertices[3].normal.y = 1.0;
    squVertices[3].normal.z = 0.0;
    squVertices[3].texCoords.x = 0.0;
    squVertices[3].texCoords.y = 100.0;

}

void fillSquIndices(unsigned int squIndices[1][4]){
    squIndices[0][0] = 0;
    squIndices[0][1] = 1;
    squIndices[0][2] = 2;
    squIndices[0][3] = 3;
}

void fillSquCounts(int squCounts[1]){ //one triangle strip with 4 vertices
    squCounts[0] = 4;
}

// Fill the array of buffer offsets
void fillSquOffsets(void* squOffsets[1])
{
    squOffsets[0] = 0;
}

/**
 * Fills the coordinates arrays and the pointers
 * @param squVertices[4] The array that will contain the vertex coordinates data.
 */
void fillSqu(Vertex squVertices[4], unsigned int squIndices[1][4], int squCounts[1], void* squOffsets[1])
{
    fillSquVertexArray(squVertices);
    fillSquIndices(squIndices);
    fillSquOffsets(squOffsets);
    fillSquCounts(squCounts);
}

//WALLS
/**
 * Fills the arrays for coordinates, normals and texture-coordinates to generate a parallelepiped.
 * @return Nothing.
 */
void fillParVertexArray(Vertex parVertices[4*4]){ //vertici * 4 facce
    
    //PRIMA FACCIA
    parVertices[0].coords.x = 10.0;
    parVertices[0].coords.y = 5.0;
    parVertices[0].coords.z = 10.0;
    parVertices[0].coords.w = 1.0;
    parVertices[0].normal.x = 0.0;
    parVertices[0].normal.y = 0.0;
    parVertices[0].normal.z = 1.0;

    parVertices[1].coords.x = 10.0;
    parVertices[1].coords.y = -5.0;
    parVertices[1].coords.z = 10.0;
    parVertices[1].coords.w = 1.0;
    parVertices[1].normal.x = 0.0;
    parVertices[1].normal.y = 0.0;
    parVertices[1].normal.z = 1.0;

    parVertices[2].coords.x = -10.0;
    parVertices[2].coords.y = 5.0;
    parVertices[2].coords.z = 10.0;
    parVertices[2].coords.w = 1.0;
    parVertices[2].normal.x = 0.0;
    parVertices[2].normal.y = 0.0;
    parVertices[2].normal.z = 1.0;

    parVertices[3].coords.x = -10.0;
    parVertices[3].coords.y = -5.0;
    parVertices[3].coords.z = 10.0;
    parVertices[3].coords.w = 1.0;
    parVertices[3].normal.x = 0.0;
    parVertices[3].normal.y = 0.0;
    parVertices[3].normal.z = 1.0;

    //SECONDA FACCIA
    parVertices[4].coords.x = -10.0;
    parVertices[4].coords.y = 5.0;
    parVertices[4].coords.z = 10.0;
    parVertices[4].coords.w = 1.0;
    parVertices[4].normal.x = -1.0;
    parVertices[4].normal.y = 0.0;
    parVertices[4].normal.z = 0.0;

    parVertices[5].coords.x = -10.0;
    parVertices[5].coords.y = -5.0;
    parVertices[5].coords.z = 10.0;
    parVertices[5].coords.w = 1.0;
    parVertices[5].normal.x = -1.0;
    parVertices[5].normal.y = 0.0;
    parVertices[5].normal.z = 0.0;

    parVertices[6].coords.x = -10.0;
    parVertices[6].coords.y = 5.0;
    parVertices[6].coords.z = -10.0;
    parVertices[6].coords.w = 1.0;
    parVertices[6].normal.x = -1.0;
    parVertices[6].normal.y = 0.0;
    parVertices[6].normal.z = 0.0;

    parVertices[7].coords.x = -10.0;
    parVertices[7].coords.y = -5.0;
    parVertices[7].coords.z = -10.0;
    parVertices[7].coords.w = 1.0;
    parVertices[7].normal.x = -1.0;
    parVertices[7].normal.y = 0.0;
    parVertices[7].normal.z = 0.0;

    //TERZA FACCIA
    parVertices[8].coords.x = -10.0;
    parVertices[8].coords.y = 5.0;
    parVertices[8].coords.z = -10.0;
    parVertices[8].coords.w = 1.0;
    parVertices[8].normal.x = 0.0;
    parVertices[8].normal.y = 0.0;
    parVertices[8].normal.z = -1.0;

    parVertices[9].coords.x = -10.0;
    parVertices[9].coords.y = -5.0;
    parVertices[9].coords.z = -10.0;
    parVertices[9].coords.w = 1.0;
    parVertices[9].normal.x = 0.0;
    parVertices[9].normal.y = 0.0;
    parVertices[9].normal.z = -1.0;

    parVertices[10].coords.x = 10.0;
    parVertices[10].coords.y = 5.0;
    parVertices[10].coords.z = -10.0;
    parVertices[10].coords.w = 1.0;
    parVertices[10].normal.x = 0.0;
    parVertices[10].normal.y = 0.0;
    parVertices[10].normal.z = -1.0;

    parVertices[11].coords.x = 10.0;
    parVertices[11].coords.y = -5.0;
    parVertices[11].coords.z = -10.0;
    parVertices[11].coords.w = 1.0;
    parVertices[11].normal.x = 0.0;
    parVertices[11].normal.y = 0.0;
    parVertices[11].normal.z = -1.0;

    //QUARTA FACCIA
    parVertices[12].coords.x = 10.0;
    parVertices[12].coords.y = 5.0;
    parVertices[12].coords.z = -10.0;
    parVertices[12].coords.w = 1.0;
    parVertices[12].normal.x = 1.0;
    parVertices[12].normal.y = 0.0;
    parVertices[12].normal.z = 0.0;

    parVertices[13].coords.x = 10.0;
    parVertices[13].coords.y = -5.0;
    parVertices[13].coords.z = -10.0;
    parVertices[13].coords.w = 1.0;
    parVertices[13].normal.x = 1.0;
    parVertices[13].normal.y = 0.0;
    parVertices[13].normal.z = 0.0;

    parVertices[14].coords.x = 10.0;
    parVertices[14].coords.y = 5.0;
    parVertices[14].coords.z = 10.0;
    parVertices[14].coords.w = 1.0;
    parVertices[14].normal.x = 1.0;
    parVertices[14].normal.y = 0.0;
    parVertices[14].normal.z = 0.0;

    parVertices[15].coords.x = 10.0;
    parVertices[15].coords.y = -5.0;
    parVertices[15].coords.z = 10.0;
    parVertices[15].coords.w = 1.0;
    parVertices[15].normal.x = 1.0;
    parVertices[15].normal.y = 0.0;
    parVertices[15].normal.z = 0.0;

}
void fillParIndices(unsigned int parIndices[4][4]){
    for(int v=0; v<4; v++)
        for(int f=0; f<4; f++){
            parIndices[v][f] = v*4+f;
            parIndices[v][f+1] = (v+1)*4+f;
        }
}
void fillParCounts(int parCounts[4]){ //Four triangle strips with 4 vertices each
    for(int f=0; f<4; f++){
        parCounts[f] = 4;
    }
}
void fillParOffsets(void* parOffsets[4]){
     /*  parOffsets[v] = (GLvoid*)(v*sizeof(unsigned int)); */
    for(int f = 0; f < 4; f++) 
      parOffsets[f] = (GLvoid*)(4*f*sizeof(unsigned int));
}

/**
 * Fills the coordinates arrays and the pointers
 * @param l1 The length of a side of the base rectangle.
 * @param l2 The length of a side of the base rectangle.
 * @param h The height of the parallelepiped.
 * @param parVertices[8] The array that will contain the vertex coordinates data.
 */
void fillPar(Vertex parVertices[4*4], unsigned int parIndices[4][4], int parCounts[4], void* parOffsets[4]){
    fillParVertexArray(parVertices);
    fillParIndices(parIndices);
    fillParCounts(parCounts);
    fillParOffsets(parOffsets);
}

