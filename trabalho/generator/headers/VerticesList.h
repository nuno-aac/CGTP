#ifndef VERTICESLIST_H__
#define VERTICESLIST_H__

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>

using namespace std;

class VerticesList {
	vector<float> points;

public:
	VerticesList();
	VerticesList(vector<float>);
	vector<float> getPoints();
	void setPoints(vector<float>);
  void addPoint(float, float, float);
	void plane(float);
  void box(float, float, float);
	void sphere(float, int, int);
	void cone(float, float, int, int);

};

#endif
