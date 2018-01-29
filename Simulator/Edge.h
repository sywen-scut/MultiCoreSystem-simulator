#pragma once
#include "Vertex.h"
class Edge {
private:
	int speed;
	Vertex source;
	Vertex destination;
public:
	Edge(Vertex s, Vertex d,int sp = 0) {
		source = s;
		destination = d;
		speed = sp;
	}
	int getSpeed();
};