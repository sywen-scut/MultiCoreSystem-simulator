#include"Shape.h"
#include <math.h>
Shape::Shape(int nT) {
	numTasks = nT;
	length = 1;//for 2D shapes;
}

void Shape::rectangle() {
	height = sqrt((double)numTasks);
	while (height >= 1) {
		width = numTasks / height;
		if (height * width == numTasks) {
			break;
		}
		else {
			height--;
		}
	}
}

void Shape::fittingMap() {
	width = sqrt((double)numTasks);
	height = numTasks / width;
	while (width*height < numTasks) {
		width++;
	}
}