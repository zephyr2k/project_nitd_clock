/***
TODO:                                       STATUS


Note : internally all vertex are 0 indexed, along with their control steps, however when displayed ,they are 1 indexed
***/

#include<iostream>
#include<malloc.h>
#include<vector>
#include<string>
#include<stdlib.h>
#include<algorithm>
#include<stack>
#include<fstream>
#undef DEBUG

#define MAX_LT 100

// STD definition for program
#include "basic.h"
using namespace std;


/// PList Node Structure for Scheduling
///

typedef struct sTable
{
    int N;
    int vertex_no[MAX_LT];
    int priority[MAX_LT];
    int asap[MAX_LT];
    int alap[MAX_LT];
    char operation[MAX_LT];
    //int status[MAX_LT];
    sTable(int n)
    {
        N=n;
        for(int i=0;i<N;i++)
        {
            vertex_no[i]=-1;
            priority[i]=-1;
            asap[i]=-1;
            alap[i]=-1;
            operation[i]='-';
            //status[i]=UN_SCH;
        }
    }
    int toString()
    {
        //1 - Indexing
        cout<< "\n\n-------------Table Printing----------------";
        cout<<"\n\tVertex No <ASAP,ALAP,OP> -- Priority \n";
        for(int i=0;i<N;i++)
        {
            cout<<vertex_no[i]+1<<"< "<<asap[i]<<", "<<alap[i]<<", "<<operation[i]<<" > -- "<<priority[i]<<endl;
        }
        cout<< "\n-------------Table Printing----------------\n";
    }
};


