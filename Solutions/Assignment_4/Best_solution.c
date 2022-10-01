#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>

struct rightThreadedBstNode{
       int key;
       int size;
       bool rightThread;
       struct rightThreadedBstNode *leftChild;
       struct rightThreadedBstNode *rightChild;
};

typedef struct rightThreadedBstNode rTBTNode;
typedef struct rightThreadedBstNode *rTBTNodePtr;
//node structure of Stack containing bst node
struct node 
{
    rTBTNodePtr bnode;
    struct node *next;
};
//Structure of Stack containing top pointer
struct stack 
{
    struct node *top;
};
typedef struct stack S;
typedef struct node node;
//Function to create empty stack
int makeStack(S **s)
{
    *s = (S *)malloc(sizeof(S));
    if(*s == NULL)
        return 0;
    (*s)->top = NULL;
    return 1;
}
//function to create a stack node
int makeStackNode(node **head,rTBTNodePtr bnode)
{
    *head = (node *)malloc(sizeof(node));
    if(*head == NULL)return 0; //Failure of node creation
    (*head)->bnode = bnode;
    (*head)->next = NULL;
    return 1; //Success
}
//Function to check stack is empty
int isEmpty(S *s)
{
    if(s->top == NULL)
        return 1; /*Stack is empty */
    return 0; /*Stack is not empty */
}
//Function to push a bst node into the stack
int push(S *s,rTBTNodePtr bnode)
{
    node *temp;
    if(makeStackNode(&temp,bnode)==0)return 0; //Failure to push 
    if(s->top == NULL)
    {
        s->top = temp;
        return 1;
    }
    temp->next = s->top;
    s->top = temp;
    return 1; //Succesfull push operation
}
//Function to pop a bst node from the stack
int pop(S *s)
{

    if(s->top == NULL) //Pop failure on empty stack
        return 0;
    
    node *temp = s->top;
    s->top = s->top->next;
    free(temp);
    temp = NULL;
    return 1; //Pop succesfull
}
//Function to find top most element in stack
int peek(S *s,rTBTNodePtr *temp)
{
    /* temp is a pointer to a variable which stores  the top bst node reference when stack is not empty*/
    if(s->top == NULL) 
        return 0; //peek failure on empty stack
    *temp = s->top->bnode; 
    return 1; //Succesfull peek operation
}
//Function to print Stack
void printStack(S *s)
{
    if(s==NULL ||isEmpty(s))
        return; //Stack is Empty or not created
    node *i = s->top;
    while(i != NULL)
    {
        printf("%d\n",i->bnode->key);
        i = i->next;
    }
}

void createRightThreadedBST(rTBTNodePtr *root)
{
     *root = NULL;
}

