#pragma once
#include "Task.h"
class Core {
private:
	Vertex coordinate;
	double inputSpeed;
	double outputSpeed;
	double verticalSpeed;
	double runFreq;
	int application_id;
	int task_id;

	static const int speedLevel[2];
public:
	int core_id;
	bool active;
	double processorPower;
	double routerPower;
	/*core needs position and speeds*/
	Core(double runFreq, double inputSpeed,double outputSpeed);

	/*turn on the core running specific task*/
	void turnOn(int a_id, int t_id);

	/*turn off the core*/
	void turnOff();

	/*returns true if the core is busy,false otherwise*/
	bool checkState();

	Vertex getCoordinate();
	int getApplicationId();
	double getFreq();
	double getInputSpeed();
	double getOutputSpeed();
};
