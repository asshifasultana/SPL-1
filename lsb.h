#ifndef LSB_H
#define LSB_H

#include<stdio.h>

int embedLSB(unsigned char *image, int imageSize, int *chaosSeq, int *bitStream, int bitCount, int table[256]);


int extractLSB(unsigned char* image, int imageSize, int *chaosSeq, int *bitStream, int* bitCount, int table[256]);
#endif



