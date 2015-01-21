// A C Program to demonstrate adjacency list representation of graphs

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stack>
using namespace std;
    int as[1000];
    int al[1000];
    int V;
// A structure to represent an adjacency list node
struct AdjListNode
{
    int dest;
    struct AdjListNode* next;
};

// A structure to represent an adjacency liat
struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph
{
    int V;

    struct AdjList* su;
    struct AdjList* pre;

};

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V
    graph->su = (struct AdjList*) malloc(V * sizeof(struct AdjList));
    graph->pre = (struct AdjList*) malloc(V * sizeof(struct AdjList));

     // Initialize each adjacency list as empty by making head as NULL
    int i;
    for (i = 0; i < V; ++i){
        graph->su[i].head = NULL;
        graph->pre[i].head = NULL;
        }

    return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->su[src].head;
    graph->su[src].head = newNode;

    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src);
    newNode->next = graph->pre[dest].head;
    graph->pre[dest].head = newNode;
}

// A utility function to print the adjacenncy list representation of graph
void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->su[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
    printf("..........");
    for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->pre[v].head;
        printf("\n predecesor list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }

}
void asap(struct Graph* graph,int V)
{
int t;
stack<int> f;
// create the graph given in above fugure
int te[100];
int s=0;
int a,b,e,i,v;
for (v = 0; v < graph->V; ++v)
{
    struct AdjListNode* pCrawl = graph->pre[v].head;
    if(!pCrawl)
    {
        as[v]=1;
        /*  stack<int>::iterator position = find(t.begin(), t.end(), v);
if (position != stack.end()) // == stack.end() means the element was not found
    t.erase(position);*/
        f.push(v);
    }
}
while(!f.empty())
{
    struct AdjListNode* pCrawl = graph->su[f.top()].head;
    t=int(f.top());
    f.pop();
    while (pCrawl)
    {
        if(pCrawl->dest)
        {
            printf("%d---%d----\n",int(pCrawl->dest),t);
            if(as[int(pCrawl->dest)]==0)
                as[int(pCrawl->dest)]=as[t]+1;
            else
            {
                if(as[int(pCrawl->dest)]<as[t]+1)
                    as[int(pCrawl->dest)]=as[t]+1;
            }
            f.push(int(pCrawl->dest));}
            pCrawl = pCrawl->next;}
            //f.pop();
           if(f.empty())
                break;
           }
           for(i=0;i<V;i++)
           {
                printf("----asap-----%d %d\n",i,as[i]);
            }
}
void alap(struct Graph* graph,int V)
{
int t;
stack<int> f;
    // create the graph given in above fugure
int te[100];
int s=0;
int a,b,e,i,v;
for (v = 0; v < graph->V; ++v)
{
    struct AdjListNode* pCrawl = graph->su[v].head;
    if(!pCrawl)
    {
        al[v]=1000;
        f.push(v);
    }
}
while(!f.empty())
{
    struct AdjListNode* pCrawl = graph->pre[f.top()].head;
    t=int(f.top());
    f.pop();
    while (pCrawl)
    {
        if(pCrawl->dest)
        {
            printf("%d---%d----\n",int(pCrawl->dest),t);
            if(al[int(pCrawl->dest)]==0)
                al[int(pCrawl->dest)]=al[t]-1;
            else
            {
                if(al[int(pCrawl->dest)]>al[t]-1)
                al[int(pCrawl->dest)]=al[t]-1;
            }
            f.push(int(pCrawl->dest));
        }
            pCrawl = pCrawl->next;
    }
    if(f.empty())
        break;

}
int m=10000;
for(i=0;i<V;i++)
{
    if(al[i]!=0 && m>al[i])
        m=al[i];
}
al[0]=1000;
struct AdjListNode* pCrawl = graph->su[0].head;
if(pCrawl->dest)
    al[0]=al[pCrawl->dest]-1;
m=10000;
for(i=0;i<V;i++)
{
    if(m>al[i])
        m=al[i];
}
for(i=0;i<V;i++)
{
    al[i]=al[i]-m+1;
    printf("----alap-----%d %d\n",i,al[i]);
}

}
void createdot(struct Graph* graph,int V)
{
int v;FILE *fp;
char ch;
fp=fopen("graph.dot","w");
//fprintf(fp, "%s %s %s %d", "We", "are", "in", 2012);
fprintf(fp, "digraph "" {\n");
   for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->su[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);

        while (pCrawl)
        {
            fprintf(fp, "%d -> %d\n",v,pCrawl->dest);
            pCrawl = pCrawl->next;
        }
    }
    fprintf(fp, "}");
   fclose(fp);
}
// Driver program to test above functions
int main()
{
int t;
stack<int> f;

    // create the graph given in above fugure
    int V;
int te[100];
int s=0;
    int a,b,e,i,v;

    printf("enter no. of vertices");
    scanf("%d",&V);
    struct Graph* graph = createGraph(V);

    printf("enter no. of edges");
    scanf("%d",&e);
    printf("enter the edges in the format a-b");
    for(i=0;i<e;i++){
    scanf("%d%d",&a,&b);

    addEdge(graph, a, b);}
//finding asap scheduling

           //finding asap scheduling


           //finding alap scheduling

    // print the adjacency list representation of the above graph
    printGraph(graph);
    asap(graph,V);
    alap(graph,V);
    createdot(graph,V);
    for(i=0;i<V;i++){

    printf("mobility of -----%d ---- %d\n",i,abs(al[i]-as[i]));
    }

    return 0;
}
