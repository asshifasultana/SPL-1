#include<stdio.h>
#include<stdlib.h>
#include"hamming.h"


void includeParityBits(int data[4],int out[7]){
    out[0] = data[0] ^ data[1] ^ data[3]; 
    out[1] = data[0] ^ data[2] ^ data[3]; 
    out[2] = data[0];                     
    out[3] = data[1] ^ data[2] ^ data[3]; 
    out[4] = data[1];                     
    out[5] = data[2];                     
    out[6] = data[3];                     
}

int hammingEncode(int *bitstream, int bitCount, int **encodedStream){
    int block= (bitCount+3)/4;
    int totalBits= block*7;

    *encodedStream= (int*)malloc(totalBits*sizeof(int));
    if(!(*encodedStream)){
        printf("Error: Memory allocation failed!");
        return -1;
    }

    for(int i=0;i<block;i++){
        int data[4]={0,0,0,0};
        for(int j=0;j<4;j++){
            if(i*4+j<bitCount){
                data[j]=bitstream[i*4+j];
            }
        }

        int encoded[7];
        includeParityBits(data,encoded);
        
        // ADD THIS - Copy encoded bits to output stream
        for(int j=0;j<7;j++){
            (*encodedStream)[i*7+j] = encoded[j];
        }
    }

    return totalBits;

}

void errorDetection(int encoded[7],int data[4]){
    int p1= encoded[0]^encoded[2]^encoded[4]^encoded[6];
    int p2= encoded[1]^encoded[2]^encoded[5]^encoded[6];
    int p3= encoded[3]^encoded[4]^encoded[5]^encoded[6];

    int error= p3*4+p2*2+p1;

    if(error>0 && error<=7){
        encoded[error-1]^=1;
    }

    data[0] = encoded[2]; 
    data[1] = encoded[4]; 
    data[2] = encoded[5]; 
    data[3] = encoded[6]; 
}

int hammingDecode(int *encodedStream, int encodedBitCount, int **decodedStream){
    int block= encodedBitCount/7;
    int totalBits= block*4;

    *decodedStream=(int *)malloc(totalBits*sizeof(int));

    if(!(*decodedStream)){
        printf("Error: Memory allocation failed!");
        return -1;
    }

    for(int i=0;i<block;i++){
        int encoded[7];
        for(int j=0;j<7;j++){
            encoded[j]=encodedStream[i*7+j];
        }
        int data[4];
        errorDetection(encoded,data);

        for(int j=0;j<4;j++){
            (*decodedStream)[i*4+j]= data[j];
        }
    }

    return totalBits;
}