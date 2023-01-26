/*Red Black Binary Search Tree  Interface*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RBKey.h"
#include "Queue.h"

void PrintTree(void);
enum { Black, Red };
enum { Left, Right };
/*Case for key entering right subtree*/
/*Change to > for excluding same keys insertion*/
#define RightCase(A,B)  ((A) >= (B) ? (1) : (0))

typedef int Colour;

/** Data struct details **/
typedef struct RedBlackTree RBnode;
struct RedBlackTree{
    Colour color;
    Key key;
    RBnode *parent;
    RBnode *right;
    RBnode *left;
};

struct RedBlackTreeInfo{
    RBnode* head;
    RBnode* dummy;   //Extended node - black leaf
    int Init;
    int Key_Counter;
}T;



int IsEmpty(void){return T.Key_Counter == 0;}

int NumberOfKeys(void){return T.Key_Counter;}

int BlackDepth(void){
    int blackNodes = 0;RBnode *current;
    if(T.Key_Counter){
        current = T.head;
        while(current != T.dummy){
            if(current->color == Black)blackNodes++;
            current = current->left;
        }
    }
    return blackNodes;
}

void InitTree(int max){
    if(T.Init)//Global variables are set to 0 by default
        return;
    
    T.Init = 1;
    T.dummy = malloc(sizeof(RBnode));
    T.dummy->color = Black;
    T.dummy->key = -1;
    T.dummy->parent = T.dummy->right = T.dummy->left = NULL;
    T.head = T.dummy;
    if(T.dummy == NULL){
        printf("malloc error at tree initialization!\n");
        exit(0);
    }
}


/*New node has Red color by default*/
RBnode *NewNode(Key key){
    RBnode *new = malloc(sizeof(RBnode));
    if(new == NULL){
        printf("malloc error at node creation!\n");
        exit(0);
    }
    new->key = key;
    new->color = Red;
    new->parent = NULL;
    new->right = T.dummy;
    new->left = T.dummy;
    return new;
}


/** Node info **/
int IsHead(RBnode *node)
    { return node->parent == NULL; }


int IsFrontier(RBnode *node)
    { return node->right == T.dummy && node->left == T.dummy; }


int IsRed(RBnode *node)
    { return node->color == Red; }


int IsRight(RBnode *child)
    { return child->parent->right == child; }
    

RBnode *SiblingOf(RBnode *node)
    { if(IsRight(node))return node->parent->left;
      else return node->parent->right; }


RBnode *ParentOf(RBnode *child)
    { return child->parent; }


RBnode *UncleOf(RBnode *node)
    { return SiblingOf(node->parent); }
/*e.g. If parent is right child, his away child is 
       the right child. Same principle if parent is left.*/
RBnode *AwayChild(RBnode *parent)
    { if(IsRight(parent))return parent->right;
      else return parent->left; }
/*e.g. If parent is right child, his near child is 
       the left child. Same principle if parent is left.*/
RBnode *NearChild(RBnode *parent)
    { if(IsRight(parent))return parent->left;
      else return parent->right; }


RBnode *GrandParentOf(RBnode *node)
    { return node->parent->parent; }



/** Modify node color **/
void Recolor(RBnode *node){
    if(node == T.dummy)return;
    if(IsRed(node))node->color = Black;
    else node->color = Red;
}

void SetBlack(RBnode *node)
    { node->color = Black; }


/** Rotations for nodes with parent links **/
void rotateL(RBnode *node) {
    RBnode *temp = node->right;
    temp->parent = node->parent;
        
    node->right = temp->left;

    if(node->right != T.dummy)
        node->right->parent = node;
    temp->left = node;
    node->parent = temp;

    if(temp->parent != NULL){
        if(node == temp->parent->left)
            temp->parent->left = temp;
        else 
            temp->parent->right = temp;
    } 
    else T.head = temp;
}
    
