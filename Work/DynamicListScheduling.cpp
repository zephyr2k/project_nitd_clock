/***
TODO:                                       STATUS

    Implement basic function:
        insert_ready_ops                    DONE
        schedule_op                         DONE
        delete                              DONE
        first                               DONE
    Implement List Scheduling               DONE
    Implement Schedule Structure            DONE, V2(FEATURES MAY BE ADDED)
    Implement PList                         DONE, (FEATURES MAY BE ADDED)
    Graph Linking                           Done

** Requires reading schedule from Graph with ASAP , ALAP & Mobility of each vertex already computed.
    ---> Function added to compute mobility,asap,alap.
BUGS
    1. Inadequate hardware pushes other operation in that Control Step to below it - FIXED (It ignores the new schedule operation)

***/

#define DEBUG
#define SCH_2
#define READ_POWER
#include<iostream>

/// Contains Structure definition of PList, Schedule ,etc along with some basic function
#include "basic.h"
using namespace std;


/** Deletes references to predecessor node 'toDel' from successor list **/
Graph* delete_Succ(Graph *graph,int fromDel,int toDel)
{
    AdjListNode *curr=graph->pre[fromDel].head;
    AdjListNode *prev=curr;
    if(curr==NULL)
        {
        curr=NULL;

        }
    else if(curr->dest==toDel)
        {
        /// Not Working
        /// FIXED
        graph->pre[fromDel].head=graph->pre[fromDel].head->next;
        }
    else
    {
    prev=curr;
    curr=curr->next;

    while(curr!=NULL)
    {
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

    vector<int> visited(graph->V,0);
    for(int v=0;v<graph->V;v++)
    {
        //checks for NULL ref
        // check if adjNodes of v are schedules in 's' or not

        if(visited[v]!=1 && graph->stat[v]!=0 && checkPred(graph,v,s))
        {

            PList *t=new PList(graph->operation[v]);
            t->vertex_no=v;
            t->mobility=graph->mob[v];
            // current node to be deleted
            //goto successor of v delete all ref to v
            AdjListNode *p=graph->su[v].head;
            while(p!=NULL)
            {

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

                    if(plMain->direct[i].head==NULL)
                    {
                        plMain->direct[i].head=t;
                        break;
                    }
                    int c=0;
                    PList *pCrawl=plMain->direct[i].head;
                    if(t->mobility<=pCrawl->mobility)
                    {

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
        cout<<"\n"<<v+1<<" assigned operation : "<<graph->operation[v];
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
/*
TRUE - Remains Inside
FALSE - BREAKS OUT
*/
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
void createSchDot(Schedule *sch,char *param,char *fname)
{
    FILE *fp;
    char ch;
    fp=fopen(fname,"w");
    #ifndef SCH_2
    fprintf(fp,"// Schedule Format : Vertex Number < Mobility > Control Step ");
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
    #else
    /// Edit Start
    fprintf(fp,"Schedule Format :\nControl Step :  Vertex Number (Type of Operation)");
    fprintf(fp,"\n**************************************\n");
    fprintf(fp,"%s \t Hardware Constraints : ",param);
    for(int i=0;i<sch->n_ops;i++)
        fprintf(fp,"%c",sch->op_arrange[i]);
    fprintf(fp,"\n**************************************\n\n");
    vector< vector<int> > vtx(MAX_LT);
    vector< char > op(MAX_LT,'E');
         for(int j=0;j<sch->n_ops;j++)
            {
                PList *temp=sch->pOps[j].head;
                while(temp!=NULL && temp->cStep>=0)
                {
                    //cout<<"\n Pushed "<<temp->vertex_no<<" to cStep : "<<temp->cStep<<"\n";
                    vtx[temp->cStep].push_back(temp->vertex_no);
                    op[temp->vertex_no]=temp->op;
                    temp=temp->next;
                }
            }
        for(int i=0;i<sch->n_cSteps;i++)
        {
            fprintf(fp, "s%d :\t",i+1);
            for(int j=0;j<vtx[i].size();j++)
            {
                fprintf(fp, "%d (%c), ",vtx[i][j]+1,op[vtx[i][j]]);
            }
            fprintf(fp, "\n");
        }
    /// Edit End
    #endif // SCH_2
    fclose(fp);
    cout<<"\n Created Output for "<<param<<" with Hardware constraints ";
    for(int i=0;i<sch->n_ops;i++)
        printf("%c",sch->op_arrange[i]);
    cout<<" at Location : "<<fname<<"\n";
}
int asap_alap(Graph *graph)
{
int t;
vector<int> as(MAX_LT,0),al(MAX_LT,0);
stack<int> f;
int te[100];
int s=0;
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
    insert_ready_ops(graph,sch,plMain);
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
            if(temp) //  Goes inside IFF non-null
            {
                for(int funit=0;funit<(sch->n_ops);funit++)
                {
                    temp=plMain->direct[k].head;
                    if((temp!=NULL && temp->op==sch->op_arrange[funit])||(temp!=NULL && sch->op_arrange[funit]=='A'))
                    {
                        #ifdef DEBUG
                        printf("To Schedule '%c' %d<%d> at step %d\n",temp->op,temp->vertex_no+1,temp->mobility,CStep+1);
                        #endif // DEBUG_

                        PList *tr=new PList(sch->op_arrange[funit]);
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
    return SUCCESS;
}

// Adjustments to vertex number and cSteps
int cleanUp(Schedule *s)
{
   for(int j=0;j<s->n_ops;j++)
            {
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
/// Displays Schedule taking Power into account
int disp_Power(Schedule *sch,char *fname)
{
    vector < pair<char,int> > cost;
        // Reads only if define

    ifstream file;	//File Handler
	file.open(fname, ios::in);
	char num1[10];
    if(file.is_open())
	{
		string line;
		//getline(file, line);
		//getline(file, line);
		while(!file.eof())
		{
		    getline(file, line);
		    if(line[0]!='#')
            {

                int k=0,l=0;
                // Read Value and store in num1
					for(LL j = 0; j<line.length(); j++)
					{
						if(int(line[j])>=48 && int(line[j])<=57)
						{
							num1[k]=line[j];
							k++;
							if(line[j+1] == ' ')
							{
								l=j+1;
								break;
							}
						}
					}
					cost.push_back(make_pair(line[0],atoi(num1)));
            }
		}
	}
	file.close();
	#ifdef DEBUG
	cout<<"\n-----Values of Each operator Read from file------\n";
	for(int i=0;i<cost.size();i++)
    {
        cout<<"\n"<<cost[i].first<<"\t"<<cost[i].second;
    }
    cout<<"\n\n------------------------------------------------\n\n";
    #endif // DEBUG
        vector< vector<int> > vtx(MAX_LT);
        vector< char > op(MAX_LT,'E');
         for(int j=0;j<sch->n_ops;j++)
            {
                PList *temp=sch->pOps[j].head;
                while(temp!=NULL && temp->cStep>=0)
                {
                    //cout<<"\n Pushed "<<temp->vertex_no<<" to cStep : "<<temp->cStep<<"\n";
                    vtx[temp->cStep].push_back(temp->vertex_no);
                    op[temp->vertex_no]=temp->op;
                    temp=temp->next;
                }
            }
        cout<<"\n\n---------------- FINAL SCHEDULE BEGIN ----------------\n\n";
        cout<<"VERTEX NUMBER < MOBILITY > CONTROL STEP\n";
        cout<<"\ncSteps: "<<sch->n_cSteps<<"\t nOps: "<<sch->n_ops<<"    "<<""<<sch->op_arrange<<"\n\t";
        cout<<"\n";
        for(int i=0;i<sch->n_cSteps;i++)
        {
            int power=0;
            cout<<"s"<<i+1<<" :\t";
            for(int j=0;j<vtx[i].size();j++)
            {

                for(int k=0;k<cost.size();k++)
                {

                    if(cost[k].first==op[vtx[i][j]])
                    {
                       // cout<<"\n"<<cost[k].first<<"\n";
                        power+=cost[k].second;
                        break;
                    }
                }
                    cout<<vtx[i][j]+1<<" "<<op[vtx[i][j]]<<",";
            }
            cout<<right;
            cout<<setw(20)<<power<<"\n";

        }
        cout<<"\n----------------- FINAL SCHEDULE END -----------------\n\n";
    return 1;
}
/// ListScheduling Utility that takes as input the DFG, Hardware Constraints, and the Namr of the Benchmarks (optional)
int ListSchedulingUtil(Graph *graph,char *hw_constraints,char *type,char *fname,char* power_cfg)
{
    // Print Graph
    printGraph(graph,1);

    //Creates PListMain based on Hardware Constraints
    PListMain *plM=new PListMain(hw_constraints);

    //Computes ASAP,ALAP, & Mobility, integrates them into graph
    asap_alap(graph);

    //Integrates ops & mobility into graph
    //alloc_op_mbty(graph,ops,graph->mob);

    // Creates Schedule based on Hardware constraints
    Schedule *currS=new Schedule(1,hw_constraints);

    // Schedules based on hardware availablity
    ListScheduling(graph,currS,plM);

    // Prints the final Schedule
    #ifndef READ_POWER
        currS->toString();
    #else
        disp_Power(currS,power_cfg);
    #endif


    // Converts 0 indexed schedule to 1 indexed schedule
    //cleanUp(currS);

    // Output to Text File
    createSchDot(currS,type,fname);


return SUCCESS;
}
int HAL_Util()
{
    Graph* graph = createGraph(MAX_LT);
    // Location of the dot File
    char *location="../benchmark/hal.dot";
    // Name of Benchmark
    char *type="HAL";
    // Expected output for the resultant Schedule
    char *fname="../Output/HAL_DynamicLS.txt";

    //Expected location to read Power Config
    char *power_cfg="cost.txt";

    readFromDot(graph,location);

    // Set HW constraints for benchmarks
    char hw_constraints[]="**+-<";

    //Call the Appropriate Scheduling Utility
    ListSchedulingUtil(graph,hw_constraints,type,fname,power_cfg);
}
int IIR_Util()
{
    Graph* graph = createGraph(MAX_LT);
    // Location of the dot File
    char *location="../benchmark/iir2.dot";
    // Name of Benchmark
    char *type="IIR 2";
    // Expected output for the resultant Schedule
    char *fname="../Output/IIR_DynamicLS.txt";

    //Expected location to read Power Config
    char *power_cfg="cost.txt";

    readFromDot(graph,location);

    // Set HW constraints for benchmarks
    char hw_constraints[]="**+A";

    //Call the Appropriate Scheduling Utility
    ListSchedulingUtil(graph,hw_constraints,type,fname,power_cfg);
}
int ARF1_Util()
{
    Graph* graph = createGraph(MAX_LT);
    // Location of the dot File
    char *location="../benchmark/arf1.dot";
    // Name of Benchmark
    char *type="ARF 1";
    // Expected location of output file for the resultant Schedule
    char *fname="../Output/ARF1_DynamicLS.txt";

    //Expected location to read Power Config
    char *power_cfg="cost.txt";

    readFromDot(graph,location);

    // Set HW constraints for benchmarks
    char hw_constraints[]="**+A";

    //Call the Appropriate Scheduling Utility
    ListSchedulingUtil(graph,hw_constraints,type,fname,power_cfg);
}
int FIR_Util()
{
    Graph* graph = createGraph(MAX_LT);
    // Location of the dot File
    char *location="../benchmark/fir2.dot";
    // Name of Benchmark
    char *type="FIR 2";
    // Expected output for the resultant Schedule
    char *fname="../Output/FIR2_DynamicLS.txt";

    //Expected location to read Power Config
    char *power_cfg="cost.txt";

    readFromDot(graph,location);

    // Set HW constraints for benchmarks
    char hw_constraints[]="**++";

    //Call the Appropriate Scheduling Utility
    ListSchedulingUtil(graph,hw_constraints,type,fname,power_cfg);
}
int COSINE2_Util()
{
    Graph* graph = createGraph(MAX_LT);
    // Location of the dot File
    char *location="../benchmark/cosine2.dot";
    // Name of Benchmark
    char *type="FIR 2";
    // Expected output for the resultant Schedule
    char *fname="../Output/cosine2_DynamicLS.txt";

    //Expected location to read Power Config
    char *power_cfg="cost.txt";

    readFromDot(graph,location);

    // Set HW constraints for benchmarks
    char hw_constraints[]="**++--/!!!";

    //Call the Appropriate Scheduling Utility
    ListSchedulingUtil(graph,hw_constraints,type,fname,power_cfg);
}
int EWF_Util()
{
    Graph* graph = createGraph(MAX_LT);
    // Location of the dot File
    char *location="../benchmark/ewf.dot";
    // Name of Benchmark
    char *type="EWF";
    // Expected output for the resultant Schedule
    char *fname="../Output/EWF_DynamicLS.txt";

    //Expected location to read Power Config
    char *power_cfg="cost.txt";

    readFromDot(graph,location);

    // Set HW constraints for benchmarks
    char hw_constraints[]="**++";

    //Call the Appropriate Scheduling Utility
    ListSchedulingUtil(graph,hw_constraints,type,fname,power_cfg);
}
int main()
{
    cout<<"\n Main Method Begin\n";
    /// Implement cost

    //HAL_Util();
    //IIR_Util();
    ARF1_Util();
    //FIR_Util();
    //COSINE2_Util();
    //EWF_Util();


    /**
    Above are predefined Util for each benchmark. In order to use custom dot file with unique hardware constraints. Refer to Each Utit
    For Support Required lines are commented out.

    // Graph definition
    Graph* graph = createGraph(MAX_LT);

    // Location of the dot File
    char *location="../benchmark/fir2.dot";

    // Name of Benchmark
    char *type="FIR 2";

    // Expected output for the resultant Schedule
    char *fname="../Output/FIR2_DynamicLS.txt";

    //Expected location to read Power Config
    char *power_cfg="cost.txt";

    readFromDot(graph,location);

    // Set HW constraints for benchmarks
    char hw_constraints[]="**+++";

    //Call the Appropriate Scheduling Utility
    ListSchedulingUtil(graph,hw_constraints,type,fname);

    **/

    cout<<"\n Main Method End\n";
return 0;
}