/* Returns NULL in case of allocation faliure*/
rTBTNodePtr getRightThreadedTreeNode()
{
     rTBTNodePtr p;
     p = (rTBTNodePtr)malloc(sizeof(rTBTNode));
     if (p != NULL)
     {
           p->leftChild = NULL;
           p->rightChild = NULL;
     }
     return p;
}
rTBTNodePtr rtbstSearch_(rTBTNodePtr root,rTBTNodePtr *parent,S *s,int key)
{
    /* Arguments
        *parent stores the address of parent node pointing to the probed node
        *s Stack to keep track of the ancestors 
    */
    if(root == NULL)
        return NULL;
    while(root != NULL)
    {
        if(root->key == key)
            return root;
        else if(root->key > key){
            if(s != NULL) push(s,root);
            if(parent != NULL) *(parent)= root;
            root = root->leftChild; //Search in left Subtree
        }
        else {
            if(s != NULL) push(s,root);
            if(parent != NULL) *(parent)= root;
            if(!root->rightThread)
                root = root->rightChild;
            else
                return NULL;
        }
    }
    return NULL;
}
//Function to search for a key and returns address of the node which is found else returns NULL
rTBTNodePtr rtbstSearch(rTBTNodePtr *root,int data)
{
    return rtbstSearch_(*root,NULL,NULL,data);
}
/* Function returns -1 if duplicate exists; otherwise return 0*/
int rtbstInsert(rTBTNodePtr *root, int data)
{
    rTBTNodePtr p, currPtr, currPtrParent;
    /* Insert into an empty tree */
    if (*root == NULL)
    {
        p = getRightThreadedTreeNode();
        if (p == NULL)
           return -2; /* Allocation faliure*/
        p->key = data;
        p->rightThread = false;
        p->size = 1;
        *root = p;
        return 0;
    }
    //Stack to keep track of the ancestors of the inserted node to update there's size
    S *s = NULL;
    if(makeStack(&s) == 0) return -2; /*Not able to keep track of ancestors */
    currPtr = *root;
    currPtrParent = NULL;
    while (currPtr != NULL)
    {
        if (currPtr->key == data)
        {
            while(!isEmpty(s))
                pop(s);
            return -1; /*Duplicate exists*/
        }
            
        else if (currPtr->key > data)
        {
            currPtrParent = currPtr;
            push(s,currPtrParent);/*pushing the parent of current in stack*/
            currPtr = currPtr->leftChild;
        }
        else if(!currPtr->rightThread)
        {
            currPtrParent = currPtr;
            push(s,currPtrParent);/*pushing the parent of current in stack*/
            currPtr = currPtr->rightChild;
        }
        else 
        {
            currPtrParent = currPtr;
            push(s,currPtrParent);/*pushing the parent of current in stack*/
            break;
        }
            
    }
    p = getRightThreadedTreeNode();
    if (p == NULL)
       return -2; /* Allocation faliure*/
    p->key = data;
    p->size = 1;
    if (currPtrParent->key > data)
    {
        currPtrParent->leftChild = p;
        p->rightChild = currPtrParent;
        p->rightThread = true;
    }
    else if(!currPtrParent->rightThread)
       currPtrParent->rightChild = p;
    else{
        p->rightChild = currPtrParent->rightChild;
        p->rightThread = true;
        currPtrParent->rightChild = p;
        currPtrParent->rightThread = false;
    }
    /*Updating ancestors of the inserted node */
    while(!isEmpty(s))
    {
        rTBTNodePtr temp;
        peek(s,&temp);
        temp->size += 1;
        pop(s);
    }
    return 0;
}
//Function returns 1 if key is present to delete else returns -1 if key is not present or bst is empty
int rtbstDelete(rTBTNodePtr *root, int key)
{
    if(*root == NULL)
        return -1;
    rTBTNodePtr parent,curr;
    parent=curr=NULL;
    //Stack to keep track of ancestors of the probed node
    S *s = NULL;
    makeStack(&s);
    curr = rtbstSearch_(*root,&parent,s,key);
    if(curr == NULL)
    {
        while(!isEmpty(s))
            pop(s);
        return -1; //Element with key not exists
    }
    if(curr->leftChild == NULL) //Node to delete doesn't has a left child
    {
        if(parent == NULL) //Node to delete is the current root
        {
            *root = curr->rightChild;
            free(curr);
        }
        else
        {
            if(parent->leftChild == curr)
            {
                if(!curr->rightThread)
                    parent->leftChild = curr->rightChild;
                else
                    parent->leftChild = NULL;
                free(curr);
                
            }
            else {
                if(!curr->rightThread)
                    parent->rightChild = curr->rightChild;
                else
                    {
                        parent->rightThread = true;
                        parent->rightChild = curr->rightChild;
                    }
                    
                free(curr);
            }
        }
    }
    else if(curr->rightThread || curr->rightChild == NULL) //Node to delete doesn't has a right child
    {
        if(parent == NULL)  //Node to delete is the current root
        {
            *root = curr->leftChild;
            if(curr->leftChild->rightThread)
            {
                curr->leftChild->rightThread = false;
                curr->leftChild->rightChild =  NULL;
            }  
            free(curr);
        }
        else
        {
            if(parent->leftChild == curr)
            {
                parent->leftChild = curr->leftChild;
                if(curr->leftChild != NULL && curr->leftChild->rightThread) curr->leftChild->rightChild =curr->rightChild;
                else if(curr->leftChild != NULL) 
                    {
                        rTBTNodePtr temp = curr->leftChild;
                        while(!temp->rightThread)
                            temp = temp->rightChild;
                        if(curr->rightChild == NULL)
                            temp->rightThread = false;
                        temp->rightChild = curr->rightChild;
                    }
                free(curr);
            
            }
            else
            {
                parent->rightChild = curr->leftChild;
                if(curr->leftChild != NULL && curr->leftChild->rightThread) curr->leftChild->rightChild =curr->rightChild;
                    else if(curr->leftChild != NULL) 
                    {
                        rTBTNodePtr temp = curr->leftChild;
                        while(!temp->rightThread)
                            temp = temp->rightChild;
                        if(curr->rightChild == NULL)
                            temp->rightThread = false;
                        temp->rightChild = curr->rightChild;
                    }
                free(curr);
            }
        }
    }
    else  //Node to delete have both children
    {
        rTBTNodePtr succParent = curr; /*curr is the probed node's address*/

		// Find inorder successor to delete and updating the size of it's ancestors
		rTBTNodePtr succ = curr->rightChild;
		while (succ->leftChild != NULL) {
			succParent = succ;
            succParent->size -= 1;
			succ = succ->leftChild;
		}

		/*Delete successor. Since successor
		  *is always left child of its parent
	      *we can safely make successor's 
		  *right child as left of its parent.
		 *If there is no succ, then assign
		 *succ->right to succParent->right*/
		if (succParent != curr)
        {
            if(!succ->rightThread)
                succParent->leftChild = succ->rightChild;
            else
                succParent->leftChild = NULL;
        }
		else
        {
            if(succ->rightThread)
                succParent->rightThread = true;
            succParent->rightChild = succ->rightChild;
        }
                
		// Copy Successor Data to curr
		curr->key = succ->key;
        curr->size -= 1; /*This is important to update the size of the updated current node*/
		// Delete Successor 
		free(succ);
    }
    //Updating the ancestor's size of the probed node 
    while(!isEmpty(s))
    {
        rTBTNodePtr temp;
        peek(s,&temp);
        temp->size -= 1;
        pop(s);
    }
    return 1;   //Successfull delete
}
rTBTNodePtr leftMost(rTBTNodePtr root)
{
    if(root == NULL)
        return NULL;
    while(root->leftChild != NULL)
        root = root->leftChild;
    return root;
}

