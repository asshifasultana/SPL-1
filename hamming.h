#ifndef HAMMING_H
#define HAMMING_H

#include<stdio.h>
#include<stdlib.h>

int hammingEncode(int *bitstream, int bitCount, int **encodedStream);

int hammingDecode(int *encodedStream, int encodedBitCount, int **decodedStream);

#endif