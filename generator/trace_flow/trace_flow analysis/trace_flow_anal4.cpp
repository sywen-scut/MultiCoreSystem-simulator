#include<iostream>
#include<fstream>
#include<stdio.h>
#include<vector>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
using namespace std;
#define NUM 80
//#define TILE 20
//#define TILE 32
//#define OCCUP 12
//#define TOTAL_COUNT 64
//#define TOTAL_COUNT 144

int TILE=0,TOTAL_COUNT=0;
struct test {
    int s;
    int d;
    int value;
};
bool sort_value(const test &x, const test &y);
int manhattan_dis(int s,int d);
vector < test > So;
vector < vector < int > > C;
int main(int argc, char** argv)
{
    int iArg=1;
    char name[100];
    do{
        if(!strcmp(argv[iArg],"-help")){
            cout<<"-C TOTAL_COUNT -T TILE"<<endl;
            cout<<"TOTAL_COUNT:[64/144/..]"<<endl;
            cout<<"TILE:[16/20/24/32/..]"<<endl;
           return 0;
        }
        else if(!strcmp(argv[iArg],"-N")){
            strcpy(name,argv[iArg+1]);
            iArg+=2;
        }
        else if(!strcmp(argv[iArg],"-C")){
            TOTAL_COUNT=atoi(argv[iArg+1]);
            iArg+=2;
        }
        else if(!strcmp(argv[iArg],"-T")){
            TILE=atoi(argv[iArg+1]);
            iArg+=2;
        }
      }
    while(iArg<argc);
    cout<<name<<","<<TOTAL_COUNT<<","<<TILE;
    cout<<endl;

	int s_x,s_y,d_x,d_y,pack_val,s_id,d_id;
	long int cycle;
    char init_name[100];

	fstream fin;
    strcpy(init_name,"../flow_");
    strcat(init_name,name);
    strcat(init_name,"_org.log");
	C.resize(NUM);
	for (int i=0;i<NUM;i++)
	{
		C[i].resize(NUM);
	}
	fin.open(init_name);
	printf("just test\n");
	while(!fin.eof())
	{
		fin>>s_x>>s_y>>d_x>>d_y>>cycle>>pack_val;
 //       printf("%d %d %d %d %d %d\n",s_x,s_y,d_x,d_y,cycle,pack_val);
		s_id=s_x*8+s_y;
		d_id=d_x*8+d_y;
	//	printf("%d %d\n",s_id,d_id);
		C[s_id][d_id]=C[s_id][d_id]+pack_val;
	//	printf("%d\n",C[s_id][d_id]);
	}
	fin.close();
	cout<<"trace flow calculate"<<endl;

	ofstream TF;
    char TF_name[100];
    strcpy(TF_name,"./output/flow_");
    strcat(TF_name,name);
    strcat(TF_name,"_TF.txt");
	TF.open(TF_name);
	for (int i=0;i<NUM;i++)
	{
		for (int j=0;j<NUM;j++)
		{
			TF<<C[i][j]<<"   ";
		}
		TF<<endl;
	}
	TF.close();
	cout<<"trace flow output finished"<<endl;

    ofstream Graph;
    char Graph_name[100];
    strcpy(Graph_name,"./output/flow_");
    strcat(Graph_name,name);
    strcat(Graph_name,"_Graph.csv");
    Graph.open(Graph_name);
    for(int i=0;i<NUM;i++)
    {
        for(int j=0;j<NUM;j++)
        {
            if(C[i][j]!=0)
            {
                Graph<<i<<","<<j<<","<<C[i][j];
                Graph<<endl;
            }

        }
    }
    Graph.close();
    cout<<"graph output finished"<<endl;

    vector <int> Location1;
    vector <int> Location2;
    Location1.resize(TILE+8);
    Location2.resize(TILE+8);
     int model1[100],model2[100];

   if(TOTAL_COUNT==64)
    {
        int temp1[]={0,1,6,7,8,9,14,15,16,17,22,23,24,25,30,31,32,33,38,39};
        for(int i=0;i<TILE;i++)model1[i]=temp1[i];
        int temp2[]={26,27,28,29,34,35,36,37,42,43,44,45,50,51,52,53,58,59,60,61};
        for(int i=0;i<TILE;i++)model2[i]=temp2[i];
    }

    if(TOTAL_COUNT==144)
    {
            
        int temp3[]={0,1,2,3,8,9,10,11,12,13,14,15,20,21,22,23,24,25,26,27,32,33,34,35,36,37,38,39,44,45,46,47};
        for(int i=0;i<TILE;i++)model1[i]=temp3[i];
        int temp4[]={51,52,53,54,55,56,63,64,65,66,67,68,75,76,77,78,79,80,87,88,89,90,91,92,99,100,101,102,103,104,113,114};
        for(int i=0;i<TILE;i++)model2[i]=temp4[i];
    }

    int i,j=0;
    int temp[]={15,18,33,19,9,12,36,30,13,26,39,27,11,23,25,16,28,10,29,22,24,31,8,14,38,20,32,17,35,34,21,37};
    for(int k=0;k<TILE;k++)
    {
 //       cin>>i;
        Location1[temp[k]]=model1[j];
        j++;
    }
    for(int k=0;k<TILE+8;k++)
        cout<<Location1[k]<<",";
        cout<<endl;
    j=0;
    for(int k=0;k<TILE;k++)
    {
  //      cin>>i;
        Location2[temp[k]]=model2[j];
        j++;
  //      cout<<Location[i]<<",";
    }
    for(int k=0;k<TILE+8;k++)
        cout<<Location2[k]<<",";
        cout<<endl;
    cout<<"node location finished"<<endl;

    long int comm_cost1=0,comm_cost2=0;
    float p=0.0;
   ofstream Graph_choose;
   ofstream Gephi_choose;
   char Graph_choose_name[100];
   char Gephi_choose_name[100];
   strcpy(Graph_choose_name,"./output/flow_");
    strcat(Graph_choose_name,name);
    strcat(Graph_choose_name,"_Graph_choose.csv");
    Graph_choose.open(Graph_choose_name);
     strcpy(Gephi_choose_name,"./output/flow_");
    strcat(Gephi_choose_name,name);
    strcat(Gephi_choose_name,"_Gephi_choose.csv");
    Gephi_choose.open(Gephi_choose_name);
    Gephi_choose<<"Source,target";
    Gephi_choose<<endl;
    int sum_c=0;
  //  So.resize(500);
    for(int i=8;i<TILE+8;i++)
    {
        for(int j=8;j<TILE+8;j++)
        {
            if(C[i][j]!=0)
            {
                Graph_choose<<i<<","<<j<<","<<C[i][j];
                Graph_choose<<endl;
                Gephi_choose<<i<<","<<j;
                Gephi_choose<<endl;
                sum_c++;
                comm_cost1+=manhattan_dis(Location1[i],Location1[j])*C[i][j];
                comm_cost2+=manhattan_dis(Location2[i],Location2[j])*C[i][j];
            }

        }
    }
    p=(float)comm_cost1/comm_cost2;
    Graph_choose.close();
    Gephi_choose.close();
    cout<<"graph_choose output finished"<<endl;
    cout<<"gephi_choose output finished"<<endl;
    cout<<"sum communication num:"<<sum_c<<endl;
    cout<<"communication cost of scheme1 of 8*8/12*12:"<<comm_cost1<<endl;
    cout<<"communication cost of scheme2 of 8*8/12*12:"<<comm_cost2<<endl;
    cout<<"comm_cost1/comm_cost2 of schemes of 8*8/12*12:"<<p<<endl;

    So.resize(sum_c);
    ofstream Graph_sort;
    char Graph_sort_name[100];
    strcpy(Graph_sort_name,"./output/flow_");
    strcat(Graph_sort_name,name);
    strcat(Graph_sort_name,"_Graph_sort.csv");
    Graph_sort.open(Graph_sort_name);
    int k=0;
    for(int i=8;i<TILE+8;i++)
    {
        for(int j=8;j<TILE+8;j++)
        {
            if(C[i][j]!=0)
            {
                So[k].s=i;
                So[k].d=j;
                So[k].value=C[i][j];
                k++;
            }

        }
    }
    sort(So.begin(),So.end(),sort_value);
    for(int i=0;i<sum_c;i++)
    {
        Graph_sort<<So[i].s<<","<<So[i].d<<","<<So[i].value;
        Graph_sort<<endl;
    }
    Graph_sort.close();
    cout<<"graph_sort output finished"<<endl;

//for gephi input file
    ofstream Gephi_weight;
    char Gephi_weight_name[100];
    strcpy(Gephi_weight_name,"./output/flow_");
    strcat(Gephi_weight_name,name);
    strcat(Gephi_weight_name,"_Gephi_weight.csv");
    Gephi_weight.open(Gephi_weight_name);
    Gephi_weight<<"Source,Target";
    Gephi_weight<<endl;
    int lever=0;
    for(int i=0;i<NUM;i++)
    {
        for(int j=0;j<NUM;j++)
        {
            if(C[i][j]>=100)
            {
                lever=C[i][j]/100;
                if(lever>10)
                    lever=10;
                for(int k=0;k<lever;k++)
                {
                    Gephi_weight<<i<<","<<j;
                    Gephi_weight<<endl;
                }

            }

        }
    }
    Gephi_weight.close();
    cout<<"Gephi_weight outpur finished"<<endl;

    ofstream Gephi;
    char Gephi_name[100];
    strcpy(Gephi_name,"./output/flow_");
    strcat(Gephi_name,name);
    strcat(Gephi_name,"_Gephi.csv");
    Gephi.open(Gephi_name);
    Gephi<<"Source,Target";
    Gephi<<endl;
    for(int i=0;i<NUM;i++)
    {
        for(int j=0;j<NUM;j++)
        {
            if(C[i][j]!=0)
            {
                Gephi<<i<<","<<j;
                Gephi<<endl;
            }

        }
    }
    Gephi.close();
    cout<<"Gephi outpur finished"<<endl;


	return 0;
}

bool sort_value(const test &x, const test &y)
{
    return x.value<y.value;
}
int manhattan_dis(int s,int d)
{
    int XDIM;
    int x_axis_s,x_axis_d,y_axis_s,y_axis_d;
    int dis;

    XDIM=(int)sqrt(TOTAL_COUNT);
    x_axis_s=s%XDIM;
    y_axis_s=s/XDIM;
    x_axis_d=d%XDIM;
    y_axis_d=d/XDIM;
    dis=abs(x_axis_s-x_axis_d)+abs(y_axis_s-y_axis_d);
    return dis;
}

