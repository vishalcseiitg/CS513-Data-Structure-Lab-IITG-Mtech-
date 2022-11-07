#include<stdio.h>
#include "myStack.h"
#include "directedGraph.h"
#include <assert.h>
#include "myQueue.c"
#include "myQueue.h"


int time = 0;
void DepthFirstSearch(myGraph *G, int v, int child[], int currComponent, int parent[]);

//Non-recursive DFS routine to assign dfs number to each vertices, classify edges
void dfs(myGraph *G)
{
	time = -1;
    int currComponent = -1;
    // G->numOfComp = -1;
    int *child = (int *)malloc(G->numOfVertices * sizeof(int));
    int *parent = (int *)malloc(G->numOfVertices * sizeof(int));

    for (int v = 0; v < G->numOfVertices; v++)
    {
        child[v] = 0;
        parent[v] = -1;
        G->adjList[v].dfsNum = G->adjList[v].lowNum = -1;
    }
    for (int v = 0; v < G->numOfVertices; v++)
        if (G->adjList[v].dfsNum == -1)
        {
            // currComponent++;
            // G->numOfComp = currComponent + 1;
            // printf("1");
            // dfsCore(G, v, mark, currComponent, parent);
            DepthFirstSearch(G, v, child, currComponent, parent);
        }

    free(child);
    free(parent);
}

void DepthFirstSearch(myGraph *G, int v, int child[], int currComponent, int parent[])
{
    edgeListNodePtr *remaining = (edgeListNodePtr *)malloc(G->numOfVertices * sizeof(edgeListNodePtr));
    for (int v = 0; v < G->numOfVertices; v++)
        remaining[v] = G->adjList[v].edgeListHeadPtr;
    myStack child;
    createStack(&stack, G->numOfVertices);
    push(&stack, v);
    child[v] = 1;
    time = time + 1;
    G->adjList[v].dfsNum = time;
    G->adjList[v].lowNum = G->adjList[v].dfsNum;
    while (v != -1)
    {
        edgeListNodePtr itr = remaining[v];
        while (itr)
        {
            if (G->adjList[itr->vertex].dfsNum == -1) // tree edge
            {
                parent[itr->vertex] = v;
                push(&stack, itr->vertex);
                itr->edgeType = 1;
                remaining[v] = itr->linkPtr;
                v = itr->vertex;
                child[v] = 1;
                time = time + 1;
                G->adjList[v].dfsNum = time;
                G->adjList[v].lowNum = G->adjList[v].dfsNum;
                break;
            }
            else if (G->adjList[itr->vertex].dfsNum > G->adjList[v].dfsNum) // forward edge
            {
                itr->edgeType = 2;
            }
            else if (child[itr->vertex] == 0) // cross edge
            {
                if (G->adjList[v].lowNum > G->adjList[itr->vertex].dfsNum)
                    G->adjList[v].lowNum = G->adjList[itr->vertex].dfsNum;
                itr->edgeType = 4;
            }
            else // back edge
            {
                if (G->adjList[v].lowNum > G->adjList[itr->vertex].dfsNum)
                    G->adjList[v].lowNum = G->adjList[itr->vertex].dfsNum;
                itr->edgeType = 3;
            }
            itr = itr->linkPtr;
        }
        if (itr)
            continue;
        // all children traversed
        // v should be equal to currVertex
        remaining[v] = NULL;
        int currVertex;
        pop(&stack, &currVertex);
        child[v] = 0;
        if (parent[currVertex] != -1)
            if (G->adjList[parent[currVertex]].lowNum > G->adjList[currVertex].lowNum)
                G->adjList[parent[currVertex]].lowNum = G->adjList[currVertex].lowNum;
        v = parent[currVertex];
    }
    free(remaining);
}

// Returns the component graph in which each vertex represents a strongly connected component in G
void SCC(myGraph *G, myGraph *compGraph)
{
	 time = -1;
    int currComponent = -1;
    // G->numOfComp = -1;
    int *child = (int *)malloc(G->numOfVertices * sizeof(int));
    int *parent = (int *)malloc(G->numOfVertices * sizeof(int));

    for (int v = 0; v < G->numOfVertices; v++)
    {
        child[v] = 0;
        parent[v] = -1;
        G->adjList[v].dfsNum = G->adjList[v].lowNum = -1;
    }
    for (int v = 0; v < G->numOfVertices; v++)
        if (G->adjList[v].dfsNum == -1)
        {
            // currComponent++;
            // G->numOfComp = currComponent + 1;
            // printf("1");
            // dfsCore(G, v, mark, currComponent, parent);
            DepthFirstSearch(G, v, child, currComponent, parent);
        }

    free(child);
    free(parent);
}

