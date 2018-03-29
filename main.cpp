#include <iostream>
#include <string>
#include <queue>
#include <stdlib.h>  
#include <algorithm>
#include <cmath>
#include <unistd.h>
#include <fstream>
#include "Event.h"


static double threshold = 80;

using namespace std;

queue<Event> readEventFromFile(int numApplication, string filenamePrefix) {
	queue<Event> results;
	for (int i = 0; i < numApplication; i++) {
		Application app(filenamePrefix + to_string(i) + ".txt");
		Event e(app);
		results.push(e);
	}

	return results;
}
long cycle = 0;

/*check if temperture exceed threshold*/
bool CheckTemperture(Map* map){
	for(int k = 0; k < map->getLength(); k++) {
		for (int j = 0; j < map-> getWidth(); j++) {
			Vertex temp(k, j);
			if (map->getSpecificCore(temp)->getTaskId() != -1)
			{
				if(map->getSpecificCore(temp)->temperture > threshold)
					return true;
				}
			}
		}
		return false;
}

/*check which application and which tasks exceed threshold*/
vector<vector<int>> CheckHotApps(Map* map){
	vector<vector<int>> hot_aid_tid;
	vector<int> hot_aid;
	vector<int> hot_tid;
	Core* core_temp;
	for(int k = 0; k < map->getLength(); k++) {
		for (int j = 0; j < map-> getWidth(); j++) {
				Vertex temp(k, j);
				core_temp = map->getSpecificCore(temp);
				if (core_temp->active)
				{
					if(core_temp->temperture > threshold){
						hot_aid.push_back(core_temp->getApplicationId());
						hot_tid.push_back(core_temp->getTaskId());
					}

				}
		}
	}
	hot_aid_tid.push_back(hot_aid);
	hot_aid_tid.push_back(hot_tid);	
	return hot_aid_tid;
}

