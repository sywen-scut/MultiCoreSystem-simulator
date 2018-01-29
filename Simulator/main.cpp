#include <iostream>
#include <string>
#include <queue>
#include <stdlib.h>  
#include <algorithm>
#include <cmath>
#include <fstream>
#include "Event.h"
using namespace std;
long cycle = 0;

queue<Event> readEventFromFile(int numApplication, string filenamePrefix) {
	queue<Event> results;
	for (int i = 0; i < numApplication; i++) {
		Application app(filenamePrefix + to_string(i) + ".txt");
		Event e(app);
		results.push(e);
	}

	return results;
}

int main(int argc,char* argv[]) {
	
	
	int numApplication = 10;
	queue<Event> events;
	
	// Application banresApp("..\\app\\banres0.txt");
	// Event banres(banresApp);
	// events.push(banres);
	
	// Application blackscholesApp("..\\app\\blackscholes0.txt");
	// Event blackscholes(blackscholesApp);
	// events.push(blackscholes);

	// Application cannealApp("..\\app\\canneal0.txt");
	// Event canneal(cannealApp);
	// events.push(canneal);

	// Application dedupApp("..\\app\\dedup0.txt");
	// Event dedup(dedupApp);
	// events.push(dedup);

	// Application ferretApp("..\\app\\ferret0.txt");
	// Event ferret(ferretApp);
	// events.push(ferret);
	
	// Application fluidanimateApp("..\\app\\fluidanimate0.txt");
	// Event fluidanimate(fluidanimateApp);
	// events.push(fluidanimate);

	// Application freqmineApp("..\\app\\freqmine0.txt");
	// Event freqmine(freqmineApp);
	// events.push(freqmine);

	// Application raytraceApp("..\\app\\raytrace0.txt");
	// Event raytrace(raytraceApp);
	// events.push(raytrace);

	// Application streamclusterApp("..\\app\\streamcluster0.txt");
	// Event streamcluster(streamclusterApp);
	// events.push(streamcluster);

	// Application swaptionsApp("..\\app\\swaptions0.txt");
	// Event swaptions(swaptionsApp);
	// events.push(swaptions);
	Application testApp("/app/wen/7.txt")
	Event test(testApp);
	events.push(test);
	
	long cycle = 0;
	double commCost = 0;

	vector<Event> waiting;
	vector<Event> running;
	vector<Event> finished;

	Clock* clock = Clock::getClock();
	
	//Map map(4,20,20,100,10,10,0);// 8 12 16 20 //8
	Map map(4, 8, 8, 100, 10, 10, 0);
	int type = 1;//1 B2T 2 FL 


/*	int numApplication = 12;
	string applicationString = "..\\app\\self\\tn\\16\\";
	queue<Event> events = readEventFromFile(numApplication, applicationString);
	*/
	string fn ="..\\real\\"+ to_string(map.getLength()) + "-" + to_string(map.getHeight()) + "-" + to_string(map.getWidth()) + "_" +to_string(type);
	while (finished.size() != numApplication) {
		for (vector<Event>::iterator iter = waiting.begin(); (!waiting.empty())&&iter != waiting.end();) {
			if ((*iter).getNumTasks() <= map.getAvailableCores() && (*iter).startRunning(&map,type)) {
				map.showMap();
				commCost += (*iter).communicationCost(map);
				running.push_back(*iter);
				iter = waiting.erase(iter);
			}else{
				iter++;
			}
		}

		while (!events.empty()) {
			Event app = events.front();
			if (app.arrive(clock->now())) {
				if (app.getNumTasks() < map.getAvailableCores()&& app.startRunning(&map,type)) {
					map.showMap();
					running.push_back(app);
					commCost += app.communicationCost(map);
					events.pop();
				}
				else {
					app.wait();
					waiting.push_back(app);
					events.pop();
				}
			}
			else {
				break;
			}

		}

		for (vector<Event>::iterator iter = running.begin(); iter != running.end();) {
			(*iter).nRound(&map);
			if ((*iter).hasFinished()) {
				finished.push_back(*iter);
				iter = running.erase(iter);
			}
			else {
				iter++;
			}
		}
		map.writePtraceFile(fn);
		clock->tick();
		
		//cout <<"time:"<< clock->now()<<endl;
	//	cout <<"waiting:"<< waiting.size() << endl;;
	//	cout <<"running:"<< running.size()<<endl;
	//	cout << "finished:" << finished.size() << endl;
	//	cout <<"other"<< events.size()<<endl;

	}

	/*int waitingTimeSum = 0;
	for (auto iter = finished.begin(); iter != finished.end(); iter++) {
		waitingTimeSum += (*iter).getStartTime() - (*iter).getArrivalTime();
	}*/
	cout << "running time: " << clock->now() << endl;
	/*cout <<"waiting time: "<< waitingTimeSum << endl;*/
	cout << "communication cost:" << commCost << endl;

	//ofstream myfile;
	//myfile.open("result_barnes.txt",ios::app);
	//myfile << map.getHeight()<<"*"<<map.getWidth()<<"\t"<<type<<endl;
	//myfile << "running time: " << clock->now() << endl;
	////myfile << "waiting time: " << waitingTimeSum << endl;
	//myfile << "communication cost:" << commCost << endl;
	//myfile << endl;
	//myfile.close();

	return 0;
}
