#include<iostream>
#include<fstream>
#include<stdio.h>
#include<vector>
#include<string>
#include<stdlib.h>
#include<math.h>
using namespace std;
#define PIPELINE 5
//#define INTERVAL 100000
#define NUM 64

vector < vector < int > > C;
double corr[NUM][NUM];
int ID;
struct item{
    int ID;
    double value;
};
double calc_var(vector<int> &x, vector<int> &y);
double calc_means(vector<int> &x);
bool sort_value(const item &x, const item &y);
vector <item> cSortCorr;
vector <item> cSort;

int main(int argc, char** argv){
        int iArg=1;
        double freq=0;
        int interval=0;
        //string temp_str;
        char name[100];
        do{
            if(!strcmp(argv[iArg],"-help")){
                cout<<"-n  name  -i  time_interval -f frequency"<<endl;
                cout<<"name:[benchmarkname_adp/xy]"<<endl;
                cout<<"time_interval:[100/1000/10000/100000]"<<endl;
                cout<<"frequency:[0.1/0.25/0.33/0.5/0.66/0.75/1]"<<endl;
                return 0;
            }
            else if(!strcmp(argv[iArg],"-n")){
                strcpy(name,argv[iArg+1]);//give the file name eg:name=barnes_xy
                iArg+=2;
                cout<<name<<endl;
            }
            else if(!strcmp(argv[iArg],"-i")){
                interval=atoi(argv[iArg+1]);//use the function atoi for long integer data
                iArg+=2;
            }
            else if(!strcmp(argv[iArg],"-f")){
                freq=atof(argv[iArg+1]);
                iArg+=2;
            }
        }
        while(iArg<argc);


        int id;
        long int total_cycle;
        long int cycle;
        string pipeline;
        char lastLine[100];

        fstream fin;
        strcpy(lastLine,"./last_line/last_line_");//the array lastline[] load the string "  "
        strcat(lastLine,name);
        strcat(lastLine,".txt");//contact two string,eg,now the array lastline[0]=./last_line/last_line_barnes_xy.txt
        fin.open(lastLine);//open the file last_line_barnes_xy.txt,the data flow is fin
        fin>>id>>pipeline>>total_cycle;//output the file last_line.._xy.txt.
        fin.close();
        fin.clear();
        cout<<"initialization finished"<<endl;


        fstream fin2;
        C.resize(NUM);//expand the vector
        for(int i=0;i<NUM;i++){
            C[i].resize(total_cycle/interval+1);//expand sub vector
        }
       char srcFile[100];
        strcpy(srcFile,"../cpu_");
        strcat(srcFile,name);
        strcat(srcFile,".txt");//srcFile="../cpu_barnes_xy.txt"
        fin2.open(srcFile);
        fin2>>id>>pipeline>>cycle;
        while(!fin2.eof()){
                if(pipeline=="dispatch"){
                    C[id][cycle/interval]++;//the data we need load in the C[][]
                   // cout<<"id:"<<id<<"     time point:"<<cycle/interval<<endl;
                }
                fin2>>id>>pipeline>>cycle;
        }
        fin2.close();
        cout<<"Time series calculate"<<endl;

        char timeSeries[100];
        strcpy(timeSeries,"./output/");
        strcat(timeSeries,name);
        strcat(timeSeries,"_TS.txt");//timeSeries=./output/barnes_xy_TS.txt
        ofstream TS;
        TS.open(timeSeries);
        for(int i=0;i<NUM;i++){
            for(int j=0;j<C[i].size();j++){
                TS<<C[i][j]<<"  ";
            }			//put the data of C[][] into output file barnes_xy_TS.txt
            TS<<endl;
        }
        TS.close();
        cout<<"Time series output finished"<<endl;

        char corrSeries[100];
        strcpy(corrSeries,"./output/");
        strcat(corrSeries,name);
        strcat(corrSeries,"_corr.txt");//corrSeries:  ./output/barnes_xy_corr.txt
        double temp;
        ofstream fCorr;
        fCorr.open(corrSeries);
        for(int i=0;i<NUM;i++){
                for(int j=0;j<NUM;j++){
                    temp=calc_var(C[i],C[j]);//function calc_val can calculate the correlation of two vector
                    corr[i][j]=temp;//calc_var(C[i],C[j]);
                }
        }
        cout<<"calculate correlation"<<endl;

        for(int i=0;i<NUM;i++){
                for(int j=0;j<NUM;j++){
                    fCorr<<corr[i][j]<<"  ";
                }
                fCorr<<endl;
        }
        fCorr.close();
        cout<<"output correlation"<<endl;

//*************ting fei*******************//

	char ipcSeries[100];
	double sumIns[100];
        strcpy(ipcSeries,"./output/");
        strcat(ipcSeries,name);
        strcat(ipcSeries,"_ipc.txt");//corrSeries:  ./output/barnes_xy_corr.txt
      //  double temp;
        ofstream fipc;
        fipc.open(ipcSeries);
        for(int i=0;i<NUM;i++){
                for(int j=0;j<C[i].size();j++){
            sumIns[i]+=C[i][j];
        }
        }
        cout<<"calculate instructors sum"<<endl;

        for(int i=0;i<NUM;i++){
 //               fipc<<sumIns[i]<<"   "<<sumIns[i]/total_cycle; 
                fipc<<sumIns[i];           
                fipc<<endl;
        }
        fipc.close();
        cout<<"output instructors sum"<<endl;


        cSort.resize(NUM);
        cSortCorr.resize(NUM);//cSort and cSortCorr are vectors of struct{id value}
        for(int i=0;i<NUM;i++){
            cSort[i].ID=i;
            cSortCorr[i].ID=i;
            cSort[i].value=calc_means(C[i]);
            cSortCorr[i].value=0;
        }
        for(int i=0;i<NUM;i++){
                for(int j=0;j<NUM;j++){
                    cSortCorr[i].value+=calc_means(C[j])*corr[i][j];//calcultate each core correlation tast to other cores'
                }
        }

        char uSortAi[100];
        char uSortCorr[100];
        strcpy(uSortAi,"./output/");
        strcpy(uSortCorr,"./output/");
        strcat(uSortAi,name);
        strcat(uSortCorr,name);
        strcat(uSortAi,"_usort_ai.txt");
        strcat(uSortCorr,"_usort_corr.txt");
        ofstream fC;
        ofstream fCCorr;
        fC.open(uSortAi);
        fCCorr.open(uSortCorr);
        for(int i=0;i<NUM;i++){
            fC<<cSort[i].ID<<"   "<<cSort[i].value<<endl;
            fCCorr<<cSortCorr[i].ID<<"   "<<cSortCorr[i].value<<endl;
        }
        fC.close();
        fCCorr.close();
        cout<<"output cpu statistic"<<endl;

        sort(cSort.begin(),cSort.end(),sort_value);//sort is a very important and interesting function
        sort(cSortCorr.begin(),cSortCorr.end(),sort_value);
        char sortedAi[100];
        char sortedCorr[100];
        strcpy(sortedAi,"./output/");
        strcpy(sortedCorr,"./output/");
        strcat(sortedAi,name);
        strcat(sortedCorr,name);
        strcat(sortedAi,"_sorted_Ai.txt");
        strcat(sortedCorr,"_sorted_Corr.txt");
        cout<<sortedCorr<<endl;
        ofstream fSortedAi;
        ofstream fSortedCorr;
        fSortedAi.open(sortedAi);
        fSortedCorr.open(sortedCorr);
        for(int i=0;i<NUM;i++){
            fSortedAi<<"P   "<<cSort[i].ID<<"   "<<cSort[i].value<<"    "<<freq<<endl;
            fSortedCorr<<"P "<<cSortCorr[i].ID<<"   "<<cSortCorr[i].value<<"    "<<freq<<endl;
        }
        fSortedAi.close();
        fSortedCorr.close();

       // cout<<id<<"   "<<pipeline<<"   "<<total_cycle<<endl;
        return 1;       
}

double calc_var( vector<int> &x, vector<int> &y){
        double mean_x=calc_means(x);
        double mean_y=calc_means(y);
        double var=0;
        double up_factor=0;
        double down_factor_x=0;
        double down_factor_y=0;
        for(int i=0;i<x.size();i++){
            up_factor+=(x[i]-mean_x)*(y[i]-mean_y);
            down_factor_x+=(x[i]-mean_x)*(x[i]-mean_x);
            down_factor_y+=(y[i]-mean_y)*(y[i]-mean_y);
        }
        var=up_factor/(sqrt(down_factor_x*down_factor_y));
        return var;
}
double calc_means(vector<int> &x){
        double sum;
        for(int i=0;i<x.size();i++){
            sum+=x[i];
        }
        sum/=x.size();
        return sum;
}
bool sort_value(const item &x, const item &y){
    return x.value<y.value;

}
