/*Red Black Binary Search Tree  Interface*/
#include "RBKey.h"
/*Initialize at start or after the use
  of DeleteTree. If tree is initialized it will do nothing*/
void InitTree(void);
/*Inserts a key and maintaining red-black properties*/
/*Currently supports same keys insertion*/
void InsertKey(Key);

/*Removes a key and maintaining red-black properties*/
void RemoveKey(Key);


int IsEmpty(void);


int NumberOfKeys(void);

/*Remove all nodes from tree*/
void DeleteTree(void);
/*Visual representation of tree if nodes are <= 30
  Else prints the tree in order*/
void PrintTree(void);
/*Number of black nodes from leaf to head*/
/*  leaf doesnt count*/
int BlackDepth(void);
