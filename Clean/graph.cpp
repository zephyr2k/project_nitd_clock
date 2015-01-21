typedef struct AdjListNode
{
    int dest;
    AdjListNode* next;
};

// A structure to represent an adjacency liat
typedef struct AdjList
{
    AdjListNode *head;  // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
typedef struct Graph
{
    int V;
    AdjList* su;
    AdjList* pre;

};
// A utility function to create a new adjacency list node
AdjListNode* newAdjListNode(int dest)
{
    AdjListNode* newNode =
            (AdjListNode*) malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
Graph* createGraph(int V)
{
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V
    graph->su = (AdjList*) malloc(V * sizeof(AdjList));
    graph->pre = (AdjList*) malloc(V * sizeof(AdjList));

     // Initialize each adjacency list as empty by making head as NULL
    int i;
    for (i = 0; i < V; ++i){
        graph->su[i].head = NULL;
        graph->pre[i].head = NULL;
        }

    return graph;
}