// A directed graph G = (V, E)is semiconnected if, for all pairs // of vertices u and v, we have a path from u to vor v to u.
// Time complexity O(V+E)
// Score - [30 Marks]
int isSemiConnected(myGraph *G)
{
	 myGraph compGraph;
    SCC(G, &compGraph);                                          // Finding the DAG component of the graph.
    printGraph(&compGraph, "./graph.dot");
    system("dot -Tpng graph.dot -o scc_graph.png");

    int indegree[compGraph.numOfVertices], topological_order[compGraph.numOfVertices], index=0; // to find the topological order of DAG we are using the function indegree
    
    for(int i=0; i<compGraph.numOfVertices; i++){             //linear sequence of the node is stored in topological DAG
        indegree[i] = 0;
    }
    for(int i=0; i<compGraph.numOfVertices; i++){
        edgeListNodePtr edgeNodePtr = compGraph.adjList[i].edgeListHeadPtr;
        while(edgeNodePtr)
        {
            indegree[edgeNodePtr->vertex]++;                            // For each edge in component graph 
            edgeNodePtr = edgeNodePtr->linkPtr;
        }
    }
    myQueue q;
    createQueue(&q, compGraph.numOfVertices);
    for(int i=0; i<compGraph.numOfVertices; i++){
        if(indegree[i] == 0){
            pushQueue(&q, i);                           // Add node which have 0 degree in queue in the starting
        }
    }
    while(!isQueueEmpty(&q)){
        int u;
        deleteQueue(&q, &u);
        topological_order[index] = u;                                // Add node in topological order as all dependencies are removed.
        edgeListNodePtr nbr = compGraph.adjList[u].edgeListHeadPtr;
        while(nbr)
        {
            int w = nbr->vertex;
            indegree[w]--;               // Remove dependency for all neighbours if node is added in topological order.
            if(indegree[w] == 0){
                pushQueue(&q, w);      // If dependency is zero then add node in queue.
            }
            nbr = nbr->linkPtr;
        }
        index++;
    }

    printf("\nTopological order of graph is:\n");
    for(int i=0; i<compGraph.numOfVertices; i++){
        printf("%d ", topological_order[i]);
    }
    printf("\n");

    int flag = 0;
    for(int u=0; u<compGraph.numOfVertices-1; u++){
        edgeListNodePtr edgeNodePtr = compGraph.adjList[topological_order[u]].edgeListHeadPtr;
        flag = 0;
        while(edgeNodePtr)
        {
            int v = edgeNodePtr->vertex;     // In topological ordering two adjacent nodes must have edge between them otherwise
            if(v == topological_order[u+1]){      // path between those nodes is not possible
                flag = 1;
                break;
            }
            edgeNodePtr = edgeNodePtr->linkPtr;
        }
        if(!flag){                                    // If edge doesn't present in consecutive nodes then Graph is not semiconnected.
            printf("Graph is not semi connected\n");
            return 0;
        }
    }
    printf("Graph is semi connected\n");                 // For all consecutive nodes edge present, so Graph is semiconnected.
    return 1;
}

//Find the longest path and its length in the component graph
// Score - [30 Marks]
void longestPath(myGraph *G, int *path, int *pathLen)
{
	 myGraph compGraph;
    SCC(G, &compGraph);                                 // To find longest path we must have to fine component graph of given graph.
    printGraph(&compGraph, "./graph.dot");
    system("dot -Tpng graph.dot -o scc_graph.png");

    int indegree[compGraph.numOfVertices], length_path[compGraph.numOfVertices], parent[compGraph.numOfVertices], max_length_vertex=0;
    for(int i=0; i<compGraph.numOfVertices; i++){                      
        indegree[i] = 0;                                      // indegree is used to find topological order of DAG 
        length_path[i] = 0;                                  // length_path array is used to store longest path for that particular node.
        parent[i] = -1;      // Parent is used to store parent of node from which longest path is possible, it is used to backtrack, and find path with longest path.
    }
    *pathLen = 0;
    for(int i=0; i<compGraph.numOfVertices; i++){
        edgeListNodePtr edgeNodePtr = compGraph.adjList[i].edgeListHeadPtr;
        while(edgeNodePtr)
        {
            indegree[edgeNodePtr->vertex]++;                // Update indegree for each edge in component graph
            edgeNodePtr = edgeNodePtr->linkPtr;
        }
    }

    myQueue q;
    createQueue(&q, compGraph.numOfVertices);
    for(int i=0; i<compGraph.numOfVertices; i++){
        if(indegree[i] == 0){                             // At starting add node which have 0 degree in queue
            pushQueue(&q, i);
        }
    }
    while(!isQueueEmpty(&q)){
        int u;
        deleteQueue(&q, &u);
        edgeListNodePtr nbr = compGraph.adjList[u].edgeListHeadPtr;
        while(nbr)
        {
            int w = nbr->vertex;
            indegree[w]--;                                              // Remove dependency for all neighbours if node is added in topological order.
            if(length_path[w] < (1+length_path[u])){                    // If we found larger path for vertex then update longest path for vertex and update parent of vertex.
                length_path[w] = (1+length_path[u]);                  
                parent[w] = u;
                if(*pathLen < length_path[w]){                  // If we found path length which is greater than max length then update max length and also store vertex which have max length
                    *pathLen = length_path[w];
                    max_length_vertex = w;
                }
            }
            if(indegree[w] == 0){
                pushQueue(&q, w);
            }
            nbr = nbr->linkPtr;
        }
    }

    printf("Path length of each vertices\n");
    for(int i=0; i<compGraph.numOfVertices; i++){
        printf("%d ", length_path[i]);
    }
    printf("\n");

    printf("Max length is %d\n", *pathLen);
    
    int p[compGraph.numOfVertices], vertex = max_length_vertex;
    int i=0;
    p[i++] = vertex;
    while(parent[vertex] != -1){             // Get the longest path by backtracking of parent from vertex which cause max length.
        p[i++] = parent[vertex];             // Which is in reverse order.
        vertex = parent[vertex];
    }

    for(int j=0; j<=*pathLen; j++){          // Get actual longest path by reversing previous reverse order path.
        path[j] = p[--i];
    }

    printf("Path\n");
    for(int i=0; i<=*pathLen; i++){
        printf("%d ", path[i]);
    }
    printf("\n");
}




int main()
{
    myGraph G;

    readDirectedGraph("./TestCases/mediumDG.txt", &G);

    printf("#vertices = %d #edged = %d\n", G.numOfVertices, G.numOfEdges);
    printAdjList(&G);

    printGraph(&G, "./dfs_mediumDG.dot");

    //Calls to other functions

    deleteGraph(&G);

    return 0;
}
