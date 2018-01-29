#include <vector>
#include <queue>
#include <cmath>
#include "Vertex.h"
#include "general.h"
#include <iostream>

using namespace std;
class Task {
private:
	int executionVolume;
	vector<double> withNeibors;// >0, parent;
	
	int id;
	int applicationId;

public:
	Task(int ev, int aId, int tid);
	Task(int ev, int aId, int tid,vector<double>ns);
	int getExecutionVolume();
	int getApplicationId();
	int getId();
	vector<double> getWithNeibors();

	void setWithNeibors(vector<double> mask);
	void bondingwith(int another,double dataSize);// type 0,child; type 1, parent;
};

class InvokedTask {
private:
	int executionVolume;
	
	vector<double> withNeibors;
	int id;
	int applicationId;

	
	Vertex landlord; //mark the core 
	State state;
	vector<int> parents;
	vector<int> children;
	void recognize();
public:
	vector<double> dataProcessed;
	int executionVolumeFinished;
	InvokedTask(Task task) {
		executionVolume = task.getExecutionVolume();
		withNeibors = task.getWithNeibors();
		state = unmapped;
		id = task.getId();
		applicationId = task.getApplicationId();
		executionVolumeFinished = 0;
		state = unmapped;
		recognize();
		dataProcessed = vector<double>(withNeibors.size(), 0);
	}

	
	void nRound(int es, int nT);
	int predictExecutionTime(int execS);
	vector<double> unreceivedData();
	vector<double> untransferedData();
	vector<double> unprocessedData();
	//int unloadedData();
	void startRunning(Vertex v);
	void continueRunning(Vertex v);
	bool hasFinished();
	void release();
	State checkState();
	void setState(State);
	
	vector<double> getDataProcessed();
	Vertex getLandlord();
	int getApplicationId();
	vector<double> getWithNeibors();
	int getId();
	vector<int> getParents();
	vector<int> getChildren();
	int getExecutionVolume();
	int getExecutionVolumeFinished();
};