#include "Event.h"
#include <numeric>
#include <fstream>
#include <time.h>
using namespace std;

int MaxTime(const int m, vector<vector <int> >& map_graph, vector<int>& weight_value)
{
	const int n = m;//node number
	int des = 0, num = 0, ff = 0;
	int max_value = Min;//des check whether it is a destination node;min_value use to compute the maxmine value
	vector <int> comp, inde;
	//dist are distance from 0 node to other nodes; inde, comp, num, ff use to find the minimun distance
	vector <int> dist;
	dist.resize(m);
	inde.resize(m);
	comp.resize(m);
	for (int i = 0; i<n; i++)//add weight of the map_graph
		for (int j = 0; j<n; j++)
		{
			if (map_graph[i][j] != Min)
				map_graph[i][j] += weight_value[j];
		}
	for (int i = 0; i<n; i++)//initialize the dist and inde
	{
		dist[i] = map_graph[0][i];
		inde[i] = false;
	}
	inde[0] = true;
	while (des != (n - 1))
	{
		num = 0;
		max_value = Min;
		for (int i = 1; i<n; i++)
		//record the nodes which is not labeled and we will find the minimum node from these nodes 
		{
			if (inde[i] == false)
				comp[num++] = i;
		}
		for (int i = 0; i<num; i++)//find the minimum node's value
		{
			ff = comp[i];
			max_value = max(dist[ff], max_value);
		}
		for (int i = 0; i<num; i++)
		//find the minimum node's number by minimum value
		{
			ff = comp[i];
			if (dist[ff] == max_value)
				des = ff;
		}
		inde[des] = true;//we find the minimum value and mark this node true
		for (int i = 0; i<n; i++)//change the dist to this minimum node and add the minimum value
		{
			if (inde[i] == false)
				dist[i] = map_graph[des][i] + max_value;
		}
	}
	for (int i = 1; i<n; i++)//add the first node's weight value
	{
		dist[i] += weight_value[0];
	}

	//cout << endl;
	return dist[n - 1];
}
int calculateTimeFromMap(vector<vector <int> > edges,vector<int> nodes) {
	int executionTime = *(max_element(nodes.begin(),nodes.end()));
	int communicationTime = *(max_element(edges[0].begin(), edges[0].end()));
	return executionTime + communicationTime;
}
int variance(vector<int> a) {
	return *max_element(a.begin(), a.end()) - (*min_element(a.begin(), a.end()));
}
bool distanceComp(vector<int>a, vector<int>b) {

	bool small = true;

	if (a.size() == 0 && b.size() == 0) {
		return false;
	}
	if (a.size() == 0) {
		return true;
	}
	if (b.size() == 0) {
		return false;
	}

	int sumA = std::accumulate(a.begin(), a.end(), 0);
	int sumB = std::accumulate(b.begin(), b.end(), 0);
	if (sumA > sumB) {
		return !small;
	}
	else {
		if (sumA < sumB) {
			return small;
		}
		else {

			int varianceA = variance(a);
			int varianceB = variance(b);
			if (varianceA > varianceB) {
				return small;
			}
			else {
				return !small;
			}
		}
	}
}
double thermalModel(int sum2, int centerP, int sum12, int sum11) {
	//return 59.9 + 0.0097*centerP + 0.0008*sum12 + 0.0002*sum11 + 0.00015*(sum2 - sum12 - sum11);
	return 20;
}
int thermalThreshold = 60;
bool thermalCheckCore(Vertex v, Map map) {
	return false;
}
bool thermalCheckMap(Map map) {
	return false;
}


Event::Event(Application a) {//warning: havn't initialize shape yet
	vector<Task> raw = a.getTasks();
	for (auto t : raw) {
		tasks.push_back(InvokedTask(t));
	}
	taskGraph = a.getTaskGraph();
	arrivalTime = a.getArrivalTime();
	numTasks = tasks.size();
	id = a.getId();
}

bool Event::arrive(int now) {
	if (arrivalTime == now) {
		return true;
	}
	return false;
}

