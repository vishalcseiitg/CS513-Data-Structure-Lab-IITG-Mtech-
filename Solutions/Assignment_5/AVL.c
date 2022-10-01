#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
struct AVLNode
{
    int key;
    int bf; // bf = height(right subtree) - height(left subtree)
    struct AVLNode *leftChild;
    struct AVLNode *rightChild;
};

typedef struct AVLNode AVLNode;
typedef struct AVLNode *AVLNodePtr;

/*stack implemented for inertion*/
struct stackNode
{
    AVLNodePtr key;
    struct stackNode *link;
};

typedef struct stackNode stackNode;
typedef struct stackNode *stackNodePtr;

int push(stackNodePtr *top, AVLNodePtr data)
{
    stackNodePtr cur = (stackNodePtr)malloc(sizeof(stackNode));
    if (cur == NULL)
        return -1; // stack overflow
    cur->key = data;
    cur->link = *top;
    *top = cur;
    return 0; // push successfully happened
}
int pop(stackNodePtr *top)
{
    if (*top == NULL)
        return -1; // stack underflow
    stackNodePtr cur = *top;
    int temp = (cur->key)->key;
    *top = (*top)->link;
    free(cur);
    return temp;
}
bool isEmpty(stackNodePtr top)
{
    if (top == NULL)
        return true;
    return false;
}
AVLNodePtr peek(stackNodePtr top)
{
    if (isEmpty(top))
        return NULL; // stack underflow
    return top->key;
}

/*Tree functions*/
void createAVLTree(AVLNodePtr *root)
{
    *root = NULL;
}

AVLNodePtr getAVLTreeNode()
{
    AVLNodePtr p;
    p = (AVLNodePtr)malloc(sizeof(AVLNode));
    if (p != NULL)
    {
        p->leftChild = NULL;
        p->rightChild = NULL;
        p->bf = 0;
    }
    return p;
}

void displayAVLTreeHelper(AVLNodePtr root, FILE *fp)
{
    if (root != NULL)
    {
        if (root->leftChild != NULL)
        {
            fprintf(fp, "%d -> %d [color = red, style=dotted];\n", root->key, root->leftChild->key);
            displayAVLTreeHelper(root->leftChild, fp);
        }
        if (root->rightChild != NULL)
        {
            fprintf(fp, "%d -> %d ;\n", root->key, root->rightChild->key);
            displayAVLTreeHelper(root->rightChild, fp);
        }
    }
}

void displayAVLTree(AVLNodePtr root, char *filename)
{
    FILE *fp;

    fp = fopen(filename, "w+");
    if (fp == NULL)
    {
        printf("displayTree(): Unable to open file %s", filename);
    }
    fprintf(fp, "digraph g{\n");
    displayAVLTreeHelper(root, fp);
    fprintf(fp, "}\n");

    fclose(fp);
}

/*

*/
void LLRotate(AVLNodePtr *x)
{
    AVLNodePtr cur = *x;
    AVLNodePtr y = cur->rightChild;
    AVLNodePtr T3 = y->leftChild;
    y->leftChild = cur;
    cur->rightChild = T3;
    y->bf = 0;
    cur->bf = 0;
}
/*

*/
void RRRotate(AVLNodePtr *x)
{
    AVLNodePtr cur = *x;
    AVLNodePtr y = cur->leftChild;
    AVLNodePtr T3 = y->rightChild;
    y->rightChild = cur;
    (*x)->leftChild = T3;
    y->bf = 0;
    cur->bf = 0;
    *x = y;
}

void LRRotate(AVLNodePtr *x)
{
    AVLNodePtr y = (*x)->rightChild;
    AVLNodePtr z = y->leftChild;
    AVLNodePtr T2 = z->rightChild;
    z->rightChild = y;
    y->leftChild = T2;
    y->bf = 1;
    z->bf = 1;
    (*x)->rightChild = z;
    LLRotate(x);
}

void RLRotate(AVLNodePtr *x)
{
    AVLNodePtr y = (*x)->leftChild;
    AVLNodePtr z = y->rightChild;
    AVLNodePtr T2 = z->leftChild;
    z->leftChild = y;
    y->rightChild = T2;
    y->bf = -1;
    z->bf = -1;
    (*x)->leftChild = z;
    RRRotate(x);
}

void AVLInsert(AVLNodePtr *root, int data)
{
    AVLNodePtr p, currPtr, currPtrParent;
    /* Insert into an empty tree */
    if (*root == NULL)
    {
        p = getAVLTreeNode();
        if (p == NULL)
        {
            printf("\nAllocation failure");
            return;
        }

        p->key = data;
        *root = p;
        return;
    }
    stackNodePtr myStack = NULL;
    currPtr = *root;
    currPtrParent = NULL;
    while (currPtr != NULL)
    {
        if (currPtr->key == data)
        {
            printf("\nDuplicate element");
            return;
        }

        else if (currPtr->key > data)
        {
            currPtrParent = currPtr;
            currPtr = currPtr->leftChild;
        }
        else
        {
            currPtrParent = currPtr;
            currPtr = currPtr->rightChild;
        }
        push(&myStack, currPtrParent); // keeping record of the traversed nodes
    }
    p = getAVLTreeNode();
    if (p == NULL)
    {
        printf("\nAllocation failure");
        return;
    }

    p->key = data;

    if (currPtrParent->key > data)
        currPtrParent->leftChild = p;
    else
        currPtrParent->rightChild = p;

    /*balancing the tree*/
    AVLNodePtr child = p, grandChild = NULL;
    while (!isEmpty(myStack))
    {
        AVLNodePtr cur = peek(myStack);
        if (cur->rightChild && cur->rightChild == child)
            cur->bf++;
        else
            cur->bf--;

        if (cur->bf == 0)
            break;
        // if (cur->bf > 1)
        // {
        //     if (child->rightChild == grandChild)
        //         LLRotate(&cur);
        //     else
        //         LRRotate(&cur);
        // }
        // if (cur->bf < -1)
        // {
        //     if (child->leftChild == grandChild)
        //         RRRotate(&cur);
        //     else
        //         RLRotate(&cur);
        // }
        grandChild = child;
        child = cur;
        pop(&myStack);
    }

    // clearing the stack used
    while (!isEmpty(myStack))
        pop(&myStack);
}
void deleteTree(AVLNodePtr *root)
{
    if (*root == NULL)
        return;
    deleteTree(&((*root)->leftChild));  // recursively delete its left subtree
    deleteTree(&((*root)->rightChild)); // recursively delete its right subtree
    AVLNodePtr temp = *root;
    *root = NULL;
    free(temp); // freeing the node
}
int main()
{

    AVLNodePtr T;
    createAVLTree(&T);

    // AVLNodePtr p = getAVLTreeNode();
    // p->key = 5;
    // T = p;
    // p = getAVLTreeNode();
    // p->key = 4;
    // T->leftChild = p;
    // p = getAVLTreeNode();
    // p->key = 6;
    // T->rightChild = p;

    AVLInsert(&T, 1);
    AVLInsert(&T, 2);
    AVLInsert(&T, 3);

    displayAVLTree(T, "tree.dot");

    system("dot -Tpng tree.dot -o treeAfterInsertion.png");
    // deleteTree(&T);
    // displayAVLTree(T, "tree.dot");

    // system("dot -Tpng tree.dot -o treeAfterDeletion.png");

    // stackNodePtr myStack;
    // push(&myStack, T);
    // AVLNodePtr t1 = peek(myStack);
    // printf("%d ", t1->key);
    // int t = pop(&myStack);
    // printf("%d ", t);
    return 0;
}