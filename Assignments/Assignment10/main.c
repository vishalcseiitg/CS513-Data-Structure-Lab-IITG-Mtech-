#include<stdio.h>
#include "myStack.h"
#include "directedGraph.h"
#include <stdbool.h>



//[30 points] Non-recursive DFS routine to assign dfs number to each vertices, classify edges
void dfs(myGraph *G)
{
   FILE *fp = fopen("diagraph.txt" , "r");
   int v;
   fscanf(fp, %d , v);
   G-> numOfVertices = v;
   myStack s = (myStack)malloc(sizeof(myStack));
   createStack(s,v);
   
   int dfs_num = 0;
   bool vis[v];
   for(int i = 0 ; i < v ; i++){
	if(vis[i] == false && G->adjListEntry[i] !=NULL){
	dfs_num++;
	push(&s, adjListEntry[i]); 
	} 
    }
    while(s){
    int vertex = pop(&s , &i);
    printf("%d->" , vertex);
    }
}

// [20 points] Returns 1 if the graph is strongly connected, 0 ow -- use low number
int isStronglyConnected(myGraph *G)
{
	int v = G->numOfVertices;
	for(int i = 0 ; i < v ; i++){ //with the loop we are taking one vertex at one time
	bool vis[v];
	DFS(&G);
	for(int j = 0 ; j < v ; j++){
	if(vis[j] = false){
	return 0;
	}
     }
  }
	return 1;
}

void fill(int v , bool vis[] , myStack s){
	vis[v] = true;
	while(G->adjListEntry[i] = true){
	int n = G-> adjListEntry[i].linkptr;
	if(!vis[n])
	   fill(n , vis , s);
	}
    int x = push(s,v);
    }

//[50 points] Returns the component graph in which each vertex represents a strongly connected component in G 
void SCC(myGraph *G, myGraph *compGraph)
{
	myStack s;
	createStack(s,v);
	bool vis[v];
	
	for(int i = 0 ; i < v ; i++){
	vis[i] = false;
	}
	for(int i = 0 ; i < v ; i++){
	if(vis[i] ==false){
	fill(i , vis , s);
	}
	
	for(int i = 0 ; i < v ; i++){
	vis[i] = false;
	}
	while(push(s,i) != -1){
	int v = pop(s,i);
	if(vis[v] == false)
	{
	DFS(&G);
	printf("\n");
		}
	   }
      }

}
	
	
	

//---------------------------------------------------------------------------------------


int main()
{
    myGraph G;

    readDirectedGraph("./TestCases/mediumDG.txt", &G);

    printf("#vertices = %d #edged = %d\n", G.numOfVertices, G.numOfEdges);
    printAdjList(&G);

    dfs(&G);
    printGraph(&G, "./dfs_mediumDG.dot");

    //Calls to other functions
    
    deleteGraph(&G);

    return 0;
}
