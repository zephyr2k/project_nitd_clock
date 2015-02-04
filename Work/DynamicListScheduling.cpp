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

#define DEBUG
#define HAL

#define MAX_LT 100

#include "basic.h"
using namespace std;


/** Deletes references to predecessor node 'toDel' from successor list **/
Graph* delete_Succ(Graph *graph,int fromDel,int toDel)
{
    AdjListNode *curr=graph->pre[fromDel].head;
    AdjListNode *prev=curr;
    //printf("\nFrom %d delete %d\n",curr->dest,toDel);
    if(curr==NULL)
        {
        curr=NULL;
       // cout<<"1.\n";
        }
    else if(curr->dest==toDel)
        {
        /// Not Working
        /// FIXED
        graph->pre[fromDel].head=graph->pre[fromDel].head->next;
       // cout<<"2.\n";
       //printf("Delete from %d to %d\n",fromDel,toDel);
        }
    else
    {
    prev=curr;
    curr=curr->next;
    //cout<<"3.\t";
    while(curr!=NULL)
    {
       // cout<<curr->dest<<"\n";

        if(curr->dest==toDel)
        {
            prev->next=curr->next;
            curr->next=NULL;
            break;
        }

        prev=curr;
        curr=curr->next;
    }
    }
return graph;
}


