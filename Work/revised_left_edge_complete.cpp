/********************************************/
/*Program: Graph Reordering in ALAP Form	*
*		   and Mobility 					*

* Description: This program takes a .dot 	*
* file as input and evaluates the ASAP		*
* form of the graph mentioned in the file.	*
* Structures used: Struct, Pointers, Loops,	*
* File Handlers, Library Functions.			*/
/********************************************/
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <string.h>
#include <assert.h>
using namespace std;
#define K 8

int global_count = 0;

//Structure for the Node to store the individual elements of the graphs.
typedef unsigned long long int LL;
typedef class node {
    public:
        char node_name[10];
        node *next[K];
        node *pred[K];
        int node_number;
        int control_step_asap;
        int control_step_alap;
        int mobility;
        int flag;
        node()
        {
			int i;
			control_step_asap = 0;
			for(i=0;i<K;i++)
			{
				next[i] = 0;
				pred[i] = 0;
			}
            flag = 0;
        }

}node;


//Main Function starts. All the computation of the algorithm resides here.
int main()
{
	ifstream file;	//File Handler
	file.open("../benchmark/iir2.dot",ios::in);	//Accessing the input file. It is in the .dot format. *Needs to be changed according to the input path of the file*
	node *arr[100000];
	char num1[10];
	char num2[10];
	//int global_count=0;	//Global counter for the number of nodes present in the input graph.
	if(file.is_open())
	{
		string line;
		getline(file, line);
		getline(file, line);
		//cout<<"hello"<<endl;
		while(!file.eof())
		{

			getline(file, line);
			for(LL i=0; i<line.length(); i++)
			{
				if(line[i] == '=' && line[i+1] == ' ')	//Condition for checking and accessing the lines containing the node information of the graphs.
				{
					node *vertex = new node();	//Dynamically defining a new node module. Vertex points to it
					vertex->node_name[0] = line[i+2];
					vertex->node_name[1] = line[i+3];
					vertex->node_name[2] = line[i+4];
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
					vertex->node_number = atoi(num);	//Storing the node number in the node structure.
					arr[global_count] = vertex;	//Adding the node pointer to an array, for access later while defining links.
					global_count++;
				}
				if(line[i] == '-' && line[i+1] == '>')	//Condition for accessing the lines where links are defined.
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
					for(int y = l; y<line.length(); y++)
					{
						if(int(line[y])>=48 && int(line[y])<=57)
						{
							num2[x]=line[y];
							x++;
							if(line[y+1] == ' ')
								break;
						}
					}
					node *vertex1, *vertex2;	//Declaring two node pointers to map one node to another.
					for(LL j = 0; j < global_count; j++)
					{
						if(arr[j]->node_number == atoi(num1))
							vertex1 = arr[j];
						if(arr[j]->node_number == atoi(num2))
							vertex2 = arr[j];
					}

					LL i,j;
					for(i=0; i < K && vertex1->next[i]!=0; i++);
					for(j=0; j < K && vertex2->pred[j]!=0; j++);
					vertex1->next[i] = vertex2;	//Defining the next relationship from one node to the other.
					vertex2->pred[j] = vertex1;	//Defining the previous relationship in the reverse order.
					for(int j=0; j<10; j++)
						num1[j]=num2[j]='\0';
				}
			}
		}
	}
	file.close();	//Closing the openend file.
	int max=1;
    //cout<<"hello"<<endl;
	//ASAP Scheduling
	cout<<"global_count "<<global_count<<endl;
	int temp_global_count = global_count;
	for(LL i = 0; i < global_count; i++)	//Scheduling the nodes according to ASAP notation.
	{
		if(arr[i]->pred[0] == 0)
		{
			arr[i]->control_step_asap = 1;	//All the nodes with no predecessor are assigned in the First control step.
			arr[i]->mobility = arr[i]->control_step_asap; //Assigning the control step value to mobility
			temp_global_count--;
			//arr[i]->flag = 1;
            //cout<<"control_step_asap -> "<<arr[i]->control_step_asap<<endl;
		}
	}
	int maxcsa = 1;int flag = 0 ;
	while(temp_global_count!=0)
	{

		for(LL i = 0 ; i<global_count; i++)
		{

			if(arr[i]->control_step_asap !=0)
				continue;
			maxcsa = 1; flag = 0 ;

			for(LL j = 0; j <K && arr[i]->pred[j]!=0 ;j++)
			{

					if(arr[i]->pred[j]->control_step_asap == 0)
					{
						flag = 1;
						break;
					}
					if(maxcsa<arr[i]->pred[j]->control_step_asap)
						maxcsa = arr[i]->pred[j]->control_step_asap;
			}
			if(flag==0)
			{
				arr[i]->control_step_asap = maxcsa + 1;
				arr[i]->mobility = arr[i]->control_step_asap;
				if(max < arr[i]->control_step_asap)
					max = arr[i]->control_step_asap;
				temp_global_count--;
			}
		}
	}
	//cout<<"Hello"<<endl;
	//ALAP Scheduling
	temp_global_count = global_count;
	int max1=max;
	for(LL i = 0; i < global_count; i++)	//Scheduling the nodes in the last Control Step
	{
		if(arr[i]->next[0] == 0)
		{
			arr[i]->control_step_alap = max;
			temp_global_count--;
		}
		else
			arr[i]->control_step_alap=0;
	}
	while(temp_global_count!=0)
	{
		for(LL i = 0 ; i<global_count; i++)
		{
			if(arr[i]->control_step_alap !=0)
				continue;
			int mincsa = max;int flag = 0 ;
			for(LL j = 0; j <K && arr[i]->next[j]!=0 ;j++)
			{
					if(arr[i]->next[j]->control_step_alap == 0)
					{
						flag = 1;
						break;
					}
					if(mincsa>arr[i]->next[j]->control_step_alap)
						mincsa = arr[i]->next[j]->control_step_alap;
			}
			if(flag==0)
			{
				arr[i]->control_step_alap = mincsa - 1;
				temp_global_count--;
			}
		}
	}

	for(LL i = 0; i < global_count; i++)
	{	//Calculating the Mobility Value by Subtracting the previously computed ASAP Control Step Value from current ALAP Control Step value.
		arr[i]->mobility = arr[i]->control_step_alap - arr[i]->control_step_asap;
		cout<<arr[i]->node_number<<" "<<arr[i]->node_name<<"->"<<arr[i]->control_step_asap<<" ->"<<arr[i]->control_step_alap<<" ->"<<arr[i]->mobility<<endl;
        //cout<<"Hello1"<<endl;
	}

	//Left Edge Algorithm
	node *temp;
	for(LL i=0;i < global_count; i++)
	{
		for(LL j=0; j < (global_count-i-1); j++)
		{
			if(arr[j+1]->control_step_asap < arr[j]->control_step_asap)
			{
				temp=arr[j+1];
				arr[j+1]=arr[j];
				arr[j]=temp;
			}
		/*	else if ((arr[j+1]->control_step_asap == arr[j]->control_step_asap) && (arr[j+1]->control_step_alap > arr[j]->control_step_alap))
			{
				temp=arr[j+1];
				arr[j+1]=arr[j];
				arr[j]=temp;
			}
			*/
		}
	}
	//cout<<global_count<<"Hello"<<endl;
	for(int j=0;j < global_count; j++)
        cout <<arr[j]->node_number<<" "<<arr[j]->node_name<<" "<<arr[j]->control_step_asap<<" -> "<<arr[j]->control_step_alap<<endl;
	int j=0;
	int MAP[global_count][2];
	for(j=0;j<global_count;j++)
	{
	    MAP[j][0]=0;
	    MAP[j][1]=arr[j]->node_number;
	    arr[j]->flag=0;
	}
                                            //***********************************
                                            //***********************************
                                            //All The Allocation Part  **********
                                            //***********************************
                                            //***********************************


	int add_index = 0,imp_index=0,mul_index=0,sub_index=0,exp_index=0,les_index=0;



    int highest_asap = 0;
    for(int x = 0;x < global_count;x++)
    {
        if(arr[x]->control_step_asap>highest_asap)
        {
            highest_asap = arr[x]->control_step_asap;
        }
    }
    cout<<"highest max asap "<<highest_asap<<endl;

    int curr_var = 0;
    int temp_asap = 0;
    int max_imp_index = 0;
    int temp_imp_index;
    int top_imp_asap;

    int max_mul_index = 0;
    int temp_mul_index;
    int top_mul_asap;
    int counter_mul = 0;
    int temp_var;
    int temp_mul_asap;
    int mul_var = 0;

    for(curr_var = 0;curr_var < global_count;curr_var++)
    {
        if(arr[curr_var]->node_name[0] == 'm')
        {
            arr[curr_var]->flag = 0;
            max_mul_index++;
        }
    }

    /*
    for(curr_var = 0;curr_var < global_count;curr_var++)
    {
        if(arr[curr_var]->node_name[0] == 'm')
        {
            cout<<arr[curr_var]->control_step_asap<<"   "<<arr[curr_var]->control_step_alap<<"-<"<<arr[curr_var]->node_number<<endl;
        }
    }*/

    int original_mul_index = max_mul_index;
    while(temp_asap <= highest_asap)
    {
        temp_asap++;
        for(curr_var = 0;curr_var < global_count;curr_var++)
        {
            if((arr[curr_var]->control_step_asap == temp_asap) && (arr[curr_var]->node_name[0] == 'm'))
            {
                mul_var = curr_var;
                temp_mul_asap = temp_asap;
                temp_asap = highest_asap+1;
                break;
            }
        }
    }
    temp_asap = temp_mul_asap;
    int max_alap_mul = 0;

    while(temp_asap <= highest_asap)
    {
        temp_mul_index = 0;
        if(temp_asap == temp_mul_asap)
        {
            do
            {
                max_alap_mul = 0;
                for(curr_var = 0;curr_var < global_count;curr_var++)
                {
                    if((arr[curr_var]->control_step_asap == temp_asap) && (arr[curr_var]->node_name[0] == 'm') && (arr[curr_var]->flag == 0))
                    {
                        if(arr[curr_var]->control_step_alap > max_alap_mul)
                        {
                            max_alap_mul = arr[curr_var]->control_step_alap;
                            mul_var = curr_var;
                        }
                    }
                }
                if(mul_var!=-1)
                {
                    arr[mul_var]->flag = 1;
                    cout<<arr[mul_var]->node_number<<" "<<arr[mul_var]->node_name<<"->"<<++temp_mul_index<<endl;
                }
                max_mul_index--;
                mul_var = -1;
            }while(max_mul_index > 1);
        }

        temp_asap++;
        temp_mul_index = 0;
        max_mul_index = original_mul_index;

        do
        {
            max_alap_mul = 0;
            for(curr_var = 0;curr_var < global_count;curr_var++)
            {
                if((arr[curr_var]->control_step_asap == temp_asap) && (arr[curr_var]->node_name[0] == 'm') && (arr[curr_var]->flag == 0))
                {
                    if(arr[curr_var]->control_step_alap > max_alap_mul)
                    {
                        max_alap_mul = arr[curr_var]->control_step_alap;
                        mul_var = curr_var;
                    }
                }
            }
            if(mul_var!=-1)
            {
                temp_mul_index = 0;
                arr[mul_var]->flag = 1;
                for(temp_var = 0;temp_var < curr_var;temp_var++)
                {
                    if((arr[temp_var]->node_name[0] == 'm') && (arr[temp_var]->flag == 1))
                    {
                        if(arr[temp_var]->control_step_alap >= temp_asap)
                            ++temp_mul_index;
                    }
                }
                cout<<arr[mul_var]->node_number<<" "<<arr[mul_var]->node_name<<"->"<<temp_mul_index++<<endl;
            }
            max_mul_index--;
            mul_var = -1;
        }while(max_mul_index > 1);
    }




    curr_var = 0;
    temp_asap = 0;
    max_mul_index = 0;
    counter_mul = 0;
    mul_var = 0;

    for(curr_var = 0;curr_var < global_count;curr_var++)
    {
        if(arr[curr_var]->node_name[0] == 's')
        {
            arr[curr_var]->flag = 0;
            max_mul_index++;
        }
    }
    original_mul_index = max_mul_index;
    while(temp_asap <= highest_asap)
    {
        temp_asap++;
        for(curr_var = 0;curr_var < global_count;curr_var++)
        {
            if((arr[curr_var]->control_step_asap == temp_asap) && (arr[curr_var]->node_name[0] == 's'))
            {
                mul_var = curr_var;
                temp_mul_asap = temp_asap;
                temp_asap = highest_asap+1;
                break;
            }
        }
    }
    temp_asap = temp_mul_asap;
    max_alap_mul = 0;

    while(temp_asap <= highest_asap)
    {
        temp_mul_index = 0;
        if(temp_asap == temp_mul_asap)
        {
            do
            {
                max_alap_mul = 0;
                for(curr_var = 0;curr_var < global_count;curr_var++)
                {
                    if((arr[curr_var]->control_step_asap == temp_asap) && (arr[curr_var]->node_name[0] == 's') && (arr[curr_var]->flag == 0))
                    {
                        if(arr[curr_var]->control_step_alap > max_alap_mul)
                        {
                            max_alap_mul = arr[curr_var]->control_step_alap;
                            mul_var = curr_var;
                        }
                    }
                }
                if(mul_var!=-1)
                {
                    arr[mul_var]->flag = 1;
                    cout<<arr[mul_var]->node_number<<" "<<arr[mul_var]->node_name<<"->"<<++temp_mul_index<<endl;
                }
                max_mul_index--;
                mul_var = -1;
            }while(max_mul_index > 1);
        }

        temp_asap++;
        temp_mul_index = 0;
        max_mul_index = original_mul_index;

        do
        {
            max_alap_mul = 0;
            for(curr_var = 0;curr_var < global_count;curr_var++)
            {
                if((arr[curr_var]->control_step_asap == temp_asap) && (arr[curr_var]->node_name[0] == 's') && (arr[curr_var]->flag == 0))
                {
                    if(arr[curr_var]->control_step_alap > max_alap_mul)
                    {
                        max_alap_mul = arr[curr_var]->control_step_alap;
                        mul_var = curr_var;
                    }
                }
            }
            if(mul_var!=-1)
            {
                temp_mul_index = 0;
                arr[mul_var]->flag = 1;
                for(temp_var = 0;temp_var < curr_var;temp_var++)
                {
                    if((arr[temp_var]->node_name[0] == 's') && (arr[temp_var]->flag == 1))
                    {
                        if(arr[temp_var]->control_step_alap >= temp_asap)
                            ++temp_mul_index;
                    }
                }
                cout<<arr[mul_var]->node_number<<" "<<arr[mul_var]->node_name<<"->"<<temp_mul_index++<<endl;
            }
            max_mul_index--;
            mul_var = -1;
        }while(max_mul_index > 1);
    }


    curr_var = 0;
    temp_asap = 0;
    max_mul_index = 0;
    counter_mul = 0;
    mul_var = 0;

    for(curr_var = 0;curr_var < global_count;curr_var++)
    {
        if(arr[curr_var]->node_name[0] == 'a')
        {
            arr[curr_var]->flag = 0;
            max_mul_index++;
        }
    }
    original_mul_index = max_mul_index;
    while(temp_asap <= highest_asap)
    {
        temp_asap++;
        for(curr_var = 0;curr_var < global_count;curr_var++)
        {
            if((arr[curr_var]->control_step_asap == temp_asap) && (arr[curr_var]->node_name[0] == 'a'))
            {
                mul_var = curr_var;
                temp_mul_asap = temp_asap;
                temp_asap = highest_asap+1;
                break;
            }
        }
    }
    temp_asap = temp_mul_asap;
    max_alap_mul = 0;

    while(temp_asap <= highest_asap)
    {
        temp_mul_index = 0;
        if(temp_asap == temp_mul_asap)
        {
            do
            {
                max_alap_mul = 0;
                for(curr_var = 0;curr_var < global_count;curr_var++)
                {
                    if((arr[curr_var]->control_step_asap == temp_asap) && (arr[curr_var]->node_name[0] == 'a') && (arr[curr_var]->flag == 0))
                    {
                        if(arr[curr_var]->control_step_alap > max_alap_mul)
                        {
                            max_alap_mul = arr[curr_var]->control_step_alap;
                            mul_var = curr_var;
                        }
                    }
                }
                if(mul_var!=-1)
                {
                    arr[mul_var]->flag = 1;
                    cout<<arr[mul_var]->node_number<<" "<<arr[mul_var]->node_name<<"->"<<++temp_mul_index<<endl;
                }
                max_mul_index--;
                mul_var = -1;
            }while(max_mul_index > 1);
        }

        temp_asap++;
        temp_mul_index = 0;
        max_mul_index = original_mul_index;

        do
        {
            max_alap_mul = 0;
            for(curr_var = 0;curr_var < global_count;curr_var++)
            {
                if((arr[curr_var]->control_step_asap == temp_asap) && (arr[curr_var]->node_name[0] == 'a') && (arr[curr_var]->flag == 0))
                {
                    if(arr[curr_var]->control_step_alap > max_alap_mul)
                    {
                        max_alap_mul = arr[curr_var]->control_step_alap;
                        mul_var = curr_var;                    }
                }
            }
            if(mul_var!=-1)
            {
                temp_mul_index = 0;
                arr[mul_var]->flag = 1;
                for(temp_var = 0;temp_var < curr_var;temp_var++)
                {
                    if((arr[temp_var]->node_name[0] == 'a') && (arr[temp_var]->flag == 1))
                    {
                        if(arr[temp_var]->control_step_alap >= temp_asap)
                            ++temp_mul_index;
                    }
                }
                cout<<arr[mul_var]->node_number<<" "<<arr[mul_var]->node_name<<"->"<<temp_mul_index++<<endl;
            }
            max_mul_index--;
            mul_var = -1;
        }while(max_mul_index > 1);
    }




    curr_var = 0;
    temp_asap = 0;
    max_mul_index = 0;
    counter_mul = 0;
    mul_var = 0;

    for(curr_var = 0;curr_var < global_count;curr_var++)
    {
        if(arr[curr_var]->node_name[0] == 'l')
        {
            arr[curr_var]->flag = 0;
            max_mul_index++;
        }
    }
    original_mul_index = max_mul_index;
    while(temp_asap <= highest_asap)
    {
        temp_asap++;
        for(curr_var = 0;curr_var < global_count;curr_var++)
        {
            if((arr[curr_var]->control_step_asap == temp_asap) && (arr[curr_var]->node_name[0] == 'l'))
            {
                mul_var = curr_var;
                temp_mul_asap = temp_asap;
                temp_asap = highest_asap+1;
                break;
            }
        }
    }
    temp_asap = temp_mul_asap;
    max_alap_mul = 0;

    while(temp_asap <= highest_asap)
    {
        temp_mul_index = 0;
        if(temp_asap == temp_mul_asap)
        {
            do
            {
                max_alap_mul = 0;
                for(curr_var = 0;curr_var < global_count;curr_var++)
                {
                    if((arr[curr_var]->control_step_asap == temp_asap) && (arr[curr_var]->node_name[0] == 'l') && (arr[curr_var]->flag == 0))
                    {
                        if(arr[curr_var]->control_step_alap > max_alap_mul)
                        {
                            max_alap_mul = arr[curr_var]->control_step_alap;
                            mul_var = curr_var;
                        }
                    }
                }
                if(mul_var!=-1)
                {
                    arr[mul_var]->flag = 1;
                    cout<<arr[mul_var]->node_number<<" "<<arr[mul_var]->node_name<<"->"<<++temp_mul_index<<endl;
                }
                max_mul_index--;
                mul_var = -1;
            }while(max_mul_index > 1);
        }

        temp_asap++;
        temp_mul_index = 0;
        max_mul_index = original_mul_index;

        do
        {
            max_alap_mul = 0;
            for(curr_var = 0;curr_var < global_count;curr_var++)
            {
                if((arr[curr_var]->control_step_asap == temp_asap) && (arr[curr_var]->node_name[0] == 'l') && (arr[curr_var]->flag == 0))
                {
                    if(arr[curr_var]->control_step_alap > max_alap_mul)
                    {
                        max_alap_mul = arr[curr_var]->control_step_alap;
                        mul_var = curr_var;

                    }
                }
            }
            if(mul_var!=-1)
            {
                temp_mul_index = 0;
                arr[mul_var]->flag = 1;
                for(temp_var = 0;temp_var < curr_var;temp_var++)
                {
                    if((arr[temp_var]->node_name[0] == 'l') && (arr[temp_var]->flag == 1))
                    {
                        if(arr[temp_var]->control_step_alap >= temp_asap)
                            ++temp_mul_index;
                    }
                }
                cout<<arr[mul_var]->node_number<<" "<<arr[mul_var]->node_name<<"->"<<temp_mul_index++<<endl;
            }
            max_mul_index--;
            mul_var = -1;
        }while(max_mul_index > 1);
    }


	return 0;
}
