#pragma once
#include "Application.h"
#include "Map.h"
#include "Clock.h"
#include "Region.h"
using namespace std;

class Event {
private:
	vector<InvokedTask> tasks;
	vector<vector<double>> taskGraph;
	int arrivalTime;
	int taskNum;
	int id;
	
	State state;
	int startTime;
	int finishTime;
	
public:
	Region region;
	Event(Application app);
	bool arrive(int now);
	void updateTaskGraph();
	void showTaskGraph();
	/*
	if the map is clean, the start running action fails
	if the map has been changed, but can't pass the thermal check, 
	the action need to abort
	otherwise, it starts successfully
	*/
	bool startRunning(Map* map,int type);
	
	Vertex book(Map map, int bubbleNum = 0,vector<Vertex> x = vector<Vertex>(0));// find the left top corner vertex
	bool mapping(Map* map,int type);
	/*
	the logic here is that,
	true means the map has changed
	false means the map is untouched(clean)
	*/
	void finish(Map* map);
	bool ifFinished(vector<InvokedTask> tasks)
	void unhookFromMap(Map* map);
	void nRound(Map* map);
	bool hasFinished();
	void wait();
	int getArrivalTime();
	int getStartTime();
	int getFinishTime();
	int getNumTasks();
	
	vector<InvokedTask> getTasks();

	int getId();
	bool setRegion(Vertex point, int region_width, int region_length);
	bool squareMapping(Map* map, Vertex point);
	bool chessMapping(Map* map, Vertex point);
	bool nonContigousMapping(Map* map)



	
};
