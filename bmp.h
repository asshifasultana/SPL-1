#ifndef BMP_H
#define BMP_H

#include<stdio.h>

#pragma pack(push,1)

typedef struct{
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
}BMPFILEHEADER;

typedef struct{
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short int biPlanes;
    unsigned short int biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPixelsPerMeter;
    int biYPixelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
}BMPIMAGEHEADER;

#pragma pack(pop)

int readBMP(const char *filename,unsigned char **image,int *imageSize,BMPFILEHEADER *fileHeader,BMPIMAGEHEADER *infoHeader);

int writeBMP(const char *filename,unsigned char *image,int imageSize, BMPFILEHEADER *fileHeader, BMPIMAGEHEADER *infoHeader);

#endif