void rotateR(RBnode *node) {
    RBnode *temp = node->left;
    temp->parent = node->parent;
        
    node->left = temp->right;

    if(node->left != T.dummy)
        node->left->parent = node;
    temp->right = node;
    node->parent = temp;

    if(temp->parent != NULL){
        if(node == temp->parent->right)
            temp->parent->right = temp;
        else 
            temp->parent->left = temp;
    } 
    else T.head = temp;
}
/*Selects the rotation needed in order to simplify
  Insert and delete "mirror" cases.*/
void Rotate(RBnode *root, int direction){ 
    if(direction == Left)rotateL(root);
    else  rotateR(root);
}


/*Restore red black properties if needed*/
void RebalanceInsert(RBnode *target){
    RBnode  *uncle, *grandparent;
    RBnode *parent = target->parent;
    
    if(IsHead(target))                    //Case 1: Node is head.   
        { SetBlack(target); return; }        
    
    if(!IsRed(parent))return;             //Case 2: Parent is black, tree is balanced.                   
    uncle = UncleOf(target);                  
    grandparent = GrandParentOf(target);
    if(IsRed(uncle)){                     //Case 3: Uncle is red.
        Recolor(uncle);                        
        Recolor(parent);
        Recolor(grandparent);
        target = grandparent;             //Check grandparent node for unbalances.
    }
    else if(AwayChild(parent) == target){ //Case 4.a Uncle is Black and target is "away" red nephew.
        Rotate(grandparent, !IsRight(target)); 
        Recolor(grandparent);                  
        Recolor(parent);                  //**Mirror cases included by choosing the direction of rotations 
    }                                     //**with the help of functions IsAwayChild(), IsRight(), UncleOf(); 
    else if(AwayChild(parent) != target){ //Case 4.b Uncle is Black and target is "near" red nephew.
        Rotate(parent,!IsRight(target));  //If target is right child rotate parent left else right.
        target = parent;                  //Go to case 4.a
    }                                   
    RebalanceInsert(target);
}

/*Inserts a key and calls RebalanceInsert*/
void InsertKey(Key key){
    int right; RBnode *node, *parent;
    node = T.head;

    if(!T.Init){
        printf("Initialize the tree first!\n");
        return;
    }
    if(T.head == T.dummy){
        T.head = NewNode(key);
        node = T.head;
    }
    else{
        while(node != T.dummy){
            parent = node;
            if(key < node->key){
                node = node->left; right = 0;
            }    
            else if RightCase(key, node->key){
                node = node->right; right = 1;
            }    /*In case we dont want same keys*/
            else{
                printf("\nKey is allready inserted!\n");
                return;
            }
        }
        node = NewNode(key);
        node->parent = parent;
        if(right)parent->right = node;
        else parent->left = node;
    }
    T.Key_Counter++;
    RebalanceInsert(node);
}

/**Functions for RemoveKey**/
/*Returns T.dummy node if key doesn't exist*/
RBnode* FindNode(RBnode* node, Key key){
    if(node == T.dummy)return T.dummy;
    if(key < node->key)return FindNode(node->left, key);
    else if(key == node->key)return node;
    else FindNode(node->right,key);
}

RBnode* min(RBnode* node){
    RBnode *current = node;
    while(current->left != T.dummy)
        current = current->left;
    return current;
}

RBnode* max(RBnode* node){
    RBnode *current = node;
    while(current->right != T.dummy)
        current = current->right;
    return current;
}
/*1st choise is left's subtree max*/
RBnode* Successor(RBnode *node){
    if(node->left != T.dummy)return max(node->left);
    else return min(node->right);
}

void SwapData(RBnode* A, RBnode* B){
    Key bkey = B->key;
    B->key = A->key;
    A->key = bkey;
}

void ReplaceChild (RBnode *parent, RBnode *prev_child, RBnode *new_child){
    if(parent == NULL)T.head = T.dummy;
    else{
        if(IsRight(prev_child))parent->right = new_child;
        else parent->left = new_child;
        if(new_child != T.dummy)new_child->parent = parent;
    }
    free(prev_child);
}

