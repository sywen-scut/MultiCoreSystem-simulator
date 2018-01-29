#include "Region.h"

using namespace std;
Region::Region(Vertex p){
	start_p = p;
}

Region::Region(Vertex p, int w, int  l) {
	this->start_p = p;
	this->w = w;
	this->l = l;
	Vertex end_piont(p.w+region_width-1, p.l + region_length-1);
	this->end_p = end_piont;
	this->area = w * l;
}

