#include "Event.h"
#include <numeric>
#include <time.h>
using namespace std;

static double threshold = 80;

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

bool Event::startRunning(Map* map,int type, Vertex point) {
	state = running;
	startTime = Clock::getClock()->now();
	if (mapping(map,type, point)) {
		return true;	
	}
	unhookFromMap(map);
	return false;
}
	

bool Event::mapping(Map* map, int type, Vertex point) {
	/*default : square contigous map*/
	// Vertex pin = book(*map);
	switch (type) {
	
	case 1:
		return squareMapping(map,point);
	case 2:
		return chessMapping(map,point);
	case 3:
		return nonContigousMapping(map);
	case 4:
		return withBubbleMapping(map,point);
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


void Event::nRound(Map* map) {
	for (int i = 0; i < taskNum; i++) {/*for each task*/
			InvokedTask* t_cur = &tasks[i];
			Vertex v_cur = t_cur->getLandlord();
		       	Core* c = map->getSpecificCore(v_cur);
			double exe_speed = c->getFreq();
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
		cout<<"APP "<<getId()<<" finished"<<endl;
		cout<<" arrive at "<<arrivalTime<<" finished at "<<finishTime<<endl;
		cout<<" waiting time: " << startTime - arrivalTime<<endl;
		cout<<" execution time: " << finishTime - startTime<<endl;
		cout<<"migratetimes: "<<migratetimes<<endl;
		cout<<"migrate_distance: "<<migrate_distance<<endl;
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

	if (point.w+region_width <= 8)
		if (point.l + region_length <= 8)
		{
			Region r_tmp(point, region_width, region_length);
			this->region = r_tmp;
			return true;
		}
	return false;
}

bool Event::squareMapping(Map* map, Vertex point){
	int size = tasks.size();
	int region_width = floor(sqrt(size));
	int region_length = ceil(sqrt(size)) + region_width;
	int count = 0;
	if (!setRegion(point, region_width, region_length))
	{
		return false;
	}
	Vertex top_right_p(region.start_p.l,region.end_p.w);
	if (map->getSpecificCore(region.end_p)->active||map->getSpecificCore(top_right_p)->active)
	{
		return false;
	}
	cout<<"squaremap: "<<region.w<<"*"<<region.w<<endl;	
		for (int j = region.start_p.l; j <= region.end_p.l; j++) {
			for (int k = region.start_p.w; k <= region.end_p.w; k++) {
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
		//all tasks have been mapped already
	}
	else {/*unhook map*/
		unhookFromMap(map);
		cout<<" map failed" <<endl;
		return false;
	}
} 

bool Event::chessMapping(Map* map, Vertex point){
	int size = tasks.size();
	int region_width = 2*ceil(sqrt(size));
	int region_length = floor(sqrt(size));
	int count = 0;
	if (!setRegion(point, region_width, region_length))
	{
		return false;
	}
	Vertex top_right_p(region.start_p.l,region.end_p.w);
	if (map->getSpecificCore(region.start_p)->active||map->getSpecificCore(region.end_p)->active||map->getSpecificCore(top_right_p)->active)
	{
		return false;
	}
	cout<<"Chessmap: "<<region.w<<"*"<<region.l<<endl;
		for (int j = region.start_p.l; j <= region.end_p.l; j++) {
			if (j%2 == 0){
				for (int k = region.start_p.w; k <= region.end_p.w; k+=2) {
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
				for (int k = region.start_p.w + 1; k <= region.end_p.w; k+=2) {
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
bool Event::withBubbleMapping(Map* map, Vertex point){
	int size = tasks.size();
	int region_width = floor(sqrt(size))+1;
	int region_length = floor(sqrt(size)) + 1;
	int count = 0;
	if (!setRegion(point, region_width, region_length))
	{
		return false;
	}
	Vertex top_right_p(region.start_p.l,region.end_p.w);
	if (map->getSpecificCore(region.end_p)->active||map->getSpecificCore(top_right_p)->active)
	{
		return false;
	}
	cout<<"withbubblemap: "<<region.w<<"*"<<region.w<<endl;	
		for (int j = region.start_p.l; j <= region.end_p.l; j++) {
			for (int k = region.start_p.w; k <= region.end_p.w - 1; k++) {
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
		//all tasks have been mapped already
	}
	else {/*unhook map*/
		unhookFromMap(map);
		cout<<" map failed" <<endl;
		return false;
	}
} 
bool Event::nonContigousMapping(Map* map){
	int count = 0;
		for (int j =0; j < map->getLength(); j++) {
			for (int k =0; k < map->getWidth(); k++) {
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
bool Event::squareMigration(Map* map){
	int current_location = tasks[0].getLandlord().l;
	/*turn off active cores*/
	for (int i = 0; i < tasks.size();i++) {
				if (tasks[i].checkState() != unmapped) 
		map->turnOffCore(tasks[i].getLandlord());
				tasks[i].release();
				
	}
	/*Remapping*/
	int count = 0;
	cout<<"do Square Migration for app"<<getId()<<endl;
	int j=0;
	if (current_location == 0)
		/*move down*/
		j = region.w; 
	else/*move up*/
		j = 0 ;
	for (; j <= region.end_p.l; j++) {
			for (int k = region.start_p.w; k <= region.end_p.w; k++) {
				Vertex temp(j, k);
				if (!map->getSpecificCore(temp)->active) {
					map->turnOnCore(tasks[count], temp);
					tasks.at(count).continueRunning(temp);
					count++;
       
					//map one more task 	
				}if(count == tasks.size())
				   break;
			}if(count == tasks.size())
			   break;
		}
		

	if (count == tasks.size()) {
		migratetimes++;
		return true;
		//all tasks have been mapped already
	}
	else {/*unhook map*/
		unhookFromMap(map);
		cout<<" migration failed" <<endl;
		return false;
	}
	
}

bool Event::chessMigration(Map* map){
	int current_location = tasks[0].getLandlord().w;
	/*turn off active cores*/
	for (int i = 0; i < tasks.size();i++) {
				if (tasks[i].checkState() != unmapped) {
				map->turnOffCore(tasks[i].getLandlord());
				tasks[i].release();
			}
	}	

	int count = 0;
	int jplus = 0;
	if (current_location == 0)
	{
		jplus = 1;
		// cout<<"Right shift for first line"<<endl;
	}
	else
	{
		jplus = 0;
		// cout<<"Left shift for first line"<<endl;
	}
	cout<<"do chessMigration for app"<<getId()<<endl;
	for (int j = region.start_p.l; j <= region.end_p.l; j++) {
			if ((j +jplus) %2 == 0){
				for (int k = region.start_p.w; k <= region.end_p.w; k+=2) {
					Vertex temp(j, k);
					if (!map->getSpecificCore(temp)->active) {
						map->turnOnCore(tasks[count], temp);
						tasks.at(count).continueRunning(temp);
						count++;
						//map one more task 	
					}if(count == tasks.size())
							break;
				}
			}
			else{
				for (int k = region.start_p.w + 1; k <= region.end_p.w; k+=2) {
					Vertex temp(j, k);
					if (!map->getSpecificCore(temp)->active) {
						map->turnOnCore(tasks[count], temp);
						tasks.at(count).continueRunning(temp);
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
		migratetimes++;
		return true;
		//all tasks have benn mapped already
	}
	else {/*unhook map*/
		unhookFromMap(map);
		cout<<" migration failed" <<endl;
		return false;
		}
}

bool Event::globalColdest(Map* map, vector<int> hot_tid){
	/*Find coolest core and migrate*/
	cout<<"do GlobalColdest Migration for app"<<getId()<<endl;
	double coldest_tmp = threshold;
	Vertex  previous_vertex;
	Vertex coldest_vertex;
	for (auto tid : hot_tid) {
		cout <<"Hot task:"<<tid<<endl;
		previous_vertex = tasks[tid].getLandlord();
		map->turnOffCore(tasks[tid].getLandlord());
		tasks[tid].release();
		for(int k = 0; k < map->getLength(); k++) {
			for (int j = 0; j < map-> getWidth(); j++) {
				Vertex temp(k, j);
				if(map->getSpecificCore(temp)->getTaskId() == -1){
					double tmp_cur = map->getSpecificCore(temp)->temperture;
					if ( tmp_cur < coldest_tmp)
					{
						coldest_tmp = tmp_cur;
						coldest_vertex = temp;
					}
				}
			}
		}
		if (coldest_vertex.w == -1)
		{
			/*decrease the frequency, nowhere to migrate*/
			map->turnOnCore(tasks[tid], previous_vertex);
			map->getSpecificCore(previous_vertex)->setFreq(0.5);
			tasks[tid].continueRunning(previous_vertex);
		}
		else{
			/*add cost to total migrate distance*/
			migrate_distance += coldest_vertex.Manhattan(previous_vertex);
			map->turnOnCore(tasks[tid], coldest_vertex);
			tasks[tid].continueRunning(coldest_vertex);
			coldest_tmp = threshold;/*Find next coldest core*/
		}
		
	}	
	migratetimes++;		
	return true;
}

bool Event::localColdest(Map* map, vector<int> hot_tid){
	/*Find coolest core and migrate*/
	cout<<"do local Dark Migration for app"<<getId()<<endl;
	double coldest_tmp = threshold;
	Vertex  previous_vertex;
	Vertex coldest_vertex;
	for (auto tid : hot_tid) {
		cout <<"Hot task:"<<tid<<endl;
		previous_vertex = tasks[tid].getLandlord();
		map->turnOffCore(tasks[tid].getLandlord());
		tasks[tid].release();
		for(int j = region.start_p.l; j <= region.end_p.l; j++) {
			for (int k = region.start_p.w; k <= region.end_p.w; k++){
				Vertex temp(j, k);
				if(map->getSpecificCore(temp)->getTaskId() == -1){
					double tmp_cur = map->getSpecificCore(temp)->temperture;
					if ( tmp_cur < coldest_tmp)
					{
						coldest_tmp = tmp_cur;
						coldest_vertex = temp;
					}
				}
			}
		}
		if (coldest_vertex.w == -1)
		{
			/*decrease the frequency, nowhere to migrate*/
			map->turnOnCore(tasks[tid], previous_vertex);
			map->getSpecificCore(previous_vertex)->setFreq(0.5);
			tasks[tid].continueRunning(previous_vertex);
		}
		else{
			/*add cost to total migrate distance*/
			migrate_distance += coldest_vertex.Manhattan(previous_vertex);
			map->turnOnCore(tasks[tid], coldest_vertex);
			tasks[tid].continueRunning(coldest_vertex);
			coldest_tmp = threshold;/*Find next coldest core*/
		}
		
	}	
	migratetimes++;		
	return true;
}