#include "Event.h"
#include <numeric>
#include <time.h>
using namespace std;

Event::Event(Application app) {//warning: havn't initialize shape yet
	vector<Task> row = app.getTasks();
	for (vector<Task>::iterator iter = row.begin(); iter != row.end(); ++iter)
		{
		tasks.push_back(InvokedTask(*iter));
		}
	taskGraph = app.getTaskGraph();
	arrivalTime = app.getArrivalTime();
	taskNum = tasks.size();
	id = app.getId();
}

bool Event::arrive(int now) {
	if (arrivalTime == now) {
		return true;
	}
	return false;
}

void Event::updateTaskGraph(){
	
	for (int i = 0; i < taskNum; i++) {
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
	if (mapping(map,type)) {
		return true;	
	}
	unhookFromMap(map);
	return false;
}
	

bool Event::mapping(Map* map, int type = 1) {
	/*default : square contigous map*/
	Vertex pin = book(*map);
	switch (type) {
	
	case 1:
		return squareMapping(map);
	case 2:
		return chessMapping(map);
	case 3:
		return nonContigousMapping(map);
	}
}

void Event::finish(Map* map) {
	state = finished;
	finishTime = Clock::getClock()->now();
	unhookFromMap(map);
}

bool Event::ifFinished(vector<InvokedTask> tasks){
	for (int i = 0; i < tasks.size();i++) {
		if (tasks[i].checkState() != finished)
			return false;
		}
		return true;
}

void Event::unhookFromMap(Map* map) {
	for (int i = 0; i < taskNum;i++) {
		if (tasks[i].checkState() != unmapped) {
			map->turnOffCore(tasks[i].getLandlord());
			tasks[i].release();
		}
	}
	
}


void Event::nRound(Map* map, int n) {
	for (int i = 0; i < taskNum; i++) {/*for each task*/
			InvokedTask* t_cur = &tasks[i];
			Vertex v_cur = t_cur->getLandlord();
		       	Core* c = map_t->getSpecificCore(v_cur);
			int exe_speed = c->getFreq();
			vector<double> send;
			vector<double> recieve;
			vector<double> need = t_cur->unprocessedData();
			for (int j = 0; j < taskNum; ++j)/*colunm*/
			{
				if (need[j]!=0)
				{
					int distance = tasks[j].getLandlord().Manhattan(t_cur->getLandlord());
					if (need[j]>0) 
					{
						
						recieve.push_back(1.0/distance);
						send.push_back(0);
						
						
					}
					if(need[j]<0)
					{
						recieve.push_back(0);
						send.push_back(1.0/distance);
					}
					
				}
				else{
					send.push_back(0);
					recieve.push_back(0);
				}
			}
			bool tick = true;
			vector<int> parent = t_cur->getParents();
			State s = t_cur->checkState();
			switch (s) {
			case waiting:
				if (parent.empty()) {
					t_cur->setState(executing);
				}
				else {
					tick = false;
					for (auto pi : parent) {
						if (tasks[pi].checkState() == tranfering) {
							tick = true;
						}
					}
					if (tick) {
						t_cur->setState(receiving);
					}
				}
				break;
			case receiving:
				c->turnOn_router();
				for (int i = 0; i < taskNum; ++i)
				{
					if (tasks[i].checkState() == tranfering || tasks[i].checkState() == finished)
					{
						t_cur-> dataProcessed[i] += recieve[i];
					}
				}
					t_cur->nRound(0, taskNum);
				
				break;
			case executing:
				c->turnOff_router();
				c->turnOn_Processor();
				t_cur ->executionVolumeFinished += exe_speed;
				t_cur->nRound(exe_speed, taskNum);
				break;
			case tranfering:
				c->turnOff_Processor();
				c->turnOn_router();
				for (int i = 0; i < taskNum; ++i)
				{
					t_cur-> dataProcessed[i] -= send[i];
				}
				t_cur->nRound(0,taskNum);
				break;
			case finished:
				t_cur->nRound(0, taskNum);
				c->turnOff_router();
				break;
			}	
	}
	updateTaskGraph();
	
	if (ifFinished(tasks)) {
		finish(map);
		cout<<"APP "<<getId<< endl;
		cout<<" arrive at "<<arrivalTime<<" finished at "<<finishTime<<endl;
		cout<<" waiting time: " << startTime - arrivalTime<<endl;
		cout<<" execution time: " << finishTime - startTime<<endl;
	}
}

bool Event::hasFinished() {
	return state == State::finished;
	/*finished or not*/
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
	return  taskNum;
}

vector<InvokedTask> Event::getTasks() {
	return tasks;
}

int Event::getId() {
	return id;
}

bool Event::setRegion(Vertex point, int region_width, int region_length){

	if (point.w+region_width < map->getWidth())
		if (point.l + region_length < map->getLength())
		{
			Region r_tmp = (Vertex point, region_width, region_length);
			region = r_tmp;
			return true;
		}
	return false;
}

bool Event::squareMapping(Map* map, Vertex point){
	int size = tasks.size();
	int region_width = floor(sqrt(size));
	int region_length = ceil(sqrt(size)) + region_width;
	int count = 0;
	if (!setRegion(Vertex point, region_width, region_length))
	{
		return false;
	}
	cout<<"squaremap: "<<region.w<<"*"<<region.w<<endl;	
		for (int j = region.start_p.l; j <= region.end_p.l; j++) {
			for (int k = start_p.w; k <= region.end_p.w; k++) {
				Vertex temp(j, k);
				if (!map->getSpecificCore(temp)->active) {
					map->turnOnCore(tasks[count], temp);
					tasks.at(count).startRunning(temp);
					count++;
       
					//map one more task 	
				}if(count == tasks.size())
				   break;
			}if(count == tasks.size())
			   break;
		}
		

	if (count == tasks.size()) {
		return true;
		//all tasks have benn mapped already
	}
	else {/*unhook map*/
		unhookFromMap(map);
		cout<<" map failed" <<endl;
		return false;
	}
} 

bool Event::chessMapping(Map* map){
	int size = tasks.size();
	int region_width = 2*ceil(sqrt(size));
	int region_length = floor(sqrt(size));
	int count = 0;
	if (!setRegion(Vertex point, region_width, region_length))
	{
		return false;
	}

	cout<<"Chessmap: "<<region.w<<"*"<<region.l<<endl;
		for (int j = region.start_p.l; j <= region.end_p.l; j++) {
			if (j%2 == 0){
				for (int k = start_p.w; k <= region.end_p.w; k+=2) {
					Vertex temp(j, k);
					if (!map->getSpecificCore(temp)->active) {
						map->turnOnCore(tasks[count], temp);
						tasks.at(count).startRunning(temp);
						count++;
						//map one more task 	
					}if(count == tasks.size())
							break;
				}
			}
			else{
				for (int k = start_p.w + 1; k <= region.end_p.w; k+=2) {
					Vertex temp(j, k);
					if (!map->getSpecificCore(temp)->active) {
						map->turnOnCore(tasks[count], temp);
						tasks.at(count).startRunning(temp);
						count++;
						//map one more task 	
					}if(count == tasks.size())
							break;
				}
			}
			if(count == tasks.size())
				break;

		}
	if (count == tasks.size()) {
		return true;
		//all tasks have benn mapped already
	}
	else {/*unhook map*/
		unhookFromMap(map);
		cout<<" map failed" <<endl;
		return false;
		}
	

}

bool Event::nonContigousMapping(Map* map){
	return true;
}
