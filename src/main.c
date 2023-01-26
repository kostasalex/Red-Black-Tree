#include <stdio.h>
#include <stdlib.h>
#include "RedBlackBST.h"
#include "RBKey.h"

#define MaxKeys 15
/*Creates a tree with random key values from 0 - 99*/
int  InsertRandomKeys(int number_of_keys){
    int i;
    for(i = 1; i <= MaxKeys; i++)
        InsertKey(randKey(100));
}

void PrintSelection(void){
    static int call = 0;
    if(!call){
        printf("\n**Red-Black tree implementation**\n");
        call++;
    }
    printf("1. Insert a key.\n");
    printf("2. Remove a key.\n");
    printf("3. Create a tree with %d random keys.\n",MaxKeys);
    printf("4. Delete Tree\n");
    printf("5. Exit\n");
    printf("Select: ");
}

int main(){
    int select;
    Key key;
	while(1){   
        PrintSelection();
        scanf(" %d",&select);
        while(getchar()!='\n');  //clearing the buffer
        switch(select){
            case 1:
                if(IsEmpty())InitTree();
                printf("note: visual representation supports 30 keys max\n");
                printf("key: ");
                scanf(" %d",&key);
                if(key < 0 || key > 999){
                    printf("\nSelect a key between 0 - 999!\n\n");
                    continue;
                }
                InsertKey(key);
                printf("\n");
                break;
            case 2:
                if(IsEmpty()){
                    printf("\nTree is empty!\n\n");
                    continue;
                }
                printf("key: ");
                scanf(" %d",&key);
                RemoveKey(key);
                printf("\n");
                break;
            case 3:
                DeleteTree();
                InitTree();
                InsertRandomKeys(MaxKeys);
                break;
            case 4:
                DeleteTree();
                printf("\n");
                break;
            case 5:
                DeleteTree();
                return 0;
            default:
                printf("\nWrong selection!\n\n");
                continue;
        }
        if(NumberOfKeys()){
            PrintTree();
            printf("\nNumber of keys inserted: %d\n",NumberOfKeys());
            printf("Current black depth: %d\n\n",BlackDepth());
        }
    }   
}