void Event::updateTaskGraph()
{
	for (int i = 0; i < numTasks; i++) {
		taskGraph[i] = tasks[i].unprocessedData();
	}
}

void Event::showTaskGraph()
{
	for (auto i : taskGraph) {
		for (auto j : i) {
			cout << j << '\t';
		}
		cout << endl;
	}
}

bool Event::startRunning(Map* map,int type) {
	state = running;
	startTime = Clock::getClock()->now();
	if (settleDown(map,type)) {
		//map->showMap();
		if (thermalCheckMap(*map)) {
			unhookFromMap(map);
			return false;
		}
		else {
			return true;
		}
	}
	return false;
}
	
// bool Event::mapping(Vertex pin, Map* map, int type)	{

// 	return cloestMapping_pin(pin, map);
// }

bool Event::mapping(Map* map,int type = 0) {
	//return randomMapping(map);
	//return serialMapping(pin,shape, map);
	//return CEMapping(pin, shape, map);
	//return farestMapping_pin(pin, map);
	Vertex pin = book(*map);
	//if (pin == Vertex(-1, -1, -1)) {
	//	//map->showMap();
	//	return false;
	//}
	
	switch (type) {
	case 0:
		return cloestMapping(map, 0);
	//	// return  fillMapping(map);
	//	return cloestMapping_pin(pin,map);
	case 1:
		return HWL(map);
	case 2:
		return LHW(map);
	//case 3:
		//return proposedMapping(map);
	}
	//return !thermalCheckMap(*map);
}

bool Event::settleDown(Map* map,int type) {
	// Vertex pin = book(*map);
	// if (pin == Vertex(-1,  -1)) {
	// 	map->showMap();
	// 	return false;
	// }
	// vector<Vertex> network;
	// bool valid = mapping(pin, map, type);
	// int count = 0;
	// while (!valid) {
	// 	network.push_back(pin);
	// 	pin = book(*map,0, network);
	// 	valid = mapping(pin, map, type);
	// 	count++;
	// 	if (count == map->getAvailableCores()) {
	// 		break;
	// 	}
	// }
	
	return mapping(map, type);
	//Shape shape = packing(*map);
	//return valid;
}

double Event::communicationCost(Map map) {
	double communicationCost = 0;
	for (auto task:tasks) {
		vector<int> p = task.getParents();
		vector<double> v = task.getWithNeibors();
		if (!p.empty()) {
			for (auto pi: p) {
				communicationCost += (tasks[pi].getLandlord().Manhattan(task.getLandlord()) - abs(tasks[pi].getLandlord().l - task.getLandlord().l)) * v[pi];
			}
		}
	}

	return communicationCost;
}

void Event::finish(Map* map) {
	state = finished;
	finishTime = Clock::getClock()->now();
	unhookFromMap(map);
}

void Event::unhookFromMap(Map* map) {
	for (int i = 0; i < numTasks;i++) {
		if (tasks[i].checkState() != unmapped) {
			map->turnOffCore(tasks[i].getLandlord());
			tasks[i].release();
		}
	}
	
}

int Event::calculateRemainingTime(Map* map) {
	vector<vector<int>> edges;
	vector<int> nodes;
	for (int i = 0; i < numTasks;i++) {
		auto task = tasks[i];
		auto c = map->getSpecificCore(task.getLandlord());
		vector<int> line;
		for (int j = 0; j < numTasks; j++) {
			if (i != j) {
				double data = taskGraph[i][j] > 0 ? taskGraph[i][j] : 0;
				int length = task.getLandlord().Manhattan(tasks[j].getLandlord()) - abs(task.getLandlord().l - tasks[j].getLandlord().l);
				line.push_back((data + 0.5) / (length * c->getInputSpeed()));
			}
			else {
				line.push_back(0);
			}
		}
		edges.push_back(line);

		nodes.push_back(task.predictExecutionTime(c->getExecutionSpeed()));
	}

	int eT = 0;
	for (auto each : nodes) {
		eT += each;
	}

	int cT = 0;
	for (auto line : edges) {
		for (auto each : line) {
			cT += each;
		}
	}
	return eT + cT;
	//return 1;
}

