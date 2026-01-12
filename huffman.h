#ifndef HUFFMAN_H
#define HUFFMAN_H

#include<stdio.h>
#include<stdlib.h>

#define MAX_CHAR 256

typedef struct Node {
    unsigned char ch;
    int freq;
    struct Node *left;
    struct Node *right;
} Node;


typedef struct{
    Node **node;
    int size;
}Minheap;

int Compression(unsigned char *text, int textSize, int **bitStream, int table[256]);

int Decompression(int *bitStream, int bitCount, int table[256],unsigned char **output, int *outputSize);
#endif