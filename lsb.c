#include<stdio.h>
#include<stdlib.h>
#include"lsb.h"

int embedLSB(unsigned char* image, int imageSize, int *chaosSeq, int *bitStream, int bitCount, int table[256]){
    
    if(imageSize<32+256*32+bitCount){
        printf("Error: Image too small to embedd data \n");
        return 1;
    }
    
    int chaosIndex=0;
    int bit=0;

    for(int i=31;i>=0;i--){
        bit = (bitCount >> i)&1;

        if(bit == 1){
            image[chaosSeq[chaosIndex]]|=1;
        }
        else{
            image[chaosSeq[chaosIndex]]&=0xFE;
        }

        chaosIndex++;
    }

    for(int i=0;i<256;i++){
        int val=table[i];

        for(int j=31;j>=0;j--){
            bit=(val >> j)&1;

            if(bit == 1){
            image[chaosSeq[chaosIndex]]|=1;
        }
        else{
            image[chaosSeq[chaosIndex]]&=0xFE;
        }

        chaosIndex++;
        }
    }

    for(int i=0;i<bitCount;i++){
        bit = bitStream[i];

        if(bit ==1){
            image[chaosSeq[chaosIndex]]|=1;
        }

        else{
            image[chaosSeq[chaosIndex]] &=0xFE;
        }

        chaosIndex++;
    }

    return 0;

}

int extractLSB(unsigned char* image, int imageSize, int *chaosSeq, int *bitStream, int *bitCount, int table[256]){
    
    int chaosIndex=0;
    int bit;

    *bitCount=0;

    for(int i=31;i>=0;i--){
        bit= image[chaosSeq[chaosIndex]] &1;
        *bitCount |= (bit << i);
        chaosIndex++;
    }

    if(imageSize<32+256*32+*bitCount){
        printf("Error: Image is too small to hold the data\n");
        return 1;
    }

    for(int i=0;i<256;i++){
        int val=0;
        for(int j=31;j>=0;j--){
            bit=image[chaosSeq[chaosIndex]] &1;
            val |=(bit <<j);
            chaosIndex++;
        }
        table[i]=val;
    }

    for(int i=0;i<*bitCount;i++){
        bit = image[chaosSeq[chaosIndex]]&1;
        bitStream[i]=bit;
        chaosIndex++;
    }

    return 0;

}