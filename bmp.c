#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int readBMP(const char *filename, unsigned char **image,int *imageSize, BMPFILEHEADER *header, BMPIMAGEHEADER *info)
{
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Couldn't open the file %s\n", filename);
        return 1;
    }

    if (fread(header, sizeof(BMPFILEHEADER), 1, file) != 1) {
        printf("Error: Failed to read BMP file header\n");
        fclose(file);
        return 1;
    }

    if (header->bfType != 0x4D42) {
        printf("Error: Not a valid BMP file\n");
        fclose(file);
        return 1;
    }

    if (fread(info, sizeof(BMPIMAGEHEADER), 1, file) != 1) {
        printf("Error: Failed to read BMP info header\n");
        fclose(file);
        return 1;
    }

    if (info->biBitCount != 24) {
        printf("Error: Only 24-bit BMP images are supported\n");
        fclose(file);
        return 1;
    }

    int width  = info->biWidth;
    int height = abs(info->biHeight);

    *imageSize = width *height * 3;

    *image = (unsigned char *)malloc(*imageSize);
    if (!(*image)) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    fseek(file, header->bfOffBits, SEEK_SET);

    if (fread(*image, 1, *imageSize, file) != *imageSize) {
        printf("Error: Failed to read image data\n");
        free(*image);
        fclose(file);
        return 1;
    }

    fclose(file);
    return 0;   
}
