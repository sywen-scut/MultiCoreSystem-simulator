#pragma once
class Shape {
public:
	int width;
	int height;
	int length;
	int numTasks;
	Shape(int numTasks = 0);
	void rectangle();
	void fittingMap();
};