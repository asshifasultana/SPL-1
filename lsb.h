#ifndef LSB_H
#define LSB_H

#include <stdio.h>

typedef struct {
    int index;
    unsigned char before;
    unsigned char after;
    int bit;
} PixelChange;

int embedLSB(
    unsigned char *image,
    int imageSize,
    int *chaosSeq,
    int *bitStream,
    int bitCount,
    int table[256],
    PixelChange *changes,
    int *changeCount
);

int extractLSB(
    unsigned char* image,
    int imageSize,
    int *chaosSeq,
    int *bitStream,
    int* bitCount,
    int table[256]
);

#endif