/***
TODO:                                       STATUS

    Implement basic function:
        insert_ready_ops                    DONE
        schedule_op                         DONE
        delete                              DONE
        first                               DONE
    Implement List Scheduling               WIP
    Implement Schedule Structure            DONE, (FEATURES MAY BE ADDED)
    Implement Plist                         DONE, (FEATURES MAY BE ADDED)
    Graph Linking                           Done

** Requires reading schedule from Graph with ASAP , ALAP & Mobility of each vertex already computed.
    ---> Function added to compute mobility,asap,alap
BUGS
    1. Inadequate hw pushes other operation in that Control Step to below it - FIXED (It ignores the new schedule operation)

***/

#include<iostream>
#include<malloc.h>
#include<vector>
#include<string>
#include<stdlib.h>
#include<stack>

enum{OP_PRINT,LIST_PRINT,SCHEDULE_OPS_PRINT};
enum{PRIMARY,SECONDARY};
enum{UN_SCH,SCH};
enum{FAILURE,SUCCESS};

#define MAX_LT 100

using namespace std;

/// PList Node Structure for Scheduling
///
typedef struct PList
{
    char op;
    PList *next; //Schedule Structure
    //PList *fwd; // Original Graph Sequence
    int mobility;
    int vertex_no;
    int cStep;
    PList(char init_op)
    {
        op=init_op;
        next=NULL;
        mobility=0;
        vertex_no=0;
        cStep=-1;
    }
    // Variations of print for better eligiblity
    int toString()
    {
        if(vertex_no<10)
            cout<<" "<<vertex_no;
        else
            cout<<""<<vertex_no;
        if(mobility<10)
            cout<<"< "<<mobility<<">";
        else
            cout<<"<"<<mobility<<">";

        //cout<<"      ";
        return 1;
    }
    int toString(int k)
    {
       if(vertex_no<10)
            cout<<"  ";
        else
            cout<<" ";
        if(mobility<10)
            cout<<"    ";
        else
            cout<<"   ";
        return 1;
    }
};
typedef struct PListHead
{
    PList *head;
};
typedef struct PListMain
{
    int M; /// No of Operation ----NOT---- no of vertex
    char op_type[MAX_LT];
    PListHead *direct;
    PListMain(string ops)
    {
        M=ops.length();
        direct=(PListHead *) malloc(M * sizeof(PListHead));
        for(int i=0;i<M;i++)
        {
             op_type[i]=ops[i];
        }
    }

 int toString()
    {
        cout<<"\n\n--------PLIST CONTENTS------------\n\n";
        for(int i=0;i<M;i++)
        {
            printf("%c ",op_type[i]);
            PList *p=direct[i].head;
            while(p!=NULL)
            {
                cout<<" "<<p->vertex_no+1<<"<"<<p->mobility<<"> ";
                p=p->next;

            }
            cout<<"\n";

        }
        cout<<"\n--------PLIST CONTENTS------------\n\n";
    }

};
typedef struct ScheduleHead
{
    PList *head;
};
/// SCHEDULE STRUCTURE
typedef struct Schedule
{
    int n_cSteps;           // No of Control Steps
    int n_ops;              // No of operators i.e. Hardware logic
    ScheduleHead *pOps;     // Pointer to each type of operation and its schedule in control steps
    PList *tail;
    string op_arrange;      // HW operators arrangement
    Schedule(int n_cS,string op)
    {
        //op - String for hardware constraints like adder
        /***
        n_cS - no of control steps
        n_o - no of operators (5 for below)
        Usage : For 2 multiplication, 1 adder, 1 subtractor, 1 comparator
        op should be : **+-> (in any order)
        ***/
        n_cSteps=n_cS;
        n_ops=op.length();
        op_arrange=op;
        pOps = (ScheduleHead*) malloc(n_ops * sizeof(ScheduleHead));

        for (int i = 0; i < n_ops; ++i)
        {
            PList *p=new PList(op[i]);
            pOps[i].head = p;
        }

    }
    int toString()
    {
        cout<<"\n\n-----------------SCHEDULE-----------------\n\n";
        cout<<"VERTEX NUMBER < MOBILITY > CONTROL STEP\n";
        cout<<"\ncSteps: "<<n_cSteps<<"\t nOps: "<<n_ops<<"\n\t    ";
        //for(int k=0;k<n_cSteps;k++)
       //{
         //   cout<<""<<k+1<<"  \t  ";
       // }
        cout<<"\n";

            for(int j=0;j<n_ops;j++)
            {
                cout<<""<<op_arrange[j]<<"\t";
                //cout<<"\n";
                PList *temp=pOps[j].head;
               int ctr=1;

                if(temp->cStep!=1)
                {
                        while(ctr<=temp->cStep)
                        {
                        temp->toString(1);
                        cout<<"  ";
                        ctr+=1;
                        }
                }
                //cout<<ctr<<"\n";
                while(temp!=NULL && temp->cStep>=0)
                {

                    temp->toString();
                    if(temp->cStep<10)
                        cout<<" "<<temp->cStep;
                    else
                        cout<<""<<temp->cStep;
                    cout<<" --> ";
                    temp=temp->next;
                    ctr+=1;
                }
                 cout<<"\n";
            }
        cout<<"\n-----------------SCHEDULE-----------------\n\n";
        return 1;
    }
};
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
    AdjList* pre_non_d;     //NON destructive predecssor List for Dynamic List Scheduling
    int asap[MAX_LT];
    int alap[MAX_LT];
    int mob[MAX_LT];
    char operation[MAX_LT];
    int stat[MAX_LT];       // ??? Forgot -- Status of what
};
// A utility function to create a new adjacency list node
AdjListNode* newAdjListNode(int dest)
{
    AdjListNode* newNode =(AdjListNode*) malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}
///  Global Variable Declaration HERE
///
///
///

// A utility function that creates a graph of V vertices
Graph* createGraph(int V)
{
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V
    graph->su = (AdjList*) malloc(V * sizeof(AdjList));
    graph->pre = (AdjList*) malloc(V * sizeof(AdjList));
    graph->pre_non_d = (AdjList*) malloc(V * sizeof(AdjList));


     // Initialize each adjacency list as empty by making head as NULL

    for (int i = 0; i < V; ++i){
        graph->operation[i]='E';
        graph->su[i].head = NULL;
        graph->pre[i].head = NULL;
        graph->pre_non_d[i].head = NULL;
        graph->mob[i]=-1;
        graph->stat[i]=1;
        graph->alap[i]=-1;
        graph->asap[i]=-1;
        }
    return graph;
}