void inorder(rTBTNodePtr *root,int *List)
{
    if(*(root) == NULL)
        return;
    rTBTNodePtr curr = *(root);
    curr = leftMost(curr);
    int index = 0;
    while(curr != NULL)
    {
        List[index++] = curr->key;
        if(curr->rightThread){
            curr = curr->rightChild;
        }
        else
            curr = leftMost(curr->rightChild);         
    }
}
//returns the address of the node conatining kth Smallest key of BST if present else return NULL
rTBTNodePtr kthElement(rTBTNodePtr *root, int k)
{
    if(*root == NULL)
        return NULL; //Empty Root
    int count = 1; /*Count stores the number of nodes having key less than or equal to it's key*/
    if((*root)->leftChild != NULL)
        count = (*root)->leftChild->size + 1;
    if(count == k) return *root; //Current root is the kth Smallest
    if(count > k) return kthElement((&(*root)->leftChild),k); //kth Smallest is in the left Subtree
    //kth Smallest is in the right subtree it is important to update the k as (k - count)th smallest
    else 
    {
        if(!(*root)->rightThread) /*This is important */
            return kthElement((&(*root)->rightChild),k - count); 
        return NULL;//Not found when root doesn't have any right child
    }
}
//Helper function to display the Right Threaded BST with right threads
void displayTreeHelper(rTBTNodePtr T, FILE *fp)
{
    if (T != NULL)
    {
        fprintf(fp,"node%d[label = \"<f0> | <f1> %d(size = %d) | <f2>\"];",T->key,T->key,T->size);
        if(T->leftChild != NULL)
        {
            fprintf(fp, "\"node%d\":f0 -> \"node%d\":f1 [color = red,label=\"L\"];\n", T->key, T->leftChild->key);
            displayTreeHelper(T->leftChild, fp);
        }
        if(T->rightChild != NULL)
        {
            if(!T->rightThread)
            {
                fprintf(fp, "\"node%d\":f2 -> \"node%d\":f1 [color=black,label=\"R\"];\n", T->key, T->rightChild->key);
                displayTreeHelper(T->rightChild, fp);
            }
            else{
                fprintf(fp, "\"node%d\":f2 -> \"node%d\":f1 [color = blue,style=dotted,label=\"suc\"];\n", T->key, T->rightChild->key);
            }
            
        }
    }
}

