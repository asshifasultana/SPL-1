
#include<stdio.h>
#include<stdlib.h>
#include"bmp.h"
#include"lsb.h"
#include"huffman.h"
#include"chaoticmap.h"
#include"hamming.h"
#include"network.h"
#include <string.h>
#include <conio.h>


void embeddingAnalysis(PixelChange *change, int changeCount, int totalPixels)
{
    printf("\n========EMBEDDING ANALYSIS=========\n");

    printf("Total Pixels: %d\n",totalPixels);
    printf("Modified Pixels: %d\n", changeCount);

    double percent= (double)changeCount/totalPixels*100;
    printf("Modified Percentage: %.2f%%\n",percent);

    int maxDiff=0;
    double totalDiff=0;

    for(int i=0; i<changeCount; i++)
    {
        int diff= abs(change[i].after- change[i].before);
        totalDiff+= diff;
        if(diff > maxDiff){
            maxDiff=diff;
        }
    }

    double avg= (changeCount>0)? totalDiff/changeCount : 0;

    printf("Average change: %.2f\n",avg);
    printf("Max Change: %d\n",maxDiff);

    printf("======================================\n");
}




void printChanges(PixelChange *change, int changeCount)
{
    printf("\n Sample Modified Pixels: \n");
    printf("Index \t Before \t After \t Bit \n");

    int idx= (changeCount<20) ? changeCount: 20;

    for(int i=0;i<idx;i++)
    {
        printf(" %d \t %d \t %d \t %d \n",
        change[i].index,
        change[i].before,
        change[i].after,
        change[i].bit);
    }
}




void saveCSV(PixelChange *change, int changeCount)
{
    FILE *fp= fopen("pixel_data.csv","w");

    if(fp==NULL)
    {
        printf("Error opening file\n");
        return;
    }

    fprintf(fp, "Index,Before,After,Bit,Diff\n");

    for(int i=0;i<changeCount;i++)
    {
        fprintf(fp, "%d,%d,%d,%d,%d\n",
        change[i].index,
        change[i].before,
        change[i].after,
        change[i].bit,
        change[i].after - change[i].before);
    }

    fclose(fp);
    printf("CSV file saved: pixel_data.csv\n");

}



