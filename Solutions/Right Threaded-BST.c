#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>

//Name - Vishal Kumar
//Roll no - 226101005

struct rightThreadedBSTNode{
	int key;
	int size; // total number of nodes in the subtree rooted at node
	int rightThread; // 1 , if right link is a thread to its inorder successor
	
	struct rightThreadedBSTNode *leftChild;
	struct rightThreadedBSTNode *rightChild;
};

typedef struct rightThreadedBSTNode rTBSTNode;
typedef struct rightThreadedBSTNode* rTBSTNodePtr;

void createRightThreadedBST(rTBSTNodePtr *root){
	*root = NULL;
	}
	
rTBSTNodePtr getRightThreadedTreeNode() //allocating a node dynamically
{
     rTBSTNodePtr ptr;
     ptr = (rTBSTNodePtr)malloc(sizeof(rTBSTNodePtr)); //chcecking if the memory is allocated or not
     if (ptr != NULL)
     {
           ptr->leftChild = NULL;
           ptr->rightChild = NULL;
           ptr->size = 0;
     }
     return ptr;
}
	

//first we will traverse the tree to find the leftmost node of the tree


rTBSTNodePtr leftMost(rTBSTNodePtr n) //Function to find the leftmost element
{
	if (n == NULL)
		return NULL;

	while (n->leftChild != NULL) // run the while loop till we do not get the last element
		n = n->leftChild;

	return n;
}


void inorder(rTBSTNodePtr root)  //function to traverse the BST inorder non recursively and without using stacks
{
	rTBSTNodePtr curr_node = leftMost(root); // we will take the root value from the leftMost function above 
	
	while (curr_node != NULL) { // running the while loop till our current node does not become NULL;
	
		printf("%d ", curr_node->key); // printing the data in the current node.
		
		if (curr_node->rightChild)
			curr_node = curr_node->rightChild;
		else 
			curr_node = leftMost(curr_node->rightChild);
	}
}

int rtbstInsert(rTBSTNodePtr root, int data)  //function to insert in the threaded BST
{
        rTBSTNode *temp;
        rTBSTNode *param;
        rTBSTNode *ptr;

        int found=0; //taking a found counter and put its value as 0; it is to check for the duplicate keys

        ptr = root; //ptr is pointing towards the root element;
        param = NULL;

        while( ptr!=NULL )  //we will run this while loop till ptr will not become NULL;
        {
                if( data == ptr->key)
                {
                        found =1; //it means the data is found what we are looking at
                        break; // no need to check further.
                }
                param = ptr;
                if(data < ptr->key)
                {
                        if(ptr->rightThread == true) // when right_thread is present we will move our pointer towards left_child
                                ptr = ptr->leftChild;
                        else
                                break;
                }
                else
                {
                        if(ptr->rightThread == false) // when right_thread is absent we will point towards the right_child
                                ptr = ptr->rightChild;
                        else
                                break;
                }
        }

        if(found)  //if found is true it means BST has duplicate values
        
                printf("\nDuplicate key found");
        else
        {

                temp=(rTBSTNodePtr)malloc(sizeof(rTBSTNodePtr));  //allocation of memeory dynamically
                temp->key = data;
                temp->rightThread = true;
                if(param==NULL)
                {
                        root=temp;
                        temp->leftChild=NULL;
                        temp->rightChild=NULL;
                }
                else if( data < param->key)
                {
                        temp->leftChild=param->leftChild;
                        temp->rightChild=param;
                        param->rightThread=true;
                        param->leftChild=temp;
                }
                else
                {
                        temp->leftChild=param;
                        temp->rightChild=param->rightChild;
                        param->rightThread=false;
                        param->rightChild=temp;
                }
        }
        return root;
        
}
 
 // code for the main function
        
int main(){

    rTBSTNodePtr root = NULL;
 
    root = rtbstInsert(root, 23);
    root = rtbstInsert(root, 40);
    root = rtbstInsert(root, 56);
    root = rtbstInsert(root, 68);
	
    inorder(root);
	
}
