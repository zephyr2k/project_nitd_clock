

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stack>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <map>
#include <list>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#define SWAP(a, b)  a ^= b; b ^= a; a ^= b;
#define MAXVALUE 1000
using namespace std;
    int as[MAXVALUE];//TO STORE ASAP SCHHEDULE
    int as1[MAXVALUE];
    int al[MAXVALUE];//TO STORE ALAP SCHHEDULE
    int V;//TO STROE NUMBER OF VERICES
    int tem;
    int prio[MAXVALUE];//TO STORE PRIORITY OF EACH OPERATION
    int node[MAXVALUE];//TO STORE PRIORITY LIST
    vector<int> stl;
    map<int,int> mymap;
    map<string,int> res;//TO STORE RESOSORCE CONSTRAINT
    map<string,int> res1;
    map<string,int> pow;//TO STORE POWER OF EACH RESOURCE
    int lbs[1000];

//STORE EACH NODE OF GRAPH
struct AdjListNode
{
    int dest;
    struct AdjListNode* next;
};
//STORE AJACENT NODES
struct AdjList
{
    struct AdjListNode *head;
};

//STORE GRAPH AS LIST
struct Graph
{
    int V;
    struct AdjList* su;//STORE SUCESSOR LIST
    struct AdjList* pre;//STORE PREDECESSOR LIST
    int mob[1000];

};


