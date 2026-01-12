
#include<stdio.h>
#include<stdlib.h>
#include"bmp.h"
#include"lsb.h"
#include"huffman.h"
#include"chaoticmap.h"

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
    
        fclose(textfile);
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

    //main part for steganography

    int *bitStream;
    int table[256];
    int bitCount = Compression(text,textSize,&bitStream,table);
    free(text);

    if(bitCount<0){
        printf("Error: Compression failed \n");
        free(image);
        return 1;
    }

    int *chaosSeq=(int *)malloc(imageSize*sizeof(int));
    double seed= 0.5;
    double r= 3.9;
    chaoticmap(chaosSeq,imageSize,seed,r);

    if(embedLSB(image,imageSize,chaosSeq,bitStream,bitCount,table)!=0){
        printf("Error: Embedding failed \n");
        free(image);
        free(bitStream);
        free(chaosSeq);
        return 1;
    }

    char outputFile[250];
    printf("Enter output Stego image file name:");
    scanf("%s",outputFile);

    if(writeBMP(outputFile,image,imageSize,&header,&info)!=0){
        printf("Error: Failed to write stego image\n");
    }
    else{
        printf("Steganography successful! Saved as %s",outputFile);
    }

    free(image);
    free(bitStream);
    free(chaosSeq);
    }

    else if(choice==2){
        char stegoFile[250];

        printf("Enter the Stego BMP file name:");
        scanf("%s",stegoFile);

        BMPFILEHEADER header;
        BMPIMAGEHEADER infoHeader;
        unsigned char* image = NULL;
        int imageSize=0;

        if(readBMP(stegoFile,&image,&imageSize,&header,&infoHeader)!=0){
            printf("Error: Failed to read Stego BMP File \n");
            return 1;
        }

        int *chaosSeq=(int*)malloc(imageSize*sizeof(int));
        if(!chaosSeq){
            printf("Error: Memory Allocation failed \n");
            free(image);
            return 1;
        }

        double seed=0.5;
        double r=3.9;

        chaoticmap(chaosSeq,imageSize,seed,r);

        int bitCount;
        int table[256];
        int *bitStream=(int*)malloc(imageSize*sizeof(int));

        if(!bitStream){
            printf("Error: Memory Allocation failed \n");
            free(image);
            free(chaosSeq);
            return 1;
        }

        if(extractLSB(image,imageSize,chaosSeq,bitStream,&bitCount,table)!=0){
            printf("Error: Extraction Failed \n");
            free(image);
            free(chaosSeq);
            free(bitStream);
            return 1;
        }

        unsigned char* decoded;
        int size;

        if(Decompression(bitStream,bitCount,table,&decoded,&size)!=0){
            printf("Error: Decompression Failed \n");
            free(image);
            free(chaosSeq);
            free(bitStream);
            return 1;
        }

        printf("==========DECODED MESSAGE==============\n");
        printf("%s",decoded);

        free(image);
        free(chaosSeq);
        free(bitStream);
        free(decoded);
    }

    else{
        printf("Invalid choice \n");
    }
    return 0;
}