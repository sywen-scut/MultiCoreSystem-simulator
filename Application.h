#include <queue>
#include "Task.h"
const int Min = 0;
class Application {
private:
	vector<Task> tasks;
	vector< vector<double> > taskGraph;
	int numTasks;
	int arrivalTime;
	int id;
	double CCR;
	void buildTaskGraph();
public:
	Application(int nT = 0,double r = 1.0, int aT = 0, int i = 0,int taskVol = 0,int type = 0,int fixedData = 100);
	Application(string filename);
	Application(int i,int aT,int nT,vector<Task>tvs, vector<vector<double> > cvs);
	int getArrivalTime();
	int getNumTasks();
	vector<Task> getTasks();
	int getId();
	vector<vector<double> > getTaskGraph();
	void saveAsFile(string filenamePrefix,int prefix);
};