struct AdjListNode* newAdjListNode(int dest)
{
    struct AdjListNode* newNode =(struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

//CREATE GRAPH
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
//CALCULATE THHE LIST BASED SCHEDULE
void dfg(struct Graph* graph,int V,string ops[])
{// TO FIND LIST BASED SCHEDULE
   int i,m=0,j,c,d,e,v,f;
    vector<int>::size_type sz;
    stack<int> st;
    //TO INITIALIZE LIST BASED SCHEDULE TO ZERO
    for(e=0;e<V;e++)
        lbs[e]=0;
        //TO INSERT ALL NODES IN A STACK
    for(j=0;j<V;j++ )
        {

            stl.push_back(j+1);
        }



        sz=stl.size();
        i=0;
    //TO CALCULATE LIST BASED SCHEDULE
        while(!stl.empty())
        {
            i++;
            //TO REINITIALIZE THE COUNTER OF EACH RESOUCSE CONSTRAINT AT THE STARTING OF CONTROL STEP
            res1.clear();
            res1.insert(res.begin(), res.end());

            sz=stl.size();
            // TO SORT THE NODES IN THE STACK ACCORDING TO PRIORITY
            for (c = 0 ; c < ( stl.size()  ); c++)
                {
                    for (d = 0 ; d < stl.size()- c - 1; d++)
                    {
                        if (mymap[stl[d]] < mymap[stl[d+1]])
                        {
                            swap(stl[d],stl[d+1]);
                        }

                    }
                }
//TO CALCULATE THE SIZE OF THE STACK STL
                sz=stl.size();
        f=0;
        for (c = 0 ; c < sz; c++)
        {//CHECK WHETHER THE OPERATION CAN BE SCHEDULED IN THE CONTROL STEP
            if(res1[ops[stl[c]-1]]>0)
                {
                    f=0;
                    struct AdjListNode* pCrawl = graph->pre[stl[c]-1].head;
//TO CHECK WHETHER ALL THE PREDECESOR TO THE NODE HAS BEEN SCHEDULED
                    while (pCrawl)
                    {
                        if(lbs[pCrawl->dest]==0||lbs[pCrawl->dest]==i)
                        {
                            f=1;
                        }
                        pCrawl = pCrawl->next;
                    }
//IF ALL THE PREDECESSOR ARE SCHEDULED AND
                    if(f==0)
                        {
                            lbs[stl[c]-1]=i;
                            res1[ops[stl[c]-1]]--;
                            //stl[c]=10000;
                            st.push(c);

                        }
                }

        }
        /*for (c = 0 ; c < stl.size(); c++)
        {
            if(stl[c]==10000)
                stl.erase(stl.begin()+c);

        }*/
        while(!st.empty())
        {
            stl.erase(stl.begin()+st.top());
            st.pop();

        }



        }

    m=0;
    cout<<"\n LIST BASED SCHEDULE\n\n";
    for(e=0;e<V;e++)
    {
        if(lbs[e]>m)
        m=lbs[e];
    }
    c=0;int f4=0,z=0;
    for(e=1;e<=m;e++)
    {
    cout<<"\ns"<<e<<"     ";
    for(i=0;i<graph->V;i++)
    {
    if(lbs[i]==e)
    {
    cout<<i+1<<"  ";
    }
    }
    }
    cout<<"\nnode lbs\n";
    for(e=0;e<V;e++)
    {
        cout<<"\n"<<e+1<<"  "<<lbs[e];
        if(lbs[e]>m)
        m=lbs[e];
    }
}
//to find priority array
void priority(struct Graph* graph)
{
    int c, d, t,v;
    for(v = 0; v < graph->V; ++v)
    {
        node[v]=v+1;
    }
    for (c = 0 ; c < ( graph->V - 1 ); c++)
    {
        for (d = 0 ; d < graph->V - c - 1; d++)
        {
            if (al[d] > al[d+1])
            {
        /* Swapping */
 SWAP(node[d],node[d+1]);
  SWAP(al[d],al[d+1]);
SWAP(as[d],as[d+1]);
            }
            else if(al[d] == al[d+1])
            {
                if(as[d]<as[d+1])
                    { SWAP(node[d],node[d+1]);
                    SWAP(al[d],al[d+1]);
                    SWAP(as[d],as[d+1]);
                    }
            }
    }
  }

    for (c = 0 ; c < graph->V; c++)
        prio[node[c]]=c+1;
    cout<<"\nnode "<<"alap "<<"asap "<<"priority"<<endl;

    for (c = 0 ; c < graph->V; c++){
        cout<<node[c]<<"    "<<al[c]<<"    "<<as[c]<<"    "<<prio[node[c]]<<endl;
    mymap[node[c]]=prio[node[c]];
    }
}
//ADD EDGES TO THHE GRAPH
void addEdge(struct Graph* graph, int src, int dest)
{

    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->su[src].head;
    graph->su[src].head = newNode;


    newNode = newAdjListNode(src);
    newNode->next = graph->pre[dest].head;
    graph->pre[dest].head = newNode;
}

//TO PRINT GRAPH PRODUCED
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
//TO FIND ASAP AND ALAP SCHEDULE
void mobility(struct Graph* graph,int V)
{
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
while(!f.empty())
{
    struct AdjListNode* pCrawl = graph->su[f.top()].head;
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
for(i=0;i<V;i++)
{
   printf("----asap-----%d %d\n",i+1,as[i]);
}
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
    for(i=0;i<graph->V;i++){if(al[i]!=0 && m>al[i])
    m=al[i];




    }
    al[0]=1000;
   struct AdjListNode* pCrawl = graph->su[0].head;
    if(pCrawl->dest)
        al[0]=al[pCrawl->dest]-1;
    m=10000;
    for(i=0;i<graph->V;i++)
    {
        if(m>al[i])
            m=al[i];
    }
    tem=0;
for(i=0;i<graph->V;i++)
    {
    al[i]=al[i]-m+1;
    if(tem<al[i])
    tem=al[i];
    }

for(i=0;i<graph->V;i++)
    {
    printf("----alap-----%d %d\n",i+1,al[i]=tem-al[i]+1);
    }
for(i=0;i<graph->V;i++)
    {
graph->mob[i]=abs(al[i]-as[i]);
printf("mobility of -----%d ---- %d\n",i+1,abs(al[i]-as[i]));
    }
for(i=0;i<graph->V;i++)
    {
as1[i]=as[i];
    }
}
//TO CALCULATE TIME
double get_time()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    double d = t.tv_sec + (double) t.tv_usec/100000;
    return d;
}



int main()
{
    int t;
    stack<int> f;


    int V=0;
    int te[MAXVALUE];
    int s=0;
    int a=0,b,e=0,i,v,h;
//FILES FOR WHICH LIST BASED SCHEDULE IS TO BE FOUND
    int stsize,z;
    cout<<"Enter the no. of dot files ";
    cin>>stsize;

    string l[stsize];


    string s1;

    for(h=0;h<stsize;h++)
        {

                cout<<"\nEnter the name of dot files\n";
                cin>>l[h];

                fstream file(l[h].c_str());
            if(!file.good())
            {
                cout<<"\nEnter the name of dot files\n";
                cin>>l[h];

                fstream file(l[h].c_str());
            }



            cout<<"\nEnter no. of resource constraint\n";
            cin>>v;
            cout<<"\nEnter length of string of each resource constraint\n";
            cin>>z;
            for(i=0;i<v;i++)
                {int t2,p;
                cout<<"\nenter the resource constraint in the format operation resouce conststraint power\n";
                cin>>s1>>t2>>p;
                res[s1]=t2;res1[s1]=t2;;
                pow[s1]=p;
                }



            int f1;
            string STRING;
            ifstream infile;


            infile.open (l[h].c_str());



//OPENING INPUT FILE
            string previousLine="";
            while(!infile.eof()) // To get you all the lines.
            {
                getline(infile,STRING); // Saves the line in STRING.
                if (STRING != previousLine)
                {
                    previousLine=STRING;
                    f1=STRING.find("->");
                    t=STRING.find("label");
                if(f1>=0)
                {
                    e++;
                }//find no. of edges
                else if(t>=0)
                {
                    V++;
                }//find no. of vertices


                }

            }
            infile.close();

            struct Graph* graph = createGraph(V);//initialize the graph
            string ops[V];
            int a1,a2,a3,a4,a5,a6,c,b1,c1=0;
            int ed[MAXVALUE][2];
            int t1,t2;
            string su1,su2;

            infile.open (l[h].c_str());

            previousLine="";
            c=0;
            while(!infile.eof()) // To get you all the lines.
            {
                getline(infile,STRING); // Saves the line in STRING.
                if (STRING != previousLine)
                {
                    previousLine=STRING;
                    f1=STRING.find("->");
                    b1=STRING.find("label");
                    if(f1>=0)
                        {
                            for (i = 0; STRING[i] != '['; i++)
                            {
                                if(isdigit(STRING[i]))
                                {
                                    a1=i;
                                    break;
                                }
                            }
                            for (i = a1; STRING[i] != ' '; i++);
                                a2=i-1;
                            for (i = a2+1; STRING[i] != '['; i++)
                                {
                                    if(isdigit(STRING[i]))
                                    {
                                        a3=i;
                                        break;
                                    }
                                }
                                t=STRING.find("[");
                                t=t-2;

                                su1=STRING.substr(a1,a2-a1+1);
                                su2=STRING.substr(a3,t-a3+1);
                                t1=atoi(su1.c_str())-1;
                                t2=atoi(su2.c_str())-1;


                                addEdge(graph, t1, t2);
                                //add edges to the graph
                                c++;
                        }
                        if(b1>=0)
                        {
                            t=STRING.find("=");
                            su1=STRING[t+3];
                            if(STRING[t+2]=='\"')
                                ops[c1]=STRING.substr(t+3,z);


                            else
                                {ops[c1]=STRING.substr(t+2,z);
                                }// to find operator of the vertices of the graph
                            c1++;
                        }

                    }

                }

            infile.close();

            printGraph(graph);

           // double time_start = get_time();
            mobility(graph,V);//find asap alap schedule
            priority(graph);//find priority for list based schedule


            dfg(graph,V,ops);
            // double time_end = get_time();
             //cout <<"\nTime to find priority and list based schedule is "<< time_end - time_start;
//FIND LIST BASED SCHEDULE
            int e,m=0;
//TO STORE THE NAME OF THE FILE IN WHICH WE ARE GOING TO STORE OUTPUT EX.-in.txt
    l[h][l[h].size()-3]='t';
    l[h][l[h].size()-2]='x';
    l[h][l[h].size()-1]='t';
     ofstream myfile;
     cout<<"\noutput is in "<<l[h];

     myfile.open (l[h].c_str());
     myfile<<"\nnode\t"<<"alap\t"<<"asap\t"<<"prio\toper\tpow"<<"\n";
     for (c = 0 ; c < graph->V; c++){// TO STORE PRIORITY AND ASAP ALAP SCHEDULE IN FILE
        myfile << node[c]<<"\t"<<al[c]<<"\t"<<as[c]<<"\t"<<prio[node[c]]<<"\t"<<ops[c]<<"\t"<<pow[ops[c]]<<"\n";}

    for(e=0;e<V;e++)
    {

        if(lbs[e]>m)
        m=lbs[e];
    }//TO FIND MAXIMUM CONTROL STEP IN THE LIST BASED SCHEDULE
    myfile<<"\nLIST BASED SCHEDULE for resource constraint ";
    for (map<string,int>::iterator it=res.begin(); it!=res.end(); ++it)
    myfile<<"\t"<<it->first << " => " << it->second ;myfile<< '\n'<<"\n\n";


    t2=0;s=0;
    for(e=1;e<=m;e++)
    {
        t2=0;
    myfile<<"\ns"<<e<<"\t";
    for(i=0;i<graph->V;i++)
    {
    if(lbs[i]==e)
    {
    t2+=pow[ops[i]];
    }
    }
    myfile<<"power = "<<t2<<"\t";
    for(i=0;i<graph->V;i++)
    {
    if(lbs[i]==e)
    {
    myfile<<i+1<<"("<<ops[i]<<")"<<"  ";

    }
    }
    s+=t2;

    }
    myfile<<"\n\n\t\ttotal power = "<<s;
    //myfile<<"\n\n\tTime to find priority and list based schedule is "<< time_end - time_start<<" sec";
    //TO STORE LIST BASED SCHEDULE IN THE FILE
   myfile.close();

            fill(as, as+V, 0);

            fill(as1, as1+V, 0);
            fill(al, al+V, 0);
            V=0;
res.clear();res1.clear();//TO CLEAR ALL VARIABLES
    }

    return 0;
}
