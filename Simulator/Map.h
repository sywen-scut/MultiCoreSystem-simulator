#pragma once
#include <iostream>
#include <vector>
#include "Core.h"
using namespace std;

class Map {
private:
	vector<Core> cores;
	int width = 8;
	int length = 8;
	int availableCores;
public:
	Map(double es = 2,double is = 1,double os = 1);
	//void updateAvailableCores(int i);
	bool turnOnCore(InvokedTask task,Vertex ver);
	bool turnOffCore(Vertex ver);
	int getAvailableCores();
	int getRunningTime();
	Core* getSpecificCore(Vertex v);
	Core* getSpecificCore(int index);
	int toIndex(Vertex v);
	Vertex toVertex(int i);
	void showMap();
	void extractMapFile();
	void writePtraceFile(string fn);
	void writeFlpsFiles(string fn);
};
