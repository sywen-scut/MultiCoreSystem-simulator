#include <fstream>
#include <string>
#include <iostream>
#pragma once
#include <fstream>
#include "Application.h"
using namespace std;

class Core {
private:
	double Freq;
	int application_id;
	int task_id;
public:
	Vertex location;
	bool active;
	double p_power_leakage = 0.1; //default normalize
	double p_power_dynamic = 0;
	double r_power = 0;
	double temperture;
	
	Core(double runFreq, int index);

	void setFreq(double freq);


	/*turn on the core running specific task*/
	void turnOn(int a_id, int t_id);

	/*turn off the core*/
	void turnOff();

	void turnOn_Processor();

	void turnOff_Processor();

	void turnOn_router();

	void turnOff_router();

	// Vertex getCoordinate();
	int getApplicationId();

	int getTaskId();

	double getFreq();

};