int login() {
    char password[20];
    char correctPassword[] = "dite parbo na";
    int i = 0;
    char ch;

    printf("Enter Password: ");

   while ((ch = getch()) != 13) {   // 13 = Enter

    if (ch == 8 || ch == 127) {  // Backspace
        if (i > 0) {
            i--;
            password[i] = '\0';

            printf("\b \b"); // removes last *
        }
    }
        else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';

    if (strcmp(password, correctPassword) == 0) {
        printf("\n==================================================\n");
        printf("              AUTHENTICATION SUCCESS             \n");
        printf("==================================================\n");
        printf("  Status : Access Granted                        \n");
        printf("  Welcome : System Unlocked                      \n");
        printf("==================================================\n\n");
        return 1;
    } else {
        printf("\n==================================================\n");
        printf("              AUTHENTICATION FAILED             \n");
        printf("==================================================\n");
        printf("  Status : Access Denied                         \n");
        printf("  Reason : Invalid credentials                   \n");
        printf("==================================================\n\n");
        return 0;
    }
}


//main function

int main(){

    int choice;

    //printf("=========Image Steganography========= \n");


    printf("========================================================================================\n");
    printf("||                                                                                    ||\n");
    printf("||   ####  #####  #####  ####   ###   #####  #   #   ####  ###   ###     #      #     ||\n");
    printf("||  #        #    #     #      #   #  #      #   #  #       #   #   #    #  #   #     ||\n");
    printf("||   ###     #    ###   #  ##  #   #  ###    #   #   ###    #   #   #    #    # #     ||\n");
    printf("||      #    #    #     #   #  #   #  #      #   #      #   #   #   #    #     ##     ||\n");
    printf("||  ####     #    #####  ####   ###   #       ###   ####   ###   ###     #      #     ||\n");
    printf("||                                                                                    ||\n");
    printf("|| ---------------------------------------------------------------------------------- ||\n");
    printf("||                         >>   STEGOFUSION v1.0   <<                                 ||\n");
    printf("||                      Hybrid Image Steganography System                             ||\n");
    printf("|| ---------------------------------------------------------------------------------- ||\n");
    printf("||                                                                                    ||\n");
    printf("========================================================================================\n");


    //password to login
     while(!login()){
        printf("Try again...\n\n");
    }
    // printf("1.Embed text into image\n");
    // printf("2.Extract text from image\n");
    // printf("Enter your choice:");
    

    while(1){
    printf("\n");
    printf("============================================================\n");
    printf("                 MAIN MENU                      \n");
    printf("============================================================\n");
    printf("   1. [ EMBED ]   Hide secret text inside image\n");
    printf("   2. [ EXTRACT ] Retrieve hidden message\n");
    printf("   3. [ EXIT ]     Close application\n\n");

    printf("------------------------------------------------------------\n");
    printf(">>> Enter your choice: ");
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
        //return 1;
        continue;
    }


    char textFile[250];
    printf("Enter the secret text message file name:");
    scanf("%s",textFile);


    FILE *textfile = fopen(textFile,"rb");
    if(!textfile){
        printf("Error: Failed to read text file\n");
        //return 1;
        continue;
    }


    fseek(textfile, 0, SEEK_END);
    long textSize = ftell(textfile);
    rewind(textfile);


    int requiredBits = (textSize * 8) + 32;

    if (requiredBits > imageSize) {
        printf("Error: Image is too small to hold this message.\n");
    
        fclose(textfile);
        free(image);
        //return 1;
        continue;
}


    unsigned char *text = (unsigned char *)malloc(textSize+1);
    if(!text){
        printf("Error: Memory Allocation failed\n");
        fclose(textfile);
        free(image);
        //return 1;
        continue;
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
        //return 1;
        continue;
    }

    //hamming for error detection and correction
    int *hammingStream;
    int hammingBitCount= hammingEncode(bitStream,bitCount,&hammingStream);
    free(bitStream);


    if(hammingBitCount>imageSize){
        printf("Error: Image is too small!");
        free(image);
        free(hammingStream);
        //return -1;
        continue;
    }


    if(hammingBitCount<0){
        printf("Error: Hamming Encoding failed!\n");
        //return -1;
        continue;
    }

    //chaos mapping for randomization
    int *chaosSeq=(int *)malloc(imageSize*sizeof(int));
    double seed= 0.5;
    double r= 3.9;
    chaoticmap(chaosSeq,imageSize,seed,r);

    //for pixel change data
    PixelChange *change= (PixelChange*)malloc(imageSize*sizeof(PixelChange));
    int changeCount=0;


    if(!change)
    {
        printf("Error:Memory Allocation failed\n");
        free(image);
        free(chaosSeq);
        free(hammingStream);
        //return 1;
        continue;
    }


    //lsb embedding function
    if(embedLSB(image,imageSize,chaosSeq,hammingStream,hammingBitCount,table, change, &changeCount)!=0){
        printf("Error: Embedding failed \n");
        free(image);
       // free(bitStream);
        free(chaosSeq);
        free(change);
        //return 1;
        continue;
    }

    char outputFile[250];
    printf("Enter output Stego image file name:");
    scanf("%s",outputFile);

    if(writeBMP(outputFile,image,imageSize,&header,&info)!=0)
    {
        printf("Error: Failed to write stego image\n");
    }
    else
    {
           int choice;

            printf("\n================================================================\n");
            printf("                   STEGO IMAGE READY                           \n");
            printf("================================================================\n");
            printf(" File saved locally as: %s\n", outputFile);
            printf("================================================================\n");

            embeddingAnalysis(change, changeCount, imageSize);
            printChanges(change, changeCount);
            saveCSV(change, changeCount);

            printf("Do you want to send this image?\n");
            printf(" 1. Yes\n");
            printf(" 2. No (Exit)\n");
            printf("Enter choice: ");

            scanf("%d", &choice);

            if(choice == 1){
                sendToUser(outputFile);
            }
            // else{
            //     printf("\n[+] Exiting...\n");
            // } 
    }

    free(image);
    //free(bitStream);
    free(chaosSeq);
    free(change);
    }



    //extracting
    //totally opposite of the embedding process

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
            //return 1;
            continue;
        }

        int *chaosSeq=(int*)malloc(imageSize*sizeof(int));
        if(!chaosSeq){
            printf("Error: Memory Allocation failed \n");
            free(image);
            //return 1;
            continue;
        }

        double seed=0.5;
        double r=3.9;

        chaoticmap(chaosSeq,imageSize,seed,r);

        int hammingBitCount;
        int table[256];
        int *hammingBitStream=(int*)malloc(imageSize*sizeof(int));

        if(!hammingBitStream){
            printf("Error: Memory Allocation failed \n");
            free(image);
            free(chaosSeq);
            //return 1;
            continue;
        }

        if(extractLSB(image,imageSize,chaosSeq,hammingBitStream,&hammingBitCount,table)!=0){
            printf("Error: Extraction Failed \n");
            free(image);
            free(chaosSeq);
            free(hammingBitStream);
            //return 1;
            continue;
        }

        int *bitStream;
        int bitCount;

        bitCount = hammingDecode(hammingBitStream,hammingBitCount,&bitStream);
        free(hammingBitStream);

        if(bitCount<0){
            printf("Error: Hamming Decode failed!\n");
            free(image);
            free(chaosSeq);
            //return -1;
            continue;
        }

        unsigned char* decoded;
        int size;

        if(Decompression(bitStream,bitCount,table,&decoded,&size)!=0){
            printf("Error: Decompression Failed \n");
            free(image);
            free(chaosSeq);
            free(bitStream);
            //return 1;
            continue;
        }

    
        printf("==========DECODED MESSAGE==============\n");
        printf("%s",decoded);

        free(image);
        free(chaosSeq);
        free(bitStream);
        free(decoded);
    }


    else if(choice==3){
        printf("\n==================================================\n");
        printf("                   EXIT                          \n");
        printf("==================================================\n");
        printf("  Thank you for using STEGOFUSION               \n");
        printf("==================================================\n\n");
        break;
    }
    else{
        printf("Invalid choice \n");
    }
}
    return 0;
}