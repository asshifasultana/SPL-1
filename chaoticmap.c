#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"chaoticmap.h"

typedef struct {
    double value;
    int index;
}ChaoticValue;

void chaoticmap(int *seq, int imageSize, double x, double r){
    ChaoticValue *arr = (ChaoticValue*)malloc(imageSize*sizeof(ChaoticValue));

    if(!arr){
        printf("Memory allocation failed.\n");
        exit(1);
    }

    for(int i=0;i<imageSize;i++){
        x=r*x*(1-x);
        arr[i].value=x;
        arr[i].index=i;
    }

    for(int i=0;i<imageSize-1;i++){
        for(int j=0;j<imageSize-i-1;j++){
            if(arr[j].value > arr[j+1].value){
                ChaoticValue temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
        }
    }

    for(int i=0;i<imageSize;i++){
        seq[i]=arr[i].index;
    }

    free(arr);
}