/** Takes PListMain inserts all ready operation in Schedule **/
int insert_ready_ops(Graph *graph,Schedule *s,PListMain *plMain)
{
    /***
    Scans all nodes, check for predecessors are scheduled or not,
    If scheduled deletes V ,and appends to Plist based on operation type
    ***/
    cout<<"IRO";
    vector<int> visited(graph->V,0);
    //int visited[graph->V];

    //for(int v=0;v<graph->V;v++){visited[v]=0;}

    for(int v=0;v<graph->V;v++)
    {
        //checks for NULL ref
        // check if adjNodes of v are schedules in 's' or not
        //printf("\n Test Vertex %d(%d) \n",v+1,checkPred(graph,v,s));
        if(visited[v]!=1 && graph->stat[v]!=0 && checkPred(graph,v,s))
        {
            //printf("Deleting %d.\n",v);
            //printf("\n Test Vertex %d(%d) \n",v+1,checkPred(graph,v,s));
            PList *t=new PList(graph->operation[v]);
            t->vertex_no=v;
            t->mobility=graph->mob[v];
            // current node to be deleted
            //goto successor of v delete all ref to v
            AdjListNode *p=graph->su[v].head;
            while(p!=NULL)
            {
                //cout<<p->dest<<"\t"<<v<<"\n";
                delete_Succ(graph,p->dest,v);
                visited[p->dest]=1;
                p=p->next;
            }
            //del all ref of curr

             graph->stat[v]=0;
             /***
                Above removal from list successfull .
                Below adding the removed nodes to PList

                Node must be added wrt to asc order of mobility
            ***/

            char type=graph->operation[v];

            for(int i=0;i<plMain->M;i++)
            {

                if( plMain->op_type[i]==type)
                {

                    /// Insert in corresp. PLMain
                    /// ORDER SHOULD BE ASC MOBLITLITY, FOR SAME MOBILITY : LOWER VERTEX NUMBER
                    //PList *tr;
                    //printf("\nAdding to List %d<%d>\n",t->vertex_no+1,t->mobility);
                    if(plMain->direct[i].head==NULL)
                    {
                        plMain->direct[i].head=t;
                        break;
                    }
                    int c=0;
                    PList *pCrawl=plMain->direct[i].head;
                    if(t->mobility<=pCrawl->mobility)
                    {
                        //Case: 1.Same moblity, 2. Mobilty Less for 't'
                        if(t->mobility<pCrawl->mobility||t->vertex_no<pCrawl->vertex_no)
                        {
                        t->next=pCrawl;
                        plMain->direct[i].head=t;
                        break;
                        }
                    }
                    while(pCrawl->next)
                    {
                        if((t->mobility<pCrawl->next->mobility))
                            {

                          //  cout<<"\n";
                            c=1;
                            break;
                            }
                        pCrawl=pCrawl->next;
                    }
                    if(c==1)
                    {
                      PList *tr=pCrawl->next;
                      pCrawl->next=t;
                       t->next=tr;
                    }
                    else
                    {
                        pCrawl->next=t;
                    }

                break;
                }
            }
        }

    }
    return SUCCESS;
}
// A utility function to print the adjacency list representation of graph
void printGraph(Graph* graph,int i=0)
{
    int v;
    cout<< "\n\n-------------Graph Printing Start----------------\n";
if(i==0)
    {
    cout<<" Vertex No : Mobility Operation\n";
    for (v = 0; v < graph->V; ++v)
    {
        if(graph->stat[v]!=0)
        cout<<v<<" : "<< graph->mob[v] <<" "<<graph->operation[v]<<"\n";
    }
    }
else if(i==1)
    {
    for (v = 0; v < graph->V; ++v)
    {
         if(graph->stat[v]!=0)
         {
        AdjListNode* pCrawl = graph->su[v].head;
        printf("\n Successor list of vertex %d: head ", v+1);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->dest+1);
            pCrawl = pCrawl->next;
        }
        }

    }
    printf("\n");
    printf("..........");
    printf("\n");
    for (v = 0; v < graph->V; ++v)
    {
        if(graph->stat[v]!=0)
         {
        AdjListNode* pCrawl = graph->pre[v].head;
        printf("\n Predecesor list of vertex %d : head ", v+1);

        while (pCrawl)
        {
            printf("-> %d", pCrawl->dest+1);
            pCrawl = pCrawl->next;
        }
        }
        //printf("\n");
    }
    }
    cout<<"\n ________________________\n";
    for (v = 0; v < graph->V; ++v)
    {
        cout<<"\n"<<v<<" assigned operation : "<<graph->operation[v];
    }
    cout<< "\n\n-------------Graph Printing End---------------\n\n";
}
void createdot(struct Graph* graph,int V)
{
    FILE *fp;
    char ch;
    fp=fopen("graph.dot","w");
    fprintf(fp, "digraph "" {\n");
    for (int v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->su[v].head;
        while (pCrawl)
        {
            fprintf(fp, "%d -> %d",v+1,(pCrawl->dest)+1);
            pCrawl = pCrawl->next;
        }
    }
    fprintf(fp, "}");
    fclose(fp);
}
int PListEmptyCheck(PListMain *p)
{
    int m=p->M;
    for(int i=0;i<m;i++)
    {
        if(p->direct[i].head!=NULL)
            return SUCCESS;
    }
    return FAILURE;
}
void createSchDot(Schedule *sch,int param)
{
    FILE *fp;
    char ch;
    fp=fopen("DynapmicLS_Schedule.txt","w");
    fprintf(fp,"// Schedule Format : Vertex Number < Mobility > Control Step \n");

    //fprintf(fp, "digraph "" {\n");
    for (int v = 0; v < sch->n_ops; ++v)
    {
        fprintf(fp,"%c : \t ",sch->op_arrange[v]);
        PList* pCrawl = sch->pOps[v].head;
        while (pCrawl)
        {
            fprintf(fp, "%d ( %d ) %d -> ",pCrawl->vertex_no,pCrawl->mobility,pCrawl->cStep);
            pCrawl = pCrawl->next;
        }
        fprintf(fp,"\n");
    }
    fprintf(fp, "");
    fclose(fp);
}
int asap_alap(Graph *graph)
{
int t;
vector<int> as(MAX_LT,0),al(MAX_LT,0);
stack<int> f;
int te[100];
int s=0;
   // int a,b,e,i,v;
for (int v = 0; v < graph->V; ++v)
{
    AdjListNode* pCrawl = graph->pre[v].head;
    if(!pCrawl)
    {
        as[v]=1;
        f.push(v);
    }
}
while(!f.empty())
{
    AdjListNode* pCrawl = graph->su[f.top()].head;
    t=int(f.top());
    f.pop();
    while (pCrawl)
    {
        if(pCrawl->dest)
        {
            if(as[int(pCrawl->dest)]==0)
                as[int(pCrawl->dest)]=as[t]+1;
            else
            {
                if(as[int(pCrawl->dest)]<as[t]+1)
                as[int(pCrawl->dest)]=as[t]+1;
            }
            f.push(int(pCrawl->dest));
        }
            pCrawl = pCrawl->next;
    }
    if(f.empty())
        break;
}
for(int i=0;i<graph->V;i++)
{
   //printf("----asap-----%d %d\n",i+1,as[i]);
}
for (int v = 0; v < graph->V; ++v)
{
    AdjListNode* pCrawl = graph->su[v].head;
    if(!pCrawl)
    {
        al[v]=MAX_LT;
        f.push(v);
    }
}
while(!f.empty())
{
    AdjListNode* pCrawl = graph->pre[f.top()].head;
    t=int(f.top());
    f.pop();
    while (pCrawl)
    {
        if(pCrawl->dest)
        {
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
int m=MAX_LT*10;
for(int i=0;i<graph->V;i++)
{
    if(al[i]!=0 && m>al[i])
        m=al[i];
}
al[0]=MAX_LT*10;
AdjListNode* pCrawl = graph->su[0].head;
if(pCrawl->dest)
    al[0]=al[pCrawl->dest]-1;
m=MAX_LT*10;
for(int i=0;i<graph->V;i++)
{
    if(m>al[i])
        m=al[i];
}
for(int i=0;i<graph->V;i++)
{
    al[i]=al[i]-m+1;
   // printf("----alap-----%d %d\n",i+1,al[i]);
}
#ifdef DEBUG
    cout<<"\nInside ASAL ALAP : Vertex Number (Mobility)\n";
#endif // DEBUG
for(int i=0;i<graph->V;i++)
{
    graph->mob[i]=abs(al[i]-as[i]);
    graph->asap[i]=as[i];
    graph->alap[i]=al[i];
    #ifdef DEBUG
        printf(" %d < %d >\n",i+1,abs(al[i]-as[i]));
    #endif // DEBUG
}
}
/// MAIN CORE
/// Implementation of List Scheduling
int ListScheduling(Graph *graph,Schedule *sch,PListMain *plMain)
{
    /// m -- No of PList operators
    /// k -- ??
        cout<<"\n Came Here 1.";
    insert_ready_ops(graph,sch,plMain);
        cout<<"\n Came Here 2.";
    int CStep=-1;
    while(PListEmptyCheck(plMain))//TRUE (Remains Inside), FALSE (BREAKS OUT)
    {
        #ifdef DEBUG
        plMain->toString();
        #endif // DEBUG_
        CStep+=1;
        for(int k=0;k<(plMain->M);k++)
        {
            PList *temp=plMain->direct[k].head;
            if(temp)
            {
                for(int funit=0;funit<(sch->n_ops);funit++)
                {
                    temp=plMain->direct[k].head;
                    if(temp!=NULL && temp->op==sch->op_arrange[funit])
                    {
                         #ifdef DEBUG
                        printf("To Schedule '%c' %d<%d> at step %d\n",temp->op,temp->vertex_no+1,temp->mobility,CStep+1);
                        #endif // DEBUG_

                        PList *tr=new PList(temp->op);
                        tr->mobility=temp->mobility;
                        tr->vertex_no=temp->vertex_no;
                        tr->cStep=CStep;
                        schedule_op(sch,tr,CStep,funit);

                            temp = plMain->direct[k].head->next;
                            plMain->direct[k].head=temp;

                    }
                }

            }
        }

        insert_ready_ops(graph,sch,plMain);
        #ifdef DEBUG
        sch->toString();
        #endif // DEBUG_
    }

    sch->n_cSteps=CStep+1;
    //sch->toString();
    return SUCCESS;
}

// Adjustments to vertex number and cSteps
int cleanUp(Schedule *s)
{
   for(int j=0;j<s->n_ops;j++)
            {
                //cout<<""<<op_arrange[j]<<"\t";
                PList *temp=s->pOps[j].head;
                while(temp!=NULL)
                {
                    if(temp->cStep!=-1)
                    {
                    temp->cStep+=1;
                    temp->vertex_no+=1;
                    }
                    temp=temp->next;
                }
            }
}
int ListSchedulingUtil(Graph *graph,char *hw_constraints)
{
    //Creates PListMain based on Hardware Constraints
    PListMain *plM=new PListMain(hw_constraints);

    //Computes ASAP,ALAP, & Mobility, integrates them into graph
    asap_alap(graph);

    //Integrates ops & mobility into graph
    //alloc_op_mbty(graph,ops,graph->mob);

    // Creates Schedule based on Hardware constraints
    Schedule *currS=new Schedule(1,hw_constraints);

    //Prints Graph and its Relevant Info
    // printGraph(graph);

    // Schedules based on hardware availablity
    ListScheduling(graph,currS,plM);

    // Converts 0 indexed schedule to 1 indexed schedule
    cleanUp(currS);

    // Prints the Schedule
    currS->toString();

    // Output to Text File
    //createSchDot(currS,0);

return SUCCESS;
}
int HAL_Bench()
{
    string op_arr="";
    /*** Example from notes
        V=11
        E=11
    ***/
    int V=11;
    //int mob[]={0,0,1,2,0,1,0,0,2,2,2};
    char ops[]="******--++<";
    char hw_constraints[]="**-+<";
    Graph* graph = createGraph(MAX_LT);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);
    addEdge(graph, 5, 6);
    addEdge(graph, 2, 5);
    addEdge(graph, 7, 8);
    addEdge(graph, 9, 10);
    graph->V=11;
    alloc_op_mbty(graph,ops);
    ListSchedulingUtil(graph,hw_constraints);
    return 0;
}

int main()
{
    cout<<"\n Main Method \n";
    Graph* graph = createGraph(MAX_LT);

    cout<<"\n Dot File Reading initiated:\n";
    //readFromDot(graph);

   /// Dot File Config
    /*char ops[]="***--***++<";
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);
    addEdge(graph, 5, 6);
    addEdge(graph, 6, 4);
    addEdge(graph, 7, 8);
    addEdge(graph, 9, 10);
    graph->V=11;
    alloc_op_mbty(graph,ops);*/

    char ops[]="*****++++";
    addEdge(graph, 0, 5);
    addEdge(graph, 1, 5);
    addEdge(graph, 2, 6);
    addEdge(graph, 3, 6);
    addEdge(graph, 6, 7);
    addEdge(graph, 7, 8);
    addEdge(graph, 4, 7);
    addEdge(graph, 5, 8);
    graph->V=9;
    alloc_op_mbty(graph,ops);
    //char hw_constraints[]="***-+<";
    char hw_constraints[]="*+";
    /// Print Graph
    //printGraph(graph,1);

    ListSchedulingUtil(graph,hw_constraints);
return 0;
}
