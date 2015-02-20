/***
About this header file :

basic.h

Contains structure definition of
PList ->    PListHead    -> PListMain
PList ->    ScheduleHead -> Schedule

Attributes of each structure in brief :

PList :
op          <char>      stores operation types
*next       <PList>     pointer to the next PList node
mobility    <int>       stores mobility of vertex
vertex_no   <int>       number assigned to the vertex when reading the dfg (1 indexed)
cStep       <int>       stores the control step which it is scheduled

PListHead  :
head       <PList>      reference to head of PList

PListMain  :
M           <int>       Stores no of operation
op_type     <char>      Stores Hardware based constraints
direct      <PListHead> Stores refernces to each of the PListHead

ScheduleHead  :
head       <PList>      reference to head of PList

Schedule :
nCsteps     <int>       Stores no of CSteps for a DFG
n_ops       <int>       Total no of H/W contraints available
pOps    <ScheduleHead>  Reference to Each of ScheduleHead of the operation
op_arrange  <string>    Arrangement of H/W logic

***/
#include<iostream>
#include<stdio.h>
#include<malloc.h>
#include<vector>
#include<string>
#include<stdlib.h>
#include<stack>
#include<cstring>
#include<stdlib.h>
#include<fstream>

enum{OP_PRINT,LIST_PRINT,SCHEDULE_OPS_PRINT};
enum{PRIMARY,SECONDARY};
enum{UN_SCH,SCH};
enum{FAILURE,SUCCESS};

typedef unsigned long long int LL;
#define MAX_LT 500

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
        cout<<vertex_no<<"<"<<mobility<<">";
        return 1;
    }
    //Depreceted
    int toString(int k)
    {
        return 1;
    }
};
typedef struct PListHead
{
    PList *head;
};
typedef struct PListMain
{
    int M; /// No of Unique Operation ----NOT---- no of vertex
    char op_type[MAX_LT];
    PListHead *direct;
    PListMain(char *ops)
    {
        int c=0;
        while(ops[c])
            c++;
        M=c;
        direct=(PListHead *) malloc(M * sizeof(PListHead));
        for(int i=0;i<M;i++)
        {
            //graph->su[i].head = NULL;
            direct[i].head=NULL;
            op_type[i]=ops[i];
            //op_type=ops;
        }
    }

 int toString()
    {
        cout<<"\n\n--------PLIST CONTENTS START------------\n\n";
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
        cout<<"\n--------PLIST CONTENTS END------------\n\n";
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
        n_ops - no of operators (5 for below)
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
        cout<<"\n\n-----------------SCHEDULE BEGIN-----------------\n\n";
        cout<<"VERTEX NUMBER < MOBILITY > CONTROL STEP\n";
        cout<<"\ncSteps: "<<n_cSteps<<"\t nOps: "<<n_ops<<"    "<<""<<op_arrange<<"\n\t";
        cout<<"\n";
        #ifndef SCH_2
            for(int j=0;j<n_ops;j++)
            {
                cout<<op_arrange[j]<<"\t";
                //cout<<"\n";
                PList *temp=pOps[j].head;
               int ctr=1;

                if(temp->cStep!=1)
                {
                        while(ctr<=temp->cStep)
                        {
                        temp->toString(1);
                        //cout<<"  ";
                        ctr+=1;
                        }
                }
                //cout<<ctr<<"\n";
                while(temp!=NULL && temp->cStep>=0)
                {

                    temp->toString();
                        cout<<temp->cStep;
                    cout<<" --> ";
                    temp=temp->next;
                    ctr+=1;
                }
                 cout<<"\n";
            }
        #else
        vector< vector<int> > vtx(MAX_LT);
        vector< char > op(MAX_LT,'E');
         for(int j=0;j<n_ops;j++)
            {
                PList *temp=pOps[j].head;
                while(temp!=NULL && temp->cStep>=0)
                {
                    //cout<<"\n Pushed "<<temp->vertex_no<<" to cStep : "<<temp->cStep<<"\n";
                    vtx[temp->cStep].push_back(temp->vertex_no);
                    op[temp->vertex_no]=temp->op;
                    temp=temp->next;
                }
            }
        for(int i=0;i<n_cSteps;i++)
        {

            cout<<"s"<<i+1<<" :\t";
            for(int j=0;j<vtx[i].size();j++)
            {


                    cout<<vtx[i][j]+1<<" "<<op[vtx[i][j]]<<",";
            }

            cout<<endl;

        }
        #endif // SCH_2

        cout<<"\n-----------------SCHEDULE END-----------------\n\n";
        return 1;
    }
};
typedef struct AdjListNode
{
    int dest;
    AdjListNode* next;
    //char op; // Workaround for Getting input from dot file
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
    int V;                  // No of Vertex
    AdjList* su;            // Successor of Vertex
    AdjList* pre;           // Predecessor of Vertex
    AdjList* pre_non_d;     //  NON destructive predecssor List for Dynamic List Scheduling
    int asap[MAX_LT];       // Position of vertex at ASAP Schecdule
    int alap[MAX_LT];       // Position of vertex at ALAP Schecdule
    int mob[MAX_LT];        // Mobility of Vertices
    char operation[MAX_LT]; // Type of operation associated with the vertex
    int stat[MAX_LT];       // ??? Forgot -- Status of what

};

