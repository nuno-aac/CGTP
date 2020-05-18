#ifndef __CATMULL_H__
#define __CATMULL_H__

#include <vector>
using namespace std;

class Catmull {
	vector<vector<float>> points;
	float time;

public:
	Catmull();
	Catmull(vector<vector<float>> points, float newAngle);

	vector<vector<float>> getPoints();
	float getTime();

	void setPoints(vector<vector<float>> p);
	void setTime(float newTime);
	void addPoint(float x, float y, float z);

};


#endif