int displayRightThreadedBST(rTBTNodePtr T, char* filename)
{
    FILE *fp;

    fp = fopen(filename, "w+");
    if (fp == NULL)
    {
        printf("displayRightThreadedBST(): Unable to open file %s", filename);
        return -1;
    }
    fprintf(fp, "digraph g{\nnode[ shape=record,height=.1];");
    displayTreeHelper(T, fp);
    fprintf(fp,"\n }\n");

    fclose(fp);

    return 0;
}

int main(int argc, char *argv[])
{
    rTBTNodePtr T;

    createRightThreadedBST(&T);
    rtbstInsert(&T, 7);
    rtbstInsert(&T, 4);
    rtbstInsert(&T, 3);
    rtbstInsert(&T, 2);
    rtbstInsert(&T, 6);
    rtbstInsert(&T, 5);
    rtbstInsert(&T, 11);
    rtbstInsert(&T, 8);
    rtbstInsert(&T, 9);
    rtbstInsert(&T, 12);
    rtbstInsert(&T, 13);
    rtbstInsert(&T, 1);
    int c;
    int key,k;
    do{
        printf("Press any non numeric character to exit \n");
        printf("Press 1 to insert a node\n");
        printf("Press 2 to delete key \n");
        printf("Press 3 to search for a key\n");
        printf("Press 4 to display tree in graphviz (output is present in rtbst.png )\n");   
        printf("Press 5 to find kth Smallest Element\n");
        printf("Press 6 to get inorder sequence \n"); 
        printf("Enter your choice : ");
        if(scanf("%d",&c)==0)
            break;
        switch(c)
        {
            case 1:
                printf("Enter key to insert : ");
                scanf("%d",&key);
                if(rtbstInsert(&T,key) == 0)
                    printf("key succesfully inserted\n");
                else
                    printf("key already exists in bst\n");
                break;
            case 2:
                printf("Enter key to delete : ");
                scanf("%d",&key);
                if(rtbstDelete(&T,key) != -1)
                    printf("Deletion is possible!!!\n");
                else
                    printf("Deletion is not possible!!!\n");
                break;
            case 3:
                printf("Enter key to search : ");
                scanf("%d",&key);
                if(rtbstSearch(&T,key) != NULL)
                    printf("key found\n");
                else
                    printf("key not exists in bst\n");
                break;
            case 4:
                //Displaying Right Threaded tree
                printf("Right Threaded BST is displayed\n");
                displayRightThreadedBST(T, "rtbst.dot");
                system("dot -Tpng rtbst.dot -o rtbst.png");
                break;   
            case 5:
                printf("Enter to find the kth smallest element : ");
                scanf("%d",&k);
                rTBTNodePtr kthSmallest = kthElement(&T,k);
                if(kthSmallest != NULL)
                    printf("%d\n",kthSmallest->key);
                else
                    printf("kth Smallest element not present\n");
                break;
            case 6:
                if(T != NULL)
                {
                    int *List = (int *)malloc(T->size*sizeof(int));
                    inorder(&T,List);
                    int i;
                    for(i = 0; i < T->size; i++)
                        printf("%d\t",List[i]);
                    printf("\n");
                    free(List);
                }
                else
                printf("Empty Tree \n");
                break;         
            default:
                printf("Wrong choice\n");
        }
    
    }while(1);
    
    return 0;
}
