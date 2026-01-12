#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"huffman.h"



Minheap* createHeap(int capacity){              //capacity= how many elements can be stored
    Minheap* heap = (Minheap*)malloc(sizeof(Minheap));
    heap->node = (Node**)malloc(capacity*sizeof(Node*));
    heap->size=0;                              //heap is empty, no node inserted
    return heap;
}

void swapNodes(Node** a, Node** b){
    Node* temp=*a;
    *a=*b;
    *b=temp;
}

void heapify(Minheap* heap, int index){
    int smallest=index;                   //assuming root node of a subtree is the smallest
    int left= 2*index+1;
    int right= 2*index+2;

    if(left<heap->size && heap->node[left]->freq < heap->node[smallest]->freq){
        smallest=left;
    }
    if(right<heap->size && heap->node[right]->freq < heap->node[smallest]->freq){
        smallest=right;
    }

    if(smallest != index){
        swapNodes(&heap->node[smallest],&heap->node[index]);  //if root not smallest, swap and heapify
        heapify(heap,smallest);
    }
}

Node* extractMin(Minheap* heap){
    Node* temp = heap->node[0];
    heap->node[0]=heap->node[heap->size-1]; //inserts the last element of the tree
    heap->size--;
    heapify(heap,0);
    return temp;
}

void insertHeap(Minheap* heap, Node* node){
    heap->size++;                                   //adding element increases the size
    int i= heap->size-1;                            //new node index
    heap->node[i]=node;
    while(i && heap->node[i]->freq < heap->node[(i-1)/2]->freq){        //parent er frequency beshi hole swap
        swapNodes(&heap->node[i],&heap->node[(i-1)/2]);
        i=(i-1)/2;
    }
}


    Node* createNode(unsigned char ch,int freq){
        Node* node= (Node*)malloc(sizeof(Node));
        node->ch=ch;
        node->freq=freq;
        node->left=node->right=NULL;
        return node;
    }

Node* huffmanTree(int freq[MAX_CHAR]){
    Minheap* heap= createHeap(MAX_CHAR);                        //heap for all 256 characters, then insert node
    for(int i=0;i<MAX_CHAR;i++){
        if(freq[i]>0){
            insertHeap(heap,createNode((unsigned char)i,freq[i]));
        }
    }

    if(heap->size==1){
        Node* single = extractMin(heap);
        Node* root = createNode(0,single->freq);
        root->left=single;
        root->right=NULL;
        free(heap->node);
        free(heap);
        return root;
    }

    while(heap->size>1){
        Node* left=extractMin(heap);
        Node* right= extractMin(heap);
        Node* parent= createNode(0,left->freq+right->freq);
        parent->left=left;
        parent->right=right;
        insertHeap(heap,parent);
    }

    Node* root = extractMin(heap);
    free(heap->node);
    free(heap);
    return root;
}

void huffmanCode(Node* root, char *code, int depth,char codes[MAX_CHAR][MAX_CHAR]){
    if(!root)
        return;
    if(!root->left && !root->right){
        code[depth]='\0';
        strcpy(codes[root->ch],code);
        return;
    }

    code[depth]='0';
    huffmanCode(root->left , code, depth+1, codes);
    code[depth]='1';
    huffmanCode(root->right, code, depth+1, codes);
}

void freeTree(Node* root){
    if(!root)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int Compression(unsigned char *text, int textSize, int **bitStream, int table[256]){
    
    if(textSize<=0 || !text){
        printf("Error: Empty text file \n");
        return -1;
    }

    for(int i=0;i<256;i++){
        table[i]=0;
    }
    for(int i=0;i<textSize;i++){
        table[text[i]]++;
    }

    Node* root= huffmanTree(table);

    char codes[MAX_CHAR][MAX_CHAR];
    char code[256];

    for(int i=0;i<MAX_CHAR;i++){
        codes[i][0]='\0';
    }
    huffmanCode(root,code,0,codes);

    int totalBits=0;
    for(int i=0;i<textSize;i++){
        totalBits+=strlen(codes[text[i]]);
    }

    *bitStream = (int*)malloc(totalBits * sizeof(int));
    if(!(*bitStream)){
        printf("Error: Memory Allocation Failed \n");
        freeTree(root);
        return -1;
    }

    int index=0;
    for(int i=0;i<textSize;i++){
        for(int j=0;codes[text[i]][j];j++){
            (*bitStream)[index++] = codes[text[i]][j]-'0';
        }
    }

    freeTree(root);
    return totalBits;
    
}

int Decompression(int *bitStream, int bitCount, int table[256],unsigned char **output, int *outputSize){
    Node* root = huffmanTree(table);
    
    if(!root){
        printf("Error: Failed to Decompress \n");
        return -1;
    }

    int size=0;
    for(int i=0;i<256;i++){
        size+=table[i];
    }

    if(!root->left && !root->right){                             //for single character
        *output = (unsigned char*)malloc(size);
        if(!(*output)){
            printf("Error: Memory Allocation Failed \n");
            freeTree(root);
            return -1;
        }

        for(int i=0;i<size;i++){
            (*output)[i]=root->ch;
        }

        *outputSize=size;
        freeTree(root);
        return 0;
    }

    *output= (unsigned char*)malloc(size);
    if(!(*output)){
        printf("Error: Memory Allocation failed \n");
        freeTree(root);
        return -1;
    }

    int index=0;
    Node* curr=root;

    for(int i=0;i<bitCount;i++){
        if(bitStream[i]==0){
            if(!curr->left){
                printf("Error: Invalid bitstream \n");
                free(*output);
                freeTree(root);
                return -1;
            }
            curr=curr->left;
        }

        else{
            if(!curr->right){
                printf("Error: Invalid bitstream");
                free(*output);
                freeTree(root);
                return -1;
            }
            curr=curr->right;
        }

        if(!curr->left && !curr->right){
            (*output)[index++]=curr->ch;
            curr=root;
        }

    }

    if(curr!=root){
            printf("Bitstream ended mid-symbol\n");
        }

        if(index!=size){
            printf("Decoded size mismatch \n");
        }


    *outputSize=index;
    freeTree(root);
    return 0;
}