void Event::nRound(Map* map, int n) {
	for (int i = 0; i < numTasks; i++) {
		InvokedTask* t = &tasks[i];
		Core* c = map->getSpecificCore(t->getLandlord());
		int es = c->getExecutionSpeed();
		vector<double> input;
		vector<double> output;
		vector<double> need = t->unprocessedData();
		for (int j = 0; j < numTasks;j++) {
			if (i != j && need[j] != 0) {
				int length = tasks[j].getLandlord().Manhattan(t->getLandlord()) - abs(t->getLandlord().l - tasks[j].getLandlord().l);
				if (tasks[j].getLandlord().h == t->getLandlord().h && tasks[j].getLandlord().w == t->getLandlord().w) {
					input.push_back(c->getVerticalSpeed());
					output.push_back(-c->getVerticalSpeed());
				}
				else {
					input.push_back(c->getInputSpeed() / (double)length);
					output.push_back(-c->getOutputSpeed() / (double)length);
				}
			}
			else {
				input.push_back(0);
				output.push_back(0);
			}
		}

		bool tick = true;
		vector<int> p = t->getParents();
		State s = t->checkState();
		switch (s) {
		case waiting:
			if (p.empty()) {
				t->setState(executing);
			}
			else {
				tick = false;
				for (auto pi : p) {
					if (tasks[pi].checkState() == tranfering) {
						tick = true;
					}
				}
				if (tick) {
					t->setState(receiving);
				}
			}
			break;
		case receiving:
			t->nRound(0, input);
			break;
		case executing:
			t->nRound(es, vector<double>(numTasks,0));
			break;
		case tranfering:
			t->nRound(0, output);
			break;
		case finished:
			t->nRound(0, vector<double>(numTasks, 0));
			break;
		}
	}
	
	updateTaskGraph();
	if (calculateRemainingTime(map) == 0) {
		finish(map);
	}
}

bool Event::hasFinished() {
	return state == State::finished;
}

void Event::wait() {
	state = waiting;
}

int Event::getArrivalTime() {
	return arrivalTime;
}

int Event::getStartTime() {
	return startTime;
}

int Event::getFinishTime() {
	return finishTime;
}

int Event::getNumTasks() {
	return  numTasks;
}

vector<InvokedTask> Event::getTasks() {
	return tasks;
}

int Event::getId() {
	return id;
}

bool Event::LHW(Map * map)
{
	int count = 0;
	for (int k =0; k <map->getWidth() && count < numTasks; k++) {
		for (int i = 0; i < map->getHeight() && count < numTasks; i++) {
			for (int j =0; j < map->getLength() && count < numTasks; j++) {
				Vertex temp(j, i, k);
				if (!map->getSpecificCore(temp)->checkState()) {
					if (!thermalCheckCore(temp, *map)) {
						map->turnOnCore(tasks[count], temp);
						tasks.at(count).startRunning(temp);
						count++;
					}
				}
			}
		}
	}
	if (count == numTasks) {
		return true;
	}
	else {
		unhookFromMap(map);
		return false;
	}
}

bool Event::HWL(Map * map)
{
	int count = 0;
	for (int k = 0; k <map->getLength() && count < numTasks; k++) {
		for (int i = 0; i < map->getHeight() && count < numTasks; i++) {
			for (int j = 0; j < map->getWidth() && count < numTasks; j++) {
				Vertex temp(k, i,j);
				if (!map->getSpecificCore(temp)->checkState()) {
					if (!thermalCheckCore(temp, *map)) {
						map->turnOnCore(tasks[count], temp);
						tasks.at(count).startRunning(temp);
						count++;
					}
				}
			}
		}
	}
	if (count == numTasks) {
		return true;
	}
	else {
		unhookFromMap(map);
		return false;
	}
}
