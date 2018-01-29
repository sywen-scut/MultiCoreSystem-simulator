//two/four lever for auto distribution with parameter inputing
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<vector>
#include<string>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
using namespace std;
#define NUM 80
#define TILE 16
//#define OCCUP 12
#define TOTAL_COUNT 64


struct test {
    int s;
    int d;
    int value;
};
struct coo{
    int core;
    int z_ax;
    };
bool sort_value(const test &x, const test &y);
int manhattan_dis(int s,int d);
int multi_manhattan_dis(coo s,coo d);
vector < test > So;
vector < vector < int > > C;


int main(int argc, char** argv)
{
    int iArg=1;
    char name[100];
    if(!strcmp(argv[iArg],"-n")){
        strcpy(name,argv[iArg+1]);
    }
    cout<<name<<endl;

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
//	freopen("in.txt","r",stdin);
	fin.open(init_name);
 //   fin.open("in.log");
	printf("just test\n");
//	fin>>s_x>>s_y>>d_x>>d_y>>pack_val;
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
    strcpy(TF_name,"./output11/flow_");
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
    strcpy(Graph_name,"./output11/flow_");
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



   ofstream Graph_choose;
   ofstream Gephi_choose;
   char Graph_choose_name[100];
   char Gephi_choose_name[100];
   strcpy(Graph_choose_name,"./output11/flow_");
   strcat(Graph_choose_name,name);
   strcat(Graph_choose_name,"_Graph_choose.csv");
   strcpy(Gephi_choose_name,"./output11/flow_");
   strcat(Gephi_choose_name,name);
   strcat(Gephi_choose_name,"_Gephi_choose.csv");
    Graph_choose.open(Graph_choose_name);
    Gephi_choose.open(Gephi_choose_name);
    Gephi_choose<<"Source,target";
    Gephi_choose<<endl;
    int sum_c=0;
  //  So.resize(500);
    for(int i=8;i<TILE*4+8;i++)
    {
        for(int j=8;j<TILE*4+8;j++)
        {
            if(C[i][j]!=0)
            {
                Graph_choose<<i<<","<<j<<","<<C[i][j];
                Graph_choose<<endl;
                Gephi_choose<<i<<","<<j;
                Gephi_choose<<endl;
                sum_c++;
 //               comm_cost1+=manhattan_dis(Location1[i],Location1[j])*C[i][j];
  //              comm_cost2+=manhattan_dis(Location2[i],Location2[j])*C[i][j];
            }

        }
    }
    Graph_choose.close();
    Gephi_choose.close();
    cout<<"graph_choose output finished"<<endl;
    cout<<"gephi_choose output finished"<<endl;
    cout<<"sum communication num:"<<sum_c<<endl;
//    cout<<"communication cost of scheme1 of 8*8:"<<comm_cost1<<endl;

    So.resize(sum_c);
    ofstream Graph_sort;
    char Graph_sort_name[100];
    strcpy(Graph_sort_name,"./output11/flow_");
    strcat(Graph_sort_name,name);
    strcat(Graph_sort_name,"_Graph_sort.csv");
    Graph_sort.open(Graph_sort_name);
    int k=0;
    for(int i=8;i<TILE*4+8;i++)
    {
        for(int j=8;j<TILE*4+8;j++)
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

    long int comm_cost1_two=0,comm_cost2_two=0;
    int sort_count=0,core_count=0,Data_count=0;
    vector <coo> Location1_two;
    vector <coo> Location2_two;
    Location1_two.resize(TILE*4+8);
    Location2_two.resize(TILE*4+8);
    int model1_two[]={0,1,6,7,8,9,14,15,16,17,22,23,24,25,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47};
    int model2_two[TILE*2];
    int nu=32;
    for(int i=0;i<TILE*2;i++)
    {
         model2_two[i]=nu;
         nu++;
         cout<<model2_two[i]<<",";
    }
    cout<<endl;
    int flag_two[8+TILE*4];
    for(int i=8;i<8+TILE*4;i++)
        flag_two[i]=0;
    while(core_count<TILE*2){
        if(flag_two[So[sort_count].s]==0){
            flag_two[So[sort_count].s]=1;
            Location2_two[So[sort_count].s].core=model2_two[Data_count];
            Location1_two[So[sort_count].s].core=model1_two[Data_count];
            Location2_two[So[sort_count].s].z_ax=0;
            Location1_two[So[sort_count].s].z_ax=0;
            Data_count++;
            core_count++;
        }
         if(flag_two[So[sort_count].d]==0){
            flag_two[So[sort_count].d]=1;
            Location2_two[So[sort_count].d].core=model2_two[Data_count];
            Location1_two[So[sort_count].d].core=model1_two[Data_count];
            Location2_two[So[sort_count].d].z_ax=0;
            Location1_two[So[sort_count].d].z_ax=0;
            Data_count++;
            core_count++;
        }
        sort_count++;
    }
    cout<<sort_count<<"  "<<core_count<<"  "<<Data_count<<endl;
    core_count=0;
    Data_count=0;
    while(core_count<TILE*2){
        if(flag_two[So[sort_count].s]==0){
            flag_two[So[sort_count].s]=1;
            Location2_two[So[sort_count].s].core=model2_two[Data_count];
            Location1_two[So[sort_count].s].core=model1_two[Data_count];
            Location2_two[So[sort_count].s].z_ax=1;
            Location1_two[So[sort_count].s].z_ax=1;
            Data_count++;
            core_count++;
        }
         if(flag_two[So[sort_count].d]==0){
            flag_two[So[sort_count].d]=1;
            Location2_two[So[sort_count].d].core=model2_two[Data_count];
            Location1_two[So[sort_count].d].core=model1_two[Data_count];
            Location2_two[So[sort_count].d].z_ax=1;
            Location1_two[So[sort_count].d].z_ax=1;
            Data_count++;
            core_count++;
        }
        sort_count++;
    }
    for(int i=0;i<TILE*4+8;i++){
        cout<<Location1_two[i].core<<","<<Location1_two[i].z_ax<<"   ";
    }
    cout<<endl;
    for(int i=0;i<TILE*4+8;i++){
        cout<<Location2_two[i].core<<","<<Location2_two[i].z_ax<<"   ";
    }
    cout<<endl;
    for(int i=8;i<TILE*4+8;i++){
        for(int j=8;j<TILE*4+8;j++){
            if(C[i][j]!=0){
                comm_cost1_two+=multi_manhattan_dis(Location1_two[i],Location1_two[j])*C[i][j];
                comm_cost2_two+=multi_manhattan_dis(Location2_two[i],Location2_two[j])*C[i][j];
            }
        }
    }
    float propoti_two=0.0;
    propoti_two=(float)comm_cost1_two/comm_cost2_two;
    cout<<"communication cost of scheme1 of 32*2:"<<comm_cost1_two<<endl;
    cout<<"communication cost of scheme2 of 32*2:"<<comm_cost2_two<<endl;
    cout<<"propotion of scheme1 and scheme2 of 32*2 :"<<propoti_two<<endl;
    cout<<"communication of 32*2 cost finished "<<endl;


    long int comm_cost1_four=0,comm_cost2_four=0;
    sort_count=0;
    core_count=0;
    Data_count=0;
    vector <coo> Location1_four;
    vector <coo> Location2_four;
    Location1_four.resize(TILE*4+8);
    Location2_four.resize(TILE*4+8);
    int model1_four[]={0,1,6,7,8,9,14,15,16,17,22,23,24,25,30,31};
    int model2_four[]={34,35,36,37,42,43,44,45,50,51,52,53,58,59,60,61};
    int flag[8+TILE*4];
    for(int i=8;i<8+TILE*4;i++)
        flag[i]=0;
    while(core_count<TILE){
        if(flag[So[sort_count].s]==0){
            flag[So[sort_count].s]=1;
            Location2_four[So[sort_count].s].core=model2_four[Data_count];
            Location1_four[So[sort_count].s].core=model1_four[Data_count];
            Location2_four[So[sort_count].s].z_ax=0;
            Location1_four[So[sort_count].s].z_ax=0;
            Data_count++;
            core_count++;
        }
         if(flag[So[sort_count].d]==0){
            flag[So[sort_count].d]=1;
            Location2_four[So[sort_count].d].core=model2_four[Data_count];
            Location1_four[So[sort_count].d].core=model1_four[Data_count];
            Location2_four[So[sort_count].d].z_ax=0;
            Location1_four[So[sort_count].d].z_ax=0;
            Data_count++;
            core_count++;
        }
        sort_count++;
    }
    cout<<sort_count<<"  "<<core_count<<"  "<<Data_count<<endl;
    core_count=0;
    Data_count=0;
    while(core_count<TILE){
        if(flag[So[sort_count].s]==0){
            flag[So[sort_count].s]=1;
            Location2_four[So[sort_count].s].core=model2_four[Data_count];
            Location1_four[So[sort_count].s].core=model1_four[Data_count];
            Location2_four[So[sort_count].s].z_ax=1;
            Location1_four[So[sort_count].s].z_ax=1;
            Data_count++;
            core_count++;
        }
         if(flag[So[sort_count].d]==0){
            flag[So[sort_count].d]=1;
            Location2_four[So[sort_count].d].core=model2_four[Data_count];
            Location1_four[So[sort_count].d].core=model1_four[Data_count];
            Location2_four[So[sort_count].d].z_ax=1;
            Location1_four[So[sort_count].d].z_ax=1;
            Data_count++;
            core_count++;
        }
        sort_count++;
    }
    core_count=0;
    Data_count=0;
    while(core_count<TILE){
        if(flag[So[sort_count].s]==0){
            flag[So[sort_count].s]=1;
            Location2_four[So[sort_count].s].core=model2_four[Data_count];
            Location1_four[So[sort_count].s].core=model1_four[Data_count];
            Location2_four[So[sort_count].s].z_ax=2;
            Location1_four[So[sort_count].s].z_ax=2;
            Data_count++;
            core_count++;
        }
         if(flag[So[sort_count].d]==0){
            flag[So[sort_count].d]=1;
            Location2_four[So[sort_count].d].core=model2_four[Data_count];
            Location1_four[So[sort_count].d].core=model1_four[Data_count];
            Location2_four[So[sort_count].d].z_ax=2;
            Location1_four[So[sort_count].d].z_ax=2;
            Data_count++;
            core_count++;
        }
        sort_count++;
    }
    cout<<sort_count<<"  "<<core_count<<"  "<<Data_count<<endl;
    core_count=0;
    Data_count=0;
    while(core_count<TILE){
        if(flag[So[sort_count].s]==0){
            flag[So[sort_count].s]=1;
            Location2_four[So[sort_count].s].core=model2_four[Data_count];
            Location1_four[So[sort_count].s].core=model1_four[Data_count];
            Location2_four[So[sort_count].s].z_ax=3;
            Location1_four[So[sort_count].s].z_ax=3;
            Data_count++;
            core_count++;
        }
         if(flag[So[sort_count].d]==0){
            flag[So[sort_count].d]=1;
            Location2_four[So[sort_count].d].core=model2_four[Data_count];
            Location1_four[So[sort_count].d].core=model1_four[Data_count];
            Location2_four[So[sort_count].d].z_ax=3;
            Location1_four[So[sort_count].d].z_ax=3;
            Data_count++;
            core_count++;
        }
        sort_count++;
    }

   for(int i=0;i<TILE*4+8;i++){
        cout<<Location1_four[i].core<<","<<Location1_four[i].z_ax<<"   ";
    }
    cout<<endl;
    for(int i=0;i<TILE*4+8;i++){
        cout<<Location2_four[i].core<<","<<Location2_four[i].z_ax<<"   ";
    }
    cout<<endl;
    for(int i=8;i<TILE*4+8;i++){
        for(int j=8;j<TILE*4+8;j++){
            if(C[i][j]!=0){
                comm_cost1_four+=multi_manhattan_dis(Location1_four[i],Location1_four[j])*C[i][j];
                comm_cost2_four+=multi_manhattan_dis(Location2_four[i],Location2_four[j])*C[i][j];
            }
        }
    }
    float propoti_four=0.0;
    propoti_four=(float)comm_cost1_four/comm_cost2_four;
    cout<<"communication cost of scheme1 of 16*4:"<<comm_cost1_four<<endl;
    cout<<"communication cost of scheme2 of 16*4:"<<comm_cost2_four<<endl;
    cout<<"propotion of scheme1 and scheme2 of 16*4 :"<<propoti_four<<endl;
    cout<<"communication of 16*4 cost finished "<<endl;




//for gephi input file
    ofstream Gephi_weight;
    char Gephi_weight_name[100];
    strcpy(Gephi_weight_name,"./output11/flow_");
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
    cout<<"Gephi_weight output finished"<<endl;

    ofstream Gephi;
    char Gephi_name[100];
    strcpy(Gephi_name,"./output11/flow_");
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
    cout<<"Gephi output finished"<<endl;


	return 0;
}

bool sort_value(const test &x, const test &y)
{
    return x.value>y.value;
}
int manhattan_dis(int s,int d)
{
    int XDIM=(int)sqrt(TOTAL_COUNT);
    int x_axis_s,x_axis_d,y_axis_s,y_axis_d;
    int dis;
    x_axis_s=s%XDIM;
    y_axis_s=s/XDIM;
    x_axis_d=d%XDIM;
    y_axis_d=d/XDIM;
    dis=abs(x_axis_s-x_axis_d)+abs(y_axis_s-y_axis_d);
    return dis;
}
int multi_manhattan_dis(coo s,coo d)
{
    int XDIM;
    int x_axis_s,y_axis_s,x_axis_d,y_axis_d,z_axis_s,z_axis_d;
    int dis;
    XDIM=(int)sqrt(TOTAL_COUNT);
    x_axis_s=s.core%XDIM;
    y_axis_s=s.core/XDIM;
    z_axis_s=s.z_ax;
    x_axis_d=d.core%XDIM;
    y_axis_d=d.core/XDIM;
    z_axis_d=d.z_ax;
    dis=abs(x_axis_s-x_axis_d)+abs(y_axis_s-y_axis_d)+abs(z_axis_s-z_axis_d);
    return dis;

}

