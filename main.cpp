#include <iostream>
#include <queue>
#include <stdlib.h>  
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include "Application.h"
using namespace std;

/*void readCMPFiles(string filename) {
	ifstream cmpTrace("trace_flow\\flow_"+filename+"_xy_org.log");
	int sx, sy, dx, dy, st, np;
	int marks[100][100];
	memset(marks, 0, sizeof marks);
	int markCount = 1;
	double communicateVolume[100][100];
	memset(communicateVolume, 0, sizeof communicateVolume);
	int startTime[100][100];
	memset(startTime, 0, sizeof startTime);
	while (cmpTrace >> sx >> sy >> dx >> dy >> st >> np) {
		if (marks[sx][sy] <= 0) {
			marks[sx][sy] = markCount;
			markCount++;
		}
		if (marks[dx][dy] <= 0) {
			marks[dx][dy] = markCount;
			markCount++;
		}
		communicateVolume[marks[sx][sy]][marks[dx][dy]] += np/50.0;
		communicateVolume[marks[dx][dy]][marks[sx][sy]] -= np/50.0;
		startTime[marks[sx][sy]][marks[dx][dy]] = st;
	}
	vector<Task> tasks;
	vector<vector<double>> cvs;
	int executionVolumes[100];
	memset(executionVolumes, 0, sizeof executionVolumes);
	ifstream efile("trace_cpu\\cpu_"+filename+"_xy.txt");
	int coreIndex;
	string s;
	int nosense;
	while (efile >> coreIndex >> s>>nosense) {
		executionVolumes[marks[coreIndex/8][coreIndex%8]]++;
	}
	for (int i = 1; i <= markCount; i++) {
		vector<double> wn;
		for (int j = 1; j <= markCount; j++) {
			wn.push_back(-communicateVolume[i][j]);
		}
		cvs.push_back(wn);
		Task t(executionVolumes[i - 1]/50, 0, i - 1, wn);
		tasks.push_back(t);
	}

	Application app(0, 0, markCount, tasks, cvs);
	app.saveAsFile("..\\app\\"+filename,0);
}*/
void generateApplications(int base,int numApplication, int numTasksRange, double CCR,int fixedData, int arrivalRate, int taskVol, string filenamePrefix,int type) {
	//queue<Event> events;

	for (int appCount = 0; appCount < numApplication; appCount++) {
		int nt = rand() % (numTasksRange/2) + numTasksRange/2;
		Application app(nt, CCR, arrivalRate, appCount + base, taskVol + rand()%200 - 200,type, fixedData);//task number
		type = 1;
		app.saveAsFile(filenamePrefix,0);
	}
	//return events;
}



int main(int argc, char* argv[]) {
	
	//int numApplication = atoi(argv[1]);// 16 32 64 128 //64
	//int numTasksRange = atoi(argv[2]);//8 16 24 32 //24
	//int taskExecutionVolume = atoi(argv[3]);//8 16 24 32 64 // 16
	//double CCR = atof(argv[4]); //threshold 0.1
	//double arrivalRate = atof(argv[5]);//0 2 4 6 // 4
	
	string applicationString = "app/wen/";
	//generateApplications(numApplication, numTasksRange, CCR, arrivalRate, taskExecutionVolume, applicationString);
	generateApplications(0,10,16, 0.0,75, 0, 1000, applicationString,0);
	//readCMPFiles("banres");
	//readCMPFiles("blackscholes");
	//readCMPFiles("canneal");
	//readCMPFiles("dedup");
	//readCMPFiles("raytrace");
	//readCMPFiles("ferret");
	//readCMPFiles("fluidanimate");
	//readCMPFiles("freqmine");
	//readCMPFiles("raytrace");
	//readCMPFiles("streamcluster");
	//readCMPFiles("swaptions");
	/*ofstream out("out");
	out << "this is it" << endl;*/
	printf("finish\n");
	return 0;
}
