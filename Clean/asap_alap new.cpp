

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stack>
using namespace std;
int as[1000];
    int al[1000];
    int V;

struct AdjListNode
{
    int dest;
    struct AdjListNode* next;
};

struct AdjList
{
    struct AdjListNode *head;
};


struct Graph
{
    int V;
    struct AdjList* su;
    struct AdjList* pre;
    int mob[1000];

};


struct AdjListNode* newAdjListNode(int dest)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}


struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;

    graph->su = (struct AdjList*) malloc(V * sizeof(struct AdjList));
    graph->pre = (struct AdjList*) malloc(V * sizeof(struct AdjList));


    int i;
    for (i = 0; i < V; ++i){
        graph->su[i].head = NULL;
        graph->pre[i].head = NULL;
        }

    return graph;
}


void addEdge(struct Graph* graph, int src, int dest)
{

    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->su[src].head;
    graph->su[src].head = newNode;


    newNode = newAdjListNode(src);
    newNode->next = graph->pre[dest].head;
    graph->pre[dest].head = newNode;
}


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
void mobility(struct Graph* graph,int V){
int t;
stack<int> f;



int te[100];
int s=0;
    int a,b,e,i,v;
 for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->pre[v].head;
        if(!pCrawl)
        {
            as[v]=1;

    f.push(v);

        }

    }
    while(!f.empty()){
struct AdjListNode* pCrawl = graph->su[f.top()].head;
t=int(f.top());
f.pop();
while (pCrawl)
        {  if(pCrawl->dest){
if(as[int(pCrawl->dest)]==0)
            as[int(pCrawl->dest)]=as[t]+1;
else
{if(as[int(pCrawl->dest)]<as[t]+1)
as[int(pCrawl->dest)]=as[t]+1;

}
            f.push(int(pCrawl->dest));}
            pCrawl = pCrawl->next;}

      //     f.pop();
           if(f.empty())
           break;

           }for(i=0;i<V;i++){
   printf("----asap-----%d %d\n",i+1,as[i]);}
   for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->su[v].head;
        if(!pCrawl)
        {
            al[v]=1000;

    f.push(v);

        }

    }

while(!f.empty()){
struct AdjListNode* pCrawl = graph->pre[f.top()].head;
t=int(f.top());
f.pop();
while (pCrawl)
        {  if(pCrawl->dest){
if(al[int(pCrawl->dest)]==0)
            al[int(pCrawl->dest)]=al[t]-1;
            else
            {if(al[int(pCrawl->dest)]>al[t]-1)
           al[int(pCrawl->dest)]=al[t]-1;
}
            f.push(int(pCrawl->dest));}

            pCrawl = pCrawl->next;}

      //     f.pop();
           if(f.empty())
           break;

           }
    int m=10000;
    for(i=0;i<V;i++){if(al[i]!=0 && m>al[i])
    m=al[i];




    }
    al[0]=1000;
   struct AdjListNode* pCrawl = graph->su[0].head;
    if(pCrawl->dest)
    al[0]=al[pCrawl->dest]-1;
    m=10000;
    for(i=0;i<V;i++){if(m>al[i])
    m=al[i];




    }


    for(i=0;i<V;i++){
    al[i]=al[i]-m+1;

    printf("----alap-----%d %d\n",i+1,al[i]);
    }
    for(i=0;i<V;i++){
graph->mob[i]=abs(al[i]-as[i]);
printf("mobility of -----%d ---- %d\n",i+1,abs(al[i]-as[i]));

    }}

    void createdot(struct Graph* graph,int V)
    {int v;FILE *fp;
char ch;
fp=fopen("graph.dot","w");

fprintf(fp, "digraph "" {\n");
   for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->su[v].head;


        while (pCrawl)
        {fprintf(fp, "%d -> %d\n",v+1,(pCrawl->dest)+1);
            pCrawl = pCrawl->next;
        }
    }
    fprintf(fp, "}");
   fclose(fp);
    }

int main()
{
int t;
stack<int> f;


    int V;
int te[100];
int s=0;
    int a,b,e,i,v;

    printf("enter no. of vertices ");
    scanf("%d",&V);
    struct Graph* graph = createGraph(V);

    printf("enter no. of edges ");
    scanf("%d",&e);
    printf("enter the edges in the format a b ");
    for(i=0;i<e;i++){
    scanf("%d%d",&a,&b);

    addEdge(graph, a-1, b-1);}

    printGraph(graph);

    createdot(graph,V);
    mobility(graph,V);

    return 0;
}
