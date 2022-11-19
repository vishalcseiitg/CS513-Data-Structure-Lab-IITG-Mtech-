#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//struture for a treap node 
struct tnode {
int key, priority;
int left, right;
int parent; //optional
};

typedef struct tnode tnode;

//structure for entire treap consistiong with child
struct Treap {
int root;
int freeList;
tnode *tnodeArr;
int numElements;
};

typedef struct Treap Treap;

int max(int a,int b){
    if(a>=b)
       return(a);
    else
       return(b);     
}  

//[5 points]

//This function is used to initialize the treap that we are taking into account
void init(Treap *T, int numElements)
{
    T->tnodeArr = (tnode *)malloc(numElements*sizeof(tnode));
    if (T->tnodeArr == NULL)
        exit(EXIT_FAILURE);

    T->numElements = numElements;

    for(int i=0;i<numElements;++i){
       T->tnodeArr[i].left  =-1;
       T->tnodeArr[i].right =-1;
       T->tnodeArr[i].parent=-1;       
    }

    T->root = -1;
    T->freeList = 0;
}

//The function is used to rotate the treap leftwards

int leftRotate(Treap *T,int parent){
    int child=T->tnodeArr[parent].right;
    int Gp   =T->tnodeArr[parent].parent;

    if(Gp!=-1){
       if(parent==T->tnodeArr[Gp].left)
          T->tnodeArr[Gp].left=child;
       else
          T->tnodeArr[Gp].right=child;       
    } 

    T->tnodeArr[parent].right=T->tnodeArr[child].left;
    T->tnodeArr[child].left=parent;
    T->tnodeArr[child].parent=Gp;     

    return(child);   
}

//The function is used for right rotation of the treap

int rightRotate(Treap *T,int parent){
    int child=T->tnodeArr[parent].right;
    int Gp   =T->tnodeArr[parent].parent;
    
    if(Gp!=-1){
       if(parent==T->tnodeArr[Gp].left)
          T->tnodeArr[Gp].left=child;
       else
          T->tnodeArr[Gp].right=child;       
    } 

    T->tnodeArr[parent].left=T->tnodeArr[child].right;
    T->tnodeArr[child].right=parent;
    T->tnodeArr[child].parent=Gp;     

    return(child);
}

//This function is used for regaining the priority of each node (maintainig the max priority) 
void rectify(Treap *T,int child){
     while(T->tnodeArr[child].parent!=-1 && T->tnodeArr[T->tnodeArr[child].parent].priority < T->tnodeArr[child].priority){
           if(child==T->tnodeArr[T->tnodeArr[child].parent].left)
              child=rightRotate(T,T->tnodeArr[child].parent);
           else
              child=leftRotate(T,T->tnodeArr[child].parent);       
     }     

     //printf("parent in the while loop=%d\n",T->tnodeArr[child].parent);
}

// [20 points] -- no duplicate keys

//This function is used to insert keys which are not duplicate in the Treap
void insertTreap(Treap *T, int key, int priority){

    if (T->root == -1){
        T->root = 0;
        T->tnodeArr[0].key     = key;
        T->tnodeArr[0].left    = -1;
        T->tnodeArr[0].right   = -1;
        T->tnodeArr[0].parent  = -1;
        T->tnodeArr[0].priority=priority;
        T->freeList++;
        return;
    }
    
    int tr=0;
    int back=0;

    while(tr!=-1){
          back=tr; 
          if(key>T->tnodeArr[tr].key)
             tr=T->tnodeArr[tr].right;
          else
             tr=T->tnodeArr[tr].left;       
    } 

    if(key>T->tnodeArr[back].key)
       T->tnodeArr[back].right=T->freeList;
    else                 
       T->tnodeArr[back].left=T->freeList;

    T->tnodeArr[T->freeList].key=key;
    T->tnodeArr[T->freeList].parent=back;
    T->tnodeArr[T->freeList].left=-1;
    T->tnodeArr[T->freeList].right=-1;
    T->tnodeArr[T->freeList].priority=priority;

    int index=T->freeList; 
    T->freeList++;        
    rectify(T,index);   
}

void display(Treap *T){
     int num=T->numElements;
     for(int i=0;i<num;++i)
         printf("Index= %d Key=%d  Left child =%d Right child=%d\n",i,T->tnodeArr[i].key,T->tnodeArr[i].left,T->tnodeArr[i].right);  
}

// [20 points]
void deleteTreap(Treap *T, int key);
// [5 points]

//This function is used to calculate the height of the treap

int heightHelper(Treap *T, int ind){
    //  if ind is -1 that means it is NULL node, we return back 0
    if(ind == -1){
        return 0;
    }
    //  if node at ind has left and right indexes as -1, that means it is a leaf node, we return 1 as height
    if(T->tnodeArr[ind].left == -1 && T->tnodeArr[ind].right == -1){
        return 1;
    }
    // recursively call on left and right subtree for height and store it in leftHeight and rightHeight variables
    int leftHeight = heightHelper(T,T->tnodeArr[ind].left);
    int rightHeight = heightHelper(T,T->tnodeArr[ind].right);

    // return maximum height from left and right subtree + 1, (+1 is done as we include the height of parent node)
    return 1 + max(leftHeight, rightHeight);
}
int height(Treap *T){
    return heightHelper(T,0);
}
// [10 points]
void displayTreap(Treap *T, char *fileName); //produce dot file
//void leftRotate(Treap *T, int node);
//void rightRotate(Treap *T, int node);

int main()
{
    srand(time(NULL));

    Treap T;

    init(&T,7);
    
    //display(&T); 

   // insertTreap(&T,7,12);
    //insertTreap(&T,8,6);
    //insertTreap(&T,21,11); 

    insertTreap(&T,7,12);
    insertTreap(&T,8,6);
    insertTreap(&T,15,9);
    insertTreap(&T,21,11);
    insertTreap(&T,27,13);
    insertTreap(&T,30,8);
    insertTreap(&T,32,14);
    display(&T);

    //print(&T,"Graph.dot");
    printf("Height =%d ",height(&T));  

    // Generate a table of tree height for different number of nodes e.g., 100, 1000, 5000, 10000, 50000
}


