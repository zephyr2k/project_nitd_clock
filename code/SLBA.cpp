

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
#define SWAP(a, b)  a ^= b; b ^= a; a ^= b;
#define MAXVALUE 1000
using namespace std;
    int as[MAXVALUE];
    int as1[MAXVALUE];
    int al[MAXVALUE];
    int V;
    int tem;
    int prio[MAXVALUE];
    int node[MAXVALUE];
    vector<int> stl;map<int,int> mymap;
    map<char,int> res;
    map<char,int> res1;
    int lbs[1000];


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
    struct AdjListNode* newNode =(struct AdjListNode*) malloc(sizeof(struct AdjListNode));
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
void dfg(struct Graph* graph,int V,char ops[])
{// TO FIND LIST BASED SCHEDULE
   int i,m=0,j,c,d,e,v,f;
    vector<int>::size_type sz;
    for(e=0;e<V;e++)
        lbs[e]=1999;
    for(i=0;i<V;i++ )
    {//cout<<as1[i]<<endl;
        if(m<as1[i])
        m=as1[i];
    }
    for(i=1;i<=m;i++ )
    {
        res1.clear();
        res1.insert(res.begin(), res.end());


        for(j=0;j<V;j++ )
        {
            if(as1[j]==i)
            stl.push_back(j+1);
        }
        sz=stl.size();
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

        sz=stl.size();
        f=0;
        for (c = 0 ; c < sz; c++)
        {
            if(res1[ops[stl[c]-1]]>0)
            {
                f=0;
                struct AdjListNode* pCrawl = graph->pre[stl[c]-1].head;
                while (pCrawl)
                {
                    if(lbs[pCrawl->dest]==1999||lbs[pCrawl->dest]==i)
                    {
                        f=1;
                    }
                    pCrawl = pCrawl->next;
                }

                if(f==0){
                    lbs[stl[c]-1]=i;
                    res1[ops[stl[c]-1]]--;
                    stl[c]=10000;
                }
            }

        }
        for (c = 0 ; c < stl.size(); c++)
        {
            if(stl[c]==10000)
            stl.erase(stl.begin()+c);

        }

    }
    if(!stl.empty())
    {
        while(!stl.empty())
        {
            i++;
            res1.clear();
            res1.insert(res.begin(), res.end());

            sz=stl.size();
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

                sz=stl.size();
        f=0;
        for (c = 0 ; c < sz; c++)
        {
            if(res1[ops[stl[c]-1]]>0)
                {
                    f=0;
                    struct AdjListNode* pCrawl = graph->pre[stl[c]-1].head;

                    while (pCrawl)
                    {
                        if(lbs[pCrawl->dest]==1999||lbs[pCrawl->dest]==i)
                        {
                            f=1;
                        }
                        pCrawl = pCrawl->next;
                    }

                    if(f==0)
                        {
                            lbs[stl[c]-1]=i;
                            res1[ops[stl[c]-1]]--;
                            stl[c]=10000;
                        }
                }

        }
        for (c = 0 ; c < stl.size(); c++)
        {
            if(stl[c]==10000)
                stl.erase(stl.begin()+c);

        }


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
    for(i=1;i<=m;i++)
    {
        c=0;
        z=0;

        for(e=0;e<V;e++)
        {
            if(lbs[e]==i)
                {
                    c++;
                }
        }
        if(c==0){
        f4++;z=0;
        c=0;}
        else
        cout<<"\ns"<<i-f4<<"    ----- ";
        for(e=0;e<V;e++)
        {
            if(lbs[e]==i)
            {
                cout<<e+1<<" ";
                c++;
                lbs[e]=i-f4;
            }
        }
        cout<<"\n";

    }
    cout<<"\nnode lbs\n";
    for(e=0;e<V;e++)
    {
        cout<<"\n"<<e+1<<"  "<<lbs[e];
        if(lbs[e]>m)
        m=lbs[e];
    }
}
void priority(struct Graph* graph)
{//to find priority array
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
void addEdge(struct Graph* graph, int src, int dest)
{//ADD EDGES TO THHE GRAPH

    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->su[src].head;
    graph->su[src].head = newNode;


    newNode = newAdjListNode(src);
    newNode->next = graph->pre[dest].head;
    graph->pre[dest].head = newNode;
}


void printGraph(struct Graph* graph)
{//TO PRINT GRAPH PRODUCED
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
void mobility(struct Graph* graph,int V)
{//TO FIND ASAP AND ALAP SCHEDULE
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




int main()
{
    int t;
    stack<int> f;


    int V=0;
    int te[MAXVALUE];
    int s=0;
    int a=0,b,e=0,i,v,h;
//FILES FOR WHICH LIST BASED SCHEDULE IS TO BE FOUND
    int stsize;
    cout<<"Enter the no. of dot files ";
    cin>>stsize;

    string l[stsize];

    //string l[]={"in.dot","hal.dot","fir2.dot","arf1.dot","iir2.dot"};
    string s1;

    for(h=0;h<stsize;h++)
        {
            cout<<"\nEnter the name of dot files\n";
            cin>>l[h];
            cout<<"\nif you want to enter your own resource constraint enter 1 else 0\n";
            cin>>v;
            if(v==1)
                {
                cout<<"\nenter the resource constraint in the format **+- if your operations are in the format *+- etc else enter the first character of each operation name ex-if operation is mul then enter m\n";
                cin>>s1;
                }
            else
                s1="**+-<>i^mmasle";
            //cout<<"\n\n\n"<<l[h]<<"\n";

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
            char ops[V];
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
                                ops[c1]=STRING[t+3];
                            else
                                ops[c1]=STRING[t+2];// to find operator of the vertices of the graph
                            c1++;
                        }

                    }

                }

            infile.close();
            printGraph(graph);


            mobility(graph,V);//find asap alap schedule
            priority(graph);//find priority for list based schedule
            for(i=0;i<s1.size();i++)
            {res[s1[i]]++;res1[s1[i]]++;
            }//to store the resource constraints in a map


            dfg(graph,V,ops);
//FIND LIST BASED SCHEDULE
            int e,m=0;
//TO STORE THE NAME OF THE FILE IN WHICH WE ARE GOING TO STORE OUTPUT EX.-in.txt
    l[h][l[h].size()-3]='t';
    l[h][l[h].size()-2]='x';
    l[h][l[h].size()-1]='t';
     ofstream myfile;
     cout<<"\noutput is in "<<l[h];

     myfile.open (l[h].c_str());
     myfile<<"\nnode "<<"alap "<<"asap "<<"priority  operation"<<"\n";
     for (c = 0 ; c < graph->V; c++){// TO STORE PRIORITY AND ASAP ALAP SCHEDULE IN FILE
        myfile << node[c]<<"    "<<al[c]<<"    "<<as[c]<<"    "<<prio[node[c]]<<"            "<<ops[c]<<"\n";}

    for(e=0;e<V;e++)
    {

        if(lbs[e]>m)
        m=lbs[e];
    }//TO FIND MAXIMUM CONTROL STEP IN THE LIST BASED SCHEDULE
    myfile<<"\nLIST BASED SCHEDULE for resource constraint "<<s1<<"\n\n";

    for(e=1;e<=m;e++)
    {
    myfile<<"\ns"<<e<<"     ";
    for(i=0;i<graph->V;i++)
    {
    if(lbs[i]==e)
    {
    myfile<<i+1<<"  ";
    }
    }
    }//TO STORE LIST BASED SCHEDULE IN THE FILE
   myfile.close();

            fill(as, as+V, 0);

            fill(as1, as1+V, 0);
            fill(al, al+V, 0);
            V=0;
res.clear();res1.clear();//TO CLEAR ALL VARIABLES
    }

    return 0;
}
