#include "Core.h"

Core::Core(double es, double is, double os) {
	int core_id;
	coordinate.w = 8;
	coordinate.l = 8;
	runFreq = es;
	inputSpeed = is;
	outputSpeed = os;
	active = false;
}

void Core::turnOn(int a_id, int t_id) {
	applicationId = a_id;
	taskId = t_id;
	active = true;
}

void Core::turnOff() {
	application_id = -1;
	task_id = -1;
	active = false;
}

bool Core::checkState() {
	if (applicationId >= 0) {
		return true;
	}
	return false;
}

Vertex Core::getCoordinate()
{
	return coordinate;
}

int Core::getApplicationId() {
	return application_id;
}

double Core::getFreq() {
	return runFreq;
}

double Core::getInputSpeed() {
	return inputSpeed;
}

double Core::getOutputSpeed()
{
	return outputSpeed;
}