/// Reads Power from file if defined and stores it in cost vector
int readPower(vector < pair<char,int> > cost)
{
    // Reads only if define
    #ifdef READ_POWER
    ifstream file;	//File Handler
	file.open("cost.txt", ios::in);
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
    #endif // READ_POWER
    return SUCCESS;
}
/*** Allocates mobility to vertices after computation ***/
int alloc_op_mbty(Graph* graph,char *op)
{
    for(int v=0;v<graph->V;++v)
    {

        graph->operation[v]=op[v];
    }
    return 1;
}
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
/** Inserts PListNode k in PList p, using 'x' as cStep parameter **/
PList* insert_PList(PList *p,PList *k,int x)
{
    int t=0;
    if(p->cStep==-1)
    {
        p=k;t=1;
    }
    else if(p->next==NULL)
    {
        if(k->cStep>p->cStep)
        {
            p->next=k;
        }
        else
        {
            k->next=p;
            p=k;
        }
        t=1;
    }
        PList *prev=p;
        PList *curr=p->next;
        /*Code to insert a node by comparing cStep*/
        while(curr!=NULL)
        {
            if(curr->cStep>x)// IMP
            {
                prev->next=k;
                k->next=curr;
                break;
            }
            prev=curr;
            curr=curr->next;

        }
    if(t==0)
    {
        prev->next=k;
    }
    k->cStep=x;
    return p;
}

/** Takes a Schedule , inserts tk at Cstep of funit hardware **/
int schedule_op(Schedule *S,PList *tk,int Cstep,int funit)
{
        int loc=funit;
        if(loc>=0)
        {
            S->pOps[loc].head=insert_PList(S->pOps[loc].head,tk,Cstep);
            return SUCCESS;
        }
    return FAILURE;
}
void addEdge(Graph* graph, int src, int dest)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->su[src].head;
    graph->su[src].head = newNode;

    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src);
    newNode->next = graph->pre[dest].head;
    graph->pre[dest].head = newNode;

    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src);
    newNode->next = graph->pre_non_d[dest].head;
    graph->pre_non_d[dest].head = newNode;
}
int readFromDot(Graph *graph,char *location)
{
    int global_count=0;
    ifstream file;	//File Handler
	file.open(location, ios::in);	//Accessing the input file. It is in the .dot format. *Needs to be changed according to the input path of the file*
	char num1[10];
	char num2[10];
    if(file.is_open())
	{
		string line;
		getline(file, line);
		getline(file, line);
		while(!file.eof())
		{
			getline(file, line);

			for(LL i=0; i<line.length(); i++)
			{
				if(line[i] == '=' && line[i+1] == ' ')	//Condition for checking and accessing the lines containing the node information of the graphs.
				{
					// Reading Vertex Number
					char num[10];
					int k=0;
					for(LL j = 0; j<line.length(); j++)
					{
						if(int(line[j])>=48 && int(line[j])<=57)
						{
							num[k]=line[j];
							k++;
							if(line[j+1]==' ')
								break;
						}
					}
					int v_no=atoi(num)-1;	//Adding the node pointer to an array, for access later while defining links.
					string str="";
					str=(line[i+2]);
					str+=(line[i+3]);
					str+=(line[i+4]);
					if(str.compare("mul")==0||str.compare("MUL")==0||line[i+3]=='*')
                        graph->operation[v_no]='*';
                    else if(str.compare("sub")==0||str.compare("SUB")==0||line[i+3]=='-')
                        graph->operation[v_no]='-';
                    else if(str.compare("add")==0||str.compare("ADD")==0||line[i+3]=='+')
                        graph->operation[v_no]='+';
                    else if(str.compare("les")==0||str.compare("LES")==0||line[i+3]=='<')
                        graph->operation[v_no]='<';
                    else if(str.compare("gre")==0||str.compare("GRE")==0||line[i+3]=='>')
                        graph->operation[v_no]='>';
                    else if(str.compare("div")==0||str.compare("DIV")==0||line[i+3]=='/')
                        graph->operation[v_no]='/';
                    else if(str.compare("exp")==0||str.compare("EXP")==0||line[i+3]=='^')
                        graph->operation[v_no]='^';
                    else if(str.compare("imp")==0||str.compare("IMP")==0||line[i+3]=='!')
                        graph->operation[v_no]='!';
                    // Add custom operation here
					global_count++;

				}
				else if(line[i] == '-' && line[i+1] == '>')	//Condition for accessing the lines where links are defined.
				{
					int k=0,l=0;
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

					int x=0;
					for(LL y = l; y<line.length(); y++)
					{
						if(int(line[y])>=48 && int(line[y])<=57)
						{
							num2[x]=line[y];
							x++;
							if(line[y+1] == ' ')
								break;
						}
					}
					cout<<"\nAdding edge between "<<(atoi(num1)-1)<<" and "<<(atoi(num2)-1)<<"\n";
                    addEdge(graph,(atoi(num1)-1),(atoi(num2)-1));
				}
			}

		}  // While Loop for file reading
	}
	file.close();
	graph->V=global_count;
	cout<<"\n No of Vertex : "<<global_count<<endl;
	return 1;
}
/** Checks if Schedule has vertex v **/
int sHasV(Schedule *s,int v)
{
    //s->toString();
    for(int i=0;i<s->n_ops;i++)
    {
    PList *pC=s->pOps[i].head;
    while(pC)
    {
        if(v==pC->vertex_no)
            return SUCCESS;
        pC=pC->next;
    }
    }
    return FAILURE;
}
/*** Should Check if Predecessors are scheduled ***/
int checkPred(Graph *graph,int v,Schedule *s)
{
    //return true for empty predecessors
     AdjListNode* p = graph->pre_non_d[v].head;
     if(p==NULL)
     {
        return SUCCESS;
     }
     while(p)
        {
            if(sHasV(s,p->dest)==FAILURE)
                {
                return FAILURE;
                }
        p=p->next;
        }
    return SUCCESS;
}


