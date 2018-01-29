#pragma once
#include "Application.h"
#include "Map.h"
#include "Clock.h"
#include "Shape.h"
using namespace std;

class Event {
private:
	vector<InvokedTask> tasks;
	vector<vector<double>> taskGraph;
	int arrivalTime;
	int numTasks;
	int id;
	
	State state;
	int startTime;
	int finishTime;
	
public:
	Event(Application a);
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
	Shape packing(Map map, int bubbleNum = 0);
	Vertex book(Map map, int bubbleNum = 0,vector<Vertex> x = vector<Vertex>(0));// find the left top corner vertex
	bool mapping(Map* map,int type);//can the map pass the thermal check
	/*
	the logic here is that,
	true means the map has changed
	false means the map is untouched(clean)
	*/
	bool settleDown(Map* map,int type);//has the map changed?
	double communicationCost(Map map);
	void finish(Map* map);
	void unhookFromMap(Map* map);
	int calculateRemainingTime(Map* map);
	/*
	the tricky thing here is that,
	the executionVolume and the communicationVolume decrease every round
	So, we can only make the communication influence the running time 
	when the communication time is  greater than execution time
	for 100 execution volume, CCR~5 do the trick.
	*/
	void nRound(Map* map, int n = 1);
	bool hasFinished();
	void wait();
	int getArrivalTime();
	int getStartTime();
	int getFinishTime();
	int getNumTasks();
	vector<InvokedTask> getTasks();
	int getId();

	/*3D mapping*/
	bool LHW(Map* map);
	bool HWL(Map* map);
	bool mappingOnLoc(Vertex loc, int regionw, int regionh, Map* map);
	bool regionAvail(Vertex loc, int lowerLayer, int height, Map* map);
	bool checkAvailable(int SL, int height, Map* map);
	bool mappingGivenDHandHeight(int lowerLayer, int height, Map* map);
	vector<double> extractDHandHeight(Map*map);
	bool proposedMapping(Map* map);
	bool aminMapping(Map * map);
	/*
	the mapping functions need to return 
	whether the layout can pass the thermal check
	*/
	/*mapping methods*/
	//bool serialMapping(Vertex pin,Shape shape, Map* map);
	//bool farestMapping(Vertex pin, Shape shape, Map* map);
	//bool CEMapping(Vertex pin, Shape shape, Map* map);
	//bool CEMapping_b(Vertex pin, Shape shape, Map* map);
	//bool randomMapping(Map* map);


	//bool proposedMapping(Map* map);
	//bool farestMapping_pin(Map* map);
	//bool cloestMapping_pin(Vertex pin, Map* map, int bubbleNum=0);

	//bool fillMapping(Map* map);//fill the holes
};
