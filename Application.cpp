#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include "Application.h"
#include "include/rapidjson/reader.h"
using namespace rapidjson;
using namespace std;
vector<Task> generateTasks(int id,int taskVol, int nT) {
	vector<Task> ttasks;
	vector<double> mask(nT, 0);
	for (int i = 0; i < nT; i++) {
		Task task(taskVol, id,ttasks.size());
		task.setWithNeibors(mask);
		ttasks.push_back(task);
	}
	return ttasks;
}

vector<Task> buildLine(vector<Task> tasks, int numTasks, double CCR,int fixedData) {
	if (numTasks < 2) {
		return tasks;
	}
	for (vector<Task>::iterator iter = (tasks.begin() + 1); iter != tasks.end(); iter++) {
		double data = CCR * (*(iter - 1)).getExecutionVolume() + fixedData;
		(*iter).bondingwith((*(iter - 1)).getId(),-data);
		(*(iter - 1)).bondingwith((*iter).getId(), data);
	}
	return tasks;
}

vector<Task> buildStar(vector<Task> tasks, int numTasks, double CCR, int fixedData) {
	for (vector<Task>::iterator iter = (tasks.begin() + 1); iter != tasks.end(); iter++) {
		int data = CCR * (*tasks.begin()).getExecutionVolume() + fixedData;
		(*tasks.begin()).bondingwith((*iter).getId(),-data);
		(*iter).bondingwith((*tasks.begin()).getId(),data);
	}
	return tasks;
}

void Application::buildTaskGraph()
{
	for (auto t : tasks) {
		taskGraph.push_back(t.getWithNeibors());
	}
}

Application::Application(int nT,double r, int aT, int i,int taskVol,int type,int fixedData) {
	CCR = r;
	numTasks = nT;
	arrivalTime = aT;
	id = i;

	if (type == 0) {
		tasks = buildStar(generateTasks(id, taskVol, numTasks), numTasks, CCR, fixedData);
	}else{
		tasks = buildLine(generateTasks(id, taskVol, numTasks), numTasks, CCR, fixedData);
	}
	buildTaskGraph();
}

Application::Application(string filename)
{
  cout<<filename<<endl;
        ifstream file(filename);
	file >> id;
	file >> arrivalTime;
	file >> numTasks;
	for (int i = 0; i < numTasks; i++) {
		int tid,ev;
		file >> tid;
		file >> ev;
		Task task(ev, id, tid);
		vector<double> withNeibors;
		for (int j = 0; j < numTasks; j++) {
			double cv;
			file >> cv;
			withNeibors.push_back(cv);
		}
		task.setWithNeibors(withNeibors);
		tasks.push_back(task);
	}
	buildTaskGraph();
}

Application::Application(int i, int aT, int nT, vector<Task> ts, vector<vector<double>> cvs)
{
	id = i;
	arrivalTime = aT;
	numTasks = nT;
	taskGraph = cvs;
	tasks = ts;
}

int Application::getArrivalTime() {
	return arrivalTime;
}

int Application::getNumTasks() {
	return  numTasks;
}

vector<Task> Application::getTasks() {
	return tasks;
}

int Application::getId() {
	return id;
}

vector<vector<double>> Application::getTaskGraph()
{
	return taskGraph;
}

void Application::saveAsFile(string filenamePrefix,int prefix)
{
	filenamePrefix = filenamePrefix.append(to_string(id + prefix)+".txt");
	ofstream file(filenamePrefix);
	file << id << endl;
	file << arrivalTime << endl;
	file << numTasks << endl;
	
	for (auto i = tasks.begin(); i != tasks.end();i++) {
		Task task = *i;
		file << task.getId() << "\t" << task.getExecutionVolume() << "\t";
		vector<double> withNeibors = task.getWithNeibors();
		for (auto j = withNeibors.begin(); j != withNeibors.end(); j++) {
			file << *j << "\t";
		}
		file << endl;
	}
}

