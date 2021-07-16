#ifndef READBMP_H_INCLUDED
#define READBMP_H_INCLUDED

typedef struct BitMapFile
{
   int sizeX;
   int sizeY;
   unsigned char *data;
} BitMapFile;

BitMapFile *readBMP(char* filename);


#endif // READBMP_H_INCLUDED

