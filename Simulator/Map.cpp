#include"Map.h"
#include<string>
using namespace std;

vector<Core> initCores(double es,double is,double os) 
{
	vector<Core> cores;
	for (int l = 0; l < length; l++) {
		
			for (int w = 0; w < width; w++) {
				Core core(es,is,os);
				cores.push_back(core);
			}
		}
	
	return cores;
}

Map::Map(double es,double is,double os) {
	
	cores = initCores(es,is,os);
	availableCores = width*length - fraction;
}

int Map::getWidth() {
	return width;
}

int Map::getLength() {
	return length;
}

/*void Map::updateAvailableCores(int i) {
	availableCores += i;
	//showMap();
}*/

bool Map::turnOnCore(InvokedTask task, Vertex ver)
{
	if (!getSpecificCore(ver)->checkState()) {
		getSpecificCore(ver)->turnOn(task.getApplicationId(), task.getId());
		availableCores--;
		return true;
	}
	return false;
}

bool Map::turnOffCore(Vertex ver)
{
	if (getSpecificCore(ver)->checkState()) {
		getSpecificCore(ver)->turnOff();
		availableCores++;
		return true;
	}
	return false;
}

int Map::getAvailableCores() {
	return availableCores;
}

Core* Map::getSpecificCore(Vertex v) {
	int index = toIndex(v);
	return &(cores.at(index));
}

Core* Map::getSpecificCore(int index) {
	return &(cores.at(index));
}

int Map::toIndex(Vertex v) {
	return v.l * width + v.w ;
}
Vertex Map::toVertex(int i) {
	int l = i / width
	int w = i %  length
	return Vertex(l,w);
}

void Map::showMap() {
	cout << endl;
	for(int k = 0; k < length; k++) {
		cout << k << ":" << endl;
			for (int j = 0; j < width; j++) {
				Vertex temp(k, j);
				cout << cores.at(toIndex(temp)).getApplicationId() << " ";
			}
			cout << endl;
		
		cout << endl;
	}
	
	cout <<"availableCores:"<< availableCores << endl;
	cout << endl;
}

