#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include "readBMP.h"

// Routine to read an uncompressed 24-bit color RGB bmp file into a
// 32-bit color RGBA bitmap file (A value being set to 1).
// Code adapted from :
// https://github.com/slackmoehrle/Computer-Graphics-Through-OpenGL-2nd/blob/master/Chapter12/LoadTextures/getbmp.cpp
BitMapFile *readBMP(char *filename)
{
    GLint offset, headerSize;
    GLuint padding, sizeScanline, sizeStorage;
    int startScanline, endScanlineImageData, temp, x, y;
    FILE *infile;
    struct BitMapFile *bmpRGB, *bmpRGBA;

    // Initialize bitmap files for RGB (input) and RGBA (output).
    bmpRGB = malloc(sizeof(struct BitMapFile));
    bmpRGBA = malloc(sizeof(struct BitMapFile));

    // Read input bmp file name
    infile = fopen(filename, "rb");
    if (infile ==  NULL)
    {
        return NULL;
    }

    // get data offset
    fseek(infile, 10, SEEK_SET);
    // get offset and header size
    fread(&offset, 4, 1, infile);
    fread(&headerSize, 4, 1, infile);

    // get image width and height
    fseek(infile, 18, SEEK_SET);
    fread((GLubyte *)&bmpRGB->sizeX, 4, 1, infile);
    fread((GLubyte *)&bmpRGB->sizeY, 4, 1, infile);

    // Determine the length of zero-byte padding of the scanlines
    // (each scanline of a bmp file is 4-byte aligned by padding with zeros).
    padding = (3 * bmpRGB->sizeX) % 4 ? 4 - (3 * bmpRGB->sizeX) % 4 : 0;

    // Add the padding to determine size of each scanline.
    sizeScanline = 3 * bmpRGB->sizeX + padding;

    // Allocate storage for image in input bitmap file.
    sizeStorage = sizeScanline * bmpRGB->sizeY;
    bmpRGB->data = malloc(sizeStorage);

    // Read bmp file image data into input bitmap file.
    fseek(infile, offset, SEEK_SET);
    fread((unsigned char *) bmpRGB->data , 1, sizeStorage, infile);

    // Reverse color values from BGR (bmp storage format) to RGB.
    for (y = 0; y < bmpRGB->sizeY; y++) {
       startScanline = y * sizeScanline; // Start position of y'th scanline.
       endScanlineImageData = startScanline + 3 * bmpRGB->sizeX; // Image data excludes padding.
       for (x = startScanline; x < endScanlineImageData; x += 3) {
          temp = bmpRGB->data[x];
    	  bmpRGB->data[x] = bmpRGB->data[x+2];
    	  bmpRGB->data[x+2] = temp;
       }
    }

    // Set image width and height values and allocate storage for image in output bitmap file.
    bmpRGBA->sizeX = bmpRGB->sizeX;
    bmpRGBA->sizeY = bmpRGB->sizeY;
    bmpRGBA->data = malloc(4*bmpRGB->sizeX*bmpRGB->sizeY);

    // Copy RGB data from input to output bitmap files, set output A to 1.
    for(x = 0; x < 4*bmpRGB->sizeY * bmpRGB->sizeX; x+=4) {
    	bmpRGBA->data[x] = bmpRGB->data[(x/4)*3];
    	bmpRGBA->data[x+1] = bmpRGB->data[(x/4)*3+1];
    	bmpRGBA->data[x+2] = bmpRGB->data[(x/4)*3+2];
    	bmpRGBA->data[x+3] = 0xFF;
    }

    // free temporary memory
    free(bmpRGB->data);

    // close image file
    fclose(infile);

    return bmpRGBA;
}
