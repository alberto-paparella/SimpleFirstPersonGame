#ifndef READBMP_H_INCLUDED
#define READBMP_H_INCLUDED

struct BitMapFile
{
   int sizeX;
   int sizeY;
   unsigned char *data;
};

struct BitMapFile *readBMP(char* filename);


#endif // READBMP_H_INCLUDED
