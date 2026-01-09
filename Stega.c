
#include<stdio.h>
#include<stdlib.h>
#include"bmp.h"

int main(){

    int choice;

    printf("=========Image Steganography========= \n");
    printf("1.Encode text into image\n");
    printf("2.Decode text from image\n");
    printf("Enter your choice:");
    fflush(stdout);
    scanf("%d",&choice);

    if(choice==1){
         char inputFile[250];
    printf("Enter the BMP file name:");
    scanf("%s",inputFile);

    BMPFILEHEADER header;
    BMPIMAGEHEADER info;
    unsigned char *image = NULL;
    int imageSize = 0;

    if (readBMP(inputFile, &image, &imageSize, &header, &info) != 0) {
        printf("Failed to read BMP image.\n");
        return 1;
    }


    free(image);
    }
    return 0;
}