// A utility function to print the adjacency list representation of graph
void printGraph(Graph* graph,int i=0)
{
    int v;
    cout<< "\n\n-------------Graph Printing----------------\n";
if(i==0)
    {
    for (v = 0; v < graph->V; ++v)
    {
        if(graph->stat[v]!=0)
        cout<<v<<": "<< graph->mob[v] <<" "<<graph->operation[v]<<"\n";
    }
    }
else if(i==1)
    {
    for (v = 0; v < graph->V; ++v)
    {
         if(graph->stat[v]!=0)
         {
        AdjListNode* pCrawl = graph->su[v].head;
        printf("\n Successor List of vertex %d: head ", v+1);
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
        cout<<"\n"<<v+1<<" assigned operation : "<<graph->operation[v];
    }
    cout<< "\n-------------Graph Printing---------------\n\n";
}

//ASAP ,ALAP Mobility Calculation - ADITYA's work
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
}
m=0;
for(int i=0;i<graph->V;i++)
{
    if(m<al[i])
    m=al[i];
}
for(int i=0;i<graph->V;i++)
{
	al[i]=abs(al[i]-m)+1;
	//printf("----alap-----%d %d\n",i+1,al[i]);
}

for(int i=0;i<graph->V;i++)
{
    graph->mob[i]=abs(al[i]-as[i]);
    graph->asap[i]=as[i];
    graph->alap[i]=al[i];
    #ifdef DEBUG_
        printf(" %d < %d >\n",i+1,abs(al[i]-as[i]));
    #endif // DEBUG_
}
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
// Merge Sort Optimized for sTable strucutre
void merge(sTable *table, int l, int m, int r,int TYPE)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int L[n1], R[n2];
    sTable *LL=new sTable(n1);
    sTable *RR=new sTable(n2);

    /* Copy data to temp arrays L[] and R[] */
    for(i = 0; i < n1; i++)
    {
        //L[i] = arr[l + i];
        LL->alap[i]=table->alap[l+i];
        LL->asap[i]=table->asap[l+i];
        LL->operation[i]=table->operation[l+i];
        LL->vertex_no[i]=table->vertex_no[l+i];
    }
    for(j = 0; j < n2; j++)
    {
        //R[j] = arr[m + 1+ j];
        RR->alap[j]=table->alap[m + 1+ j];
        RR->asap[j]=table->asap[m + 1+ j];
        RR->operation[j]=table->operation[m + 1+ j];
        RR->vertex_no[j]=table->vertex_no[m + 1+ j];

    }

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        // PRIMARY - Ascending , other = Descending
        if(TYPE==PRIMARY)
        {
            if (LL->alap[i] <= RR->alap[j])
            {
                //arr[k] = L[i];
                table->alap[k]=LL->alap[i];
                table->asap[k]=LL->asap[i];
                table->operation[k]=LL->operation[i];
                table->vertex_no[k]=LL->vertex_no[i];
                i++;
            }
            else
            {
                //arr[k] = R[j];
                table->alap[k]=RR->alap[j];
                table->asap[k]=RR->asap[j];
                table->operation[k]=RR->operation[j];
                table->vertex_no[k]=RR->vertex_no[j];
                j++;
            }
            k++;
        }
        else
        {
            if (LL->asap[i] >= RR->asap[j])
            {
                //arr[k] = L[i];
                table->alap[k]=LL->alap[i];
                table->asap[k]=LL->asap[i];
                table->operation[k]=LL->operation[i];
                table->vertex_no[k]=LL->vertex_no[i];
                i++;
            }
            else
            {
                //arr[k] = R[j];
                table->alap[k]=RR->alap[j];
                table->asap[k]=RR->asap[j];
                table->operation[k]=RR->operation[j];
                table->vertex_no[k]=RR->vertex_no[j];
                j++;
            }
            k++;
        }
    }

    /* Copy the remaining elements of L[], if there are any */
    while (i < n1)
    {
        //arr[k] = L[i];
        table->alap[k]=LL->alap[i];
        table->asap[k]=LL->asap[i];
        table->operation[k]=LL->operation[i];
        table->vertex_no[k]=LL->vertex_no[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there are any */
    while (j < n2)
    {
        //arr[k] = R[j];
        table->alap[k]=RR->alap[j];
        table->asap[k]=RR->asap[j];
        table->operation[k]=RR->operation[j];
        table->vertex_no[k]=RR->vertex_no[j];
        j++;
        k++;
    }
}
void mergeSort(sTable *table, int l, int r,int TYPE)
{
    if (l < r)
    {
        int m = l+(r-l)/2; //Same as (l+r)/2, but avoids overflow for large l and h
        mergeSort(table, l, m,TYPE);
        mergeSort(table, m+1, r,TYPE);
        merge(table, l, m, r,TYPE);
    }
}

// Transfers the relvant information from DFG -> Static Table for further work like sorting and scheduling
int createTable(Graph *graph,sTable *table)
{
   // table->toString();
   //vector < pair<int,int> > pi;
   for(int i=0;i<graph->V;i++)
   {
        //pi[i].first=graph->alap[i];
        //pi[i].second=graph->asap[i];
        table->vertex_no[i]=i;
        table->alap[i]=graph->alap[i];
        table->asap[i]=graph->asap[i];
        table->operation[i]=graph->operation[i];
   }
   #ifdef DEBUG
        table->toString();
   #endif
   mergeSort(table,0,table->N-1,PRIMARY);
   vector <int> V;
   int prev=0;
   V.push_back(0);
   for(int i=1;i<graph->V;i++)
   {
        if(table->alap[i]!=table->alap[i-1])
            V.push_back(i);
   }
   for(vector<int>::const_iterator i=V.begin()+1;i!=V.end();i++)
   {
        cout<<*i<<endl;
        mergeSort(table,prev,(*i)-1,SECONDARY);
        prev=*i;
   }
   for(int i=0;i<graph->V;i++)
   {
        table->priority[i]=i+1;
   }
   #ifdef DEBUG
        table->toString();
   #endif
return 0;

}
// Schedule Printing
void createSchDot(Schedule *sch,int param)
{
    FILE *fp;
    char ch;
    fp=fopen("StaticLS_Schedule.txt","w");
    fprintf(fp,"// Schedule Format : Vertex Number < Mobility > Control Step \n");

    //fprintf(fp, "digraph "" {\n");
    for (int v = 0; v < sch->n_ops; ++v)
    {
        fprintf(fp,"%c : \t ",sch->op_arrange[v]);
        PList* pCrawl = sch->pOps[v].head;
        while (pCrawl)
        {
            fprintf(fp, "%d ( %d ) %d -> ",pCrawl->vertex_no+1,pCrawl->mobility,pCrawl->cStep+1);
            pCrawl = pCrawl->next;
        }
        fprintf(fp,"\n");
    }
    fprintf(fp, "");
    fclose(fp);
}
// Sort the table in descending order of priority
int reverseTable(sTable *table)
{

}
///
/// CORE SCHEDULING
///
int staticListScheduling(sTable *table,Schedule *sch)
{
    int vSch=0,cStep=0,cur_op_sch=0,i=1;
    //cout<< sch->n_ops;
    int op_Sch[sch->n_ops];
    //cout<<'\n';
    // Reset them to 0
    for(int j=0;j<sch->n_ops;j++)
        { op_Sch[j]=0;}
    for(int i=table->N-1;i>=0;i--)// Each Vertex
    {
        //cout<<table->vertex_no[i]+1<<":"<<table->operation[i]<<'\t';
       //char c_op ='*';
       char c_op = table->operation[i];
      // now for each hardware logic
       for(int j=0;j<sch->n_ops;j++)
       {
          // cout<<sch->op_arrange[j]<<"\t";
            if(c_op == sch->op_arrange[j] && op_Sch[j]==0) //HW found and no op is scheduled, then schedule it
            {
                // Set isOpSch
                op_Sch[j]=1;
                //Add this vertex in the schedule
                    PList *tr=new PList(c_op);
                        tr->mobility=(abs)(table->alap[i]-table->asap[i]);
                        tr->vertex_no=table->vertex_no[i];
                        tr->cStep=cStep;
                        if(schedule_op(sch,tr,cStep,j)==1)
                        {
                            cout<<"\n Scheduling Vertex "<<(tr->vertex_no)+1<<" on cStep "<<cStep+1<<" of "<<c_op<<" operator.";
                            cur_op_sch=1;
                            break;// Breaks searching for rest of h/w logic
                        }
                       // else cur_op_sch=0;
               // cout<<"\n Scheduled vertex "<<tr->vertex_no<<" at "<<cStep;
            }

       }
       cout<<endl;
       if(cur_op_sch==0) //Curr op could not find any free hardware logic
       {
            //increment cstep , and reset cur_op_sch & vector array
            cStep+=1;
            for(int j=0;j<sch->n_ops;j++)
            { op_Sch[j]=0;}

            i++;
        }
        cur_op_sch=0;
    }
    cout<<'\n';

return 0;
}
int staticListSchedulingUtil(Graph *graph,char *hw_constraints)
{
    // Static Table initialization
    sTable *table=new sTable(graph->V);

    //Creates PListMain based on Hardware Constraints
    //PListMain *plM=new PListMain(hw_constraints);

    //Computes ASAP,ALAP, & Mobility, integrates them into graph
    asap_alap(graph);
    //Integrates ops & mobility into graph
    //alloc_op_mbty(graph,ops,graph->mob);
    // Creates Schedule based on Hardware constraints
    Schedule *currS=new Schedule(1,hw_constraints);
    //Prints Graph and its Relevant Info
    printGraph(graph,LIST_PRINT);

    // Create Static Table for Scheduling
    createTable(graph,table);

    // Reverse the static table for better conveinience
    //reverseTable(table);
    // Schedules based on hardware availablity
    staticListScheduling(table,currS);
    // Converts 0 indexed schedule to 1 indexed schedule

    // Prints the Schedule
    createSchDot(currS,0);

    cleanUp(currS);

    currS->toString();

    // Writes schedule to file

return SUCCESS;
}
int main()
{
    cout<<"\n";
    string op_arr="";
    /*** Example from notes
        V=11
        E=11
    ***/
    int V=11;
    //int mob[]={0,0,1,2,0,1,0,0,2,2,2};
    //char ops[]="******--++<";
    char hw_constraints[]="**-+<";
    Graph* graph = createGraph(MAX_LT);
    /*addEdge(graph, 0, 4);
    addEdge(graph, 1, 4);
    addEdge(graph, 4, 6);
    addEdge(graph, 6, 7);
    addEdge(graph, 5, 7);
    addEdge(graph, 2, 5);
    addEdge(graph, 3, 8);
    addEdge(graph, 9, 10);
    */
    readFromDot(graph);

    staticListSchedulingUtil(graph,hw_constraints);

    cout<<"\n";
return 0;
}