void RebalanceRemove(RBnode* node){
    RBnode *target = node, *parent, 
    *near_nephew, *away_nephew, *sibling;
    while(!IsHead(target) && !IsRed(target)){
        sibling = SiblingOf(target);
        parent = ParentOf(target);
        near_nephew = NearChild(sibling);
        away_nephew = AwayChild(sibling);
        if(!IsRed(parent)){                  /**Parent is black**/    
            if(!IsRed(sibling)){             //Case 1: Sibling, parent and nephews are black                    
                if(!IsRed(away_nephew) && !IsRed(near_nephew)){
                    Recolor(sibling);
                    target = parent;             
                    continue;                //Continue checking from parent
                }
                else if(IsRed(away_nephew)){ //Case 2: Parent and sibling are black, away nephew is red
                    Recolor(away_nephew);
                    Rotate(parent, IsRight(target));
                    return;                  //Terminal case
                }
                else{                        //Case 3: Parent and sibling are black, near nephew is red
                    Recolor(near_nephew);
                    Rotate(sibling, IsRight(sibling));
                    Rotate(parent, IsRight(target));
                    return;                  //Terminal case
                }
            }
            else{                            //Case 4: Parent is black sibling is red
                Recolor(sibling);
                Recolor(parent);
                Rotate(parent, IsRight(target));
                continue;                    //Now parent is red. Go to case 5 or 6
            }
        }
        else{                                /**Parent is red**/
            if(!IsRed(near_nephew)){         //Case 5: Parent is Red near nephew is black
                Rotate(parent, IsRight(target));
                return;                      //Terminal case
            }
            else{                            //Case 6: Parent is Red near nephew is red
                Recolor(parent);
                Rotate(sibling, IsRight(sibling));
                Rotate(parent, !IsRight(sibling));
                return;                      //Terminal case
            }                 
        }
    }
}

void RemoveKey(int key)
{
    RBnode *child ,*replace;
    if(!T.Init){
        printf("Tree is empty!\n");
        return;
    }
    replace = FindNode(T.head, key);
    if (replace == T.dummy){
        printf("\nKey not found! \n");return;
    }
    T.Key_Counter--;
    if (!IsFrontier(replace)) {              //If node to be deleted is internal
        RBnode *temp = Successor(replace);   //__find successor 
        SwapData(replace, temp);             //__swap data between node and successor
        replace = temp;                      //__now node to be deleted is the successor.
    }                                        //Else if node is frontier just delete him.
    if(replace->left != T.dummy)           
        child = replace->left;               //If replace is internal node, save child's position
    else if(replace->right != T.dummy)       //__else child is dummy
        child = replace->right;          
    else child = T.dummy;

    if(IsRed(child))child->color = Black;
    else if (!IsHead(replace) && !IsRed(replace))
        RebalanceRemove(replace);            //If replace is "double black" restore tree properties
    ReplaceChild(ParentOf(replace), replace, child);
    SetBlack(T.head);
}


/*Recursively delete all nodes*/
void DeleteR(RBnode* node){
    if(node == T.dummy)return;
    DeleteR(node->left);
    DeleteR(node->right);
    free(node);
}

void DeleteTree(void){
    if(!T.Init)return;
    DeleteR(T.head);
    free(T.dummy);
    T.dummy = T.head = NULL;
    T.Init = 0;
    T.Key_Counter = 0;
}


/*PrintTree macros - functions*/
#define empty(A) ((A) == (NULL))
/*For every next line array has dashes. So for A (level) > 1
  we need the following macros to calculate the needed location*/
#define dashlvl(A) ((A)*(2))+(1)
#define nodelvl(A) (A)*(2)

struct TreePrint{ int level, loc; RBnode* node; };
/*Travels through a given direction in order to determine the
  number of spaces between nodes.*/