int main(int argc,char* argv[]) {
	
	
	int numApplication = 7;
	queue<Event> events;
	
	/*string appName = argv[2];
	string app_filepath = "/home/chris/simulator/app/ccr/" + appName+".txt";
	Event test1(app_filepath);
	events.push(test1);*/

	Application testApp1("/home/chris/simulator/app/arrivalrate/a1000/t1.txt");
	Event t1(testApp1);
	events.push(t1);

	Application testApp2("/home/chris/simulator/app/arrivalrate/a1000/t2.txt");
	Event t2(testApp2);
	events.push(t2);

	Application testApp3("/home/chris/simulator/app/arrivalrate/a1000/t3.txt");
	Event t3(testApp3);
	events.push(t3);

	Application testApp4("/home/chris/simulator/app/arrivalrate/a1000/t4.txt");
	Event t4(testApp4);
	events.push(t4);
	
	Application testApp5("/home/chris/simulator/app/arrivalrate/a1000/t5.txt");
	Event t5(testApp5);
	events.push(t5);

	Application testApp6("/home/chris/simulator/app/arrivalrate/a1000/t6.txt");
	Event t6(testApp6);
	events.push(t6);
	
	Application testApp7("/home/chris/simulator/app/arrivalrate/a1000/t7.txt");
	Event t7(testApp7);
	events.push(t7);
	

	double commCost = 0;

	Clock* clock = Clock::getClock();
	/*Running frequency 1, mesh size 8*8*/
	Map map(1, 8, 8);
	
	
	
/*	int numApplication = 12;
	string applicationString = "..\\app\\self\\tn\\16\\";
	queue<Event> events = readEventFromFile(numApplication, applicationString);
	*/
	string type_str = argv[1];
	string Ptrace_filepath = "/home/chris/simulator/Ptrace/mixture"+type_str+".ptrace";
	FILE* fp = fopen(Ptrace_filepath.c_str(),"w");
	if( fp==NULL )
		{cout<<"can't find output path"<<endl;
		exit(0);
	}
	else
		cout << "Output: " <<Ptrace_filepath << endl;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			fprintf(fp, "P_%d_%d \t", j, i);
		}

	}
	fprintf(fp, "\n");
	fclose(fp);
	
	string cmd_string = "/home/chris/HotSpot-6.0/simulator.sh mixture"+type_str;
	char hotspot_cmd[100];
	sprintf(hotspot_cmd,"%s", cmd_string.c_str());
	string ttrace_path = "/home/chris/HotSpot-6.0/mixture"+type_str+".ttrace";
	
	cout<<"start running"<<endl;
	vector<Event> waiting;
	vector<Event> running;
	vector<Event> finished;
	/*1 square 2 chess 3 nonContigous */
	int type = atoi(argv[1]);

	Vertex cur_p(0,0);
	while (finished.size() != numApplication) {
		
		for (vector<Event>::iterator iter = waiting.begin(); (!waiting.empty())&&iter != waiting.end();) 
		{
			if ((*iter).startRunning(&map, type, cur_p)) {
				cout<<"time : "<< clock->now() <<endl;
				map.showMap();
				switch (type){
				case 1:
					if ((*iter).region.end_p.w<3)
						cur_p.w = 4;
					else
						cur_p.w = (*iter).region.end_p.w + 1;
					break;
				case 2:
					if ((*iter).region.end_p.l<3)
						cur_p.l = 4;
					else
						cur_p.l = (*iter).region.end_p.l + 1;
					break;
				case 3:
					break;
				case 4:
					if ((*iter).region.end_p.l<= 4){
						if ((*iter).region.end_p.w <= 4)
							cur_p.w = (*iter).region.end_p.w + 1;
						else
						{
							cur_p.w = 0;
							cur_p.l = (*iter).region.end_p.l + 1;
						}
					}
					else {
						if ((*iter).region.end_p.w <= 4){							
							cur_p.w = (*iter).region.end_p.w + 1;
							cur_p.l = (*iter).region.start_p.l;
						}
						else
							cur_p.l = (*iter).region.end_p.l + 1;
					}
					break;
				}

				
				running.push_back(*iter);
				iter = waiting.erase(iter);
			}
			else{
				iter++;
			}
		}

		while (!events.empty()) {
			Event app = events.front();
			if (app.arrive(clock->now())) {
				if (app.startRunning(&map, type, cur_p)) {
					cout<<"time : "<< clock->now() <<endl;
					map.showMap();
					switch (type){
					case 1:
						if (app.region.end_p.w<3)
							cur_p.w = 4;
						else
							cur_p.w = app.region.end_p.w + 1;
						break;
					case 2:
						if (app.region.end_p.l<3)
							cur_p.l = 4;
						else
							cur_p.l = app.region.end_p.l + 1;
						break;
					case 3:
						break;
					case 4:
						if (app.region.end_p.l<= 4){
							if (app.region.end_p.w <= 4)
								cur_p.w = app.region.end_p.w + 1;
							else
							{
								cur_p.w = 0;
								cur_p.l = app.region.end_p.l + 1;
							}
						}
						else {
							if (app.region.end_p.w <= 4){
								cur_p.w = app.region.end_p.w + 1;
								cur_p.l = app.region.start_p.l;
							}
							else
								cur_p.l = app.region.end_p.l + 1;
						}
						break;

						
					}
					running.push_back(app);
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
				cur_p = (*iter).region.start_p;
				finished.push_back(*iter);
				iter = running.erase(iter);
			}
			else {
				iter++;
			}
		}
		
		/*write power trace*/
		fp = fopen(Ptrace_filepath.c_str(),"a+");
		map.writePtrace(fp);
		fclose(fp);

		/*check temperature and migrate tasks here*/
		if (clock->now() % 500 == 0 && clock->now() !=0)
		{
			system(hotspot_cmd);
			map.getTemperature(ttrace_path);
			
			if(CheckTemperture(&map)){
				cout<<"time : "<< clock->now() <<endl;
				// tasks = squareMigration(map_t, tasks);
				vector<vector<int>> hot_aid_tid = CheckHotApps(&map);
				vector<int> hot_aid = hot_aid_tid[0];
				sort(hot_aid.begin(), hot_aid.end());
				vector<int>::iterator aiter = unique(hot_aid.begin(), hot_aid.end());
				hot_aid.erase(aiter, hot_aid.end());
				for(auto aid:hot_aid_tid[0]){
					cout<<aid<<'\t';
				}
				cout<<endl;
				for(auto aid:hot_aid){
					cout<<aid<<'\t';
				}
				cout<<endl;
				for (auto aid:hot_aid)
				{
					for (vector<Event>::iterator iter = running.begin(); iter != running.end();){
						if ((*iter).getId() == aid)
						{
							switch(type){
								case 1:
									if((*iter).squareMigration(&map))
										cout<<"migrate succcessfully"<<endl;
									else
										cout<<"migrate failed"<<endl;
									break;
								case 2:
									if((*iter).chessMigration(&map))
										cout<<"migrate succcessfully"<<endl;
									else
										cout<<"migrate failed"<<endl;
									break;
								case 3:{
									vector<int> cur_hot_tid;
									int index = 0;
									for (auto tid:hot_aid_tid[1]){
										if (hot_aid_tid[0].at(index) == (*iter).getId())
										{
											cur_hot_tid.push_back(tid);
										}
										index++;
									}
									if((*iter).globalColdest(&map, cur_hot_tid))
										cout<<"migrate succcessfully"<<endl;
									else
										cout<<"migrate failed"<<endl;
								}
									break;

								case 4:{
									vector<int> another_cur_hot_tid;
									int index2 = 0;
									for (auto tid:hot_aid_tid[1]){
										if (hot_aid_tid[0].at(index2) == (*iter).getId())
										{
											another_cur_hot_tid.push_back(tid);
										}
										index2++;
									}
									if((*iter).localColdest(&map, another_cur_hot_tid))
										cout<<"migrate succcessfully"<<endl;
									else
										cout<<"migrate failed"<<endl;
								}
									break;
							}
						}
						iter++;
					}
				}	
				map.showMap();
			}
		}
		
		clock->tick();
		cycle++;
	}

	cout << "total running time: " << clock->now() << endl;
	
	map.showMap();
	return 0;
}
