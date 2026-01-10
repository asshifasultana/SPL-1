
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

    char textFile[250];
    printf("Enter the secret text message file name:");
    scanf("%s",textFile);

    FILE *textfile = fopen(textFile,"rb");
    if(!textfile){
        printf("Error: Failed to read text file\n");
        return 1;
    }

    fseek(textfile, 0, SEEK_END);
    long textSize = ftell(textfile);
    rewind(textfile);

    int requiredBits = (textSize * 8) + 32;

    if (requiredBits > imageSize) {
        printf("Error: Image is too small to hold this message.\n");
    
        free(textfile);
        free(image);
        return 1;
}

    unsigned char *text = (unsigned char *)malloc(textSize+1);
    if(!text){
        printf("Error: Memory Allocation failed\n");
        fclose(textfile);
        free(image);
        return 1;
    }

    fread(text,1,textSize,textfile);
    fclose(textfile);
    text[textSize]='\0';
    //printf("Message is %s",text);

    }
    return 0;
}