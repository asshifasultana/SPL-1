
#include<stdio.h>
#include<stdlib.h>

#pragma pack(push,1)

typedef struct{
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits
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

#pragma pack(pop);

int main(){

    const char *inputFile = "lena.bmp";
    const char *outputFile = "stega.bmp";

    FILE *file = fopen(inputFile,"rb");
    if(!file){
        printf("Error: Couldn't open the file %s \n",inputFile);
        return 1;
    }

    BMPFILEHEADER header;
    fread(&header,sizeof(BMPFILEHEADER),1,file);

    if(header.bfType!=0X4D42)
    {
        printf("Error: Not a valid BMP file \n");
        fclose(file);
        return 1;
    }

    unsigned char *image = (unsigned char*)malloc(header.bfSize);
    if(!image)
    {
        printf("Error: Memory Allocation failed \n");
        fclose(file);
        return 1;
    }

    fseek(file,header.bfOffBits,SEEK_SET);
    fread(image,1,header.bfSize,file);
    fclose(file);

    return 0;
}