int Count(RBnode* node, int count, int direction){
    const int left = 1;
    if(empty(node))return count;
    /*If this is 1st call*/
    if(direction == left && !count){
        if(!empty(node->left)){
            node = node->left;
            count += 5;
        }
        /*Default distance between nodes*/
        else return 5;
    }
    else if(!direction && !count){
        if(!empty(node->right)){
            node = node->right;
            count += 5;
        }
        else return 5;
    }
    count += 5;
    count = Count(node->left, count, direction);
    count = Count(node->right, count, direction);
    return count;
}

void Dash(int num, char *tree){
    int i = 0;
    if(!num)return;
    tree[i] = '+';
    tree[num] = '+';
    i++;
    while(i < num)tree[i++] = '-';
}

/*Stores the node element in array*/
void Node(RBnode* node, char *tree){
    char color;
    if(node->color == Black)color = 'B';
    else color = 'R';
    /*If node is empty print T.dummy node*/
    if(node->key == -1)
        {sprintf(tree-1,"[]%c",color);tree[2] = ' ';}
    else
        {sprintf(tree-2,"(%03d)%c",node->key,color);tree[4] = ' ';}
}

struct TreePrint *StoreNode(RBnode* node, int level, int loc){
    struct TreePrint *new;
    new = malloc(sizeof(struct TreePrint));
    new->node = node;
    new->loc = loc;
    new->level = level;
    return new; 
}

void DisplayTree(RBnode* head){
    char Tree[30][200];
    const int right = 0, left = 1;
    int i, j, space ,dash, total, location, level = 0;

    struct TreePrint *current;

    /*Initialize tree array*/
    for(i = 0; i < 30; i++)
      for(j = 0; j < 200; j++)
            Tree[i][j] = ' ';
    /*Count root's left child's nodes to determine the spaces.*/
    space = Count(head->left, level, left);
    dash = Count(head->left, level, right);
    total = space + dash;
    if(!total)total = 2;
    EnQueue(StoreNode(head, level, total));

    while(!QEmpty()){
        current = DeQueue();
        if(empty(current))break;
        level = current->level;
        location = current->loc;
        Node(current->node, &Tree[nodelvl(level)][location]);
        if(!empty(current->node->left)){
          dash = Count(current->node->left,0,right);
          Dash(dash, &Tree[dashlvl(level)][location-dash]);
          EnQueue(StoreNode(current->node->left, level+1, location-dash));
        }   
        if(!empty(current->node->right)){
          dash = Count(current->node->right,0,left);
          Dash(dash, &Tree[dashlvl(level)][location]);
          EnQueue(StoreNode(current->node->right, level+1, location+dash));
        }
        free(current);
    }
    for(i = 0; i < 30; i++){
        for(j = 200; j >= 0; j--)
            if(Tree[i][j] != ' '){
                Tree[i][j+1] ='\n';
                Tree[i][j+2] ='\0';
                break;
            }
        if(j == -1){
          Tree[i][j]='\0';
          break;
        }
    }
    printf("\n");
    for(i = 0; i <= nodelvl(level); i++)printf("%s",Tree[i]);
    printf("\n");
}

void InOrder(RBnode *p){
    if(p!=T.dummy){
        InOrder(p->left);
        printf("%03d",p->key);
        printf(":%c -> ",p->color == 0 ? 'B' :'R');
        InOrder(p->right);
    }
}


void PrintTree(void){
    if(!T.Init)
        {printf("\nTree is empty!\n");return;}
    if(T.Key_Counter < 17){
        DisplayTree(T.head);
    }
    else if(T.Key_Counter < 31){
        printf("\n------------------------- Head: (%03d)B", T.head->key);
        printf("-------------------------\n\n");
        printf("\n        *** Left sub tree ***");
                DisplayTree(T.head->left);
        printf("\n        *** Right sub tree ***");
        DisplayTree(T.head->right);
    }
    else if(T.Key_Counter < 100){
        printf("\n**InOrder**\n");
        InOrder(T.head);
        printf("\n\n"); 
    }
}