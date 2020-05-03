#ifndef __CATMULL_H__
#define __CATMULL_H__

#include <vector>
using namespace std;

class CatMull {
	vector<vector<float>> points;
	float time;

public:
	CatMull();
	CatMull(vector<vector<float>> points, float newAngle);

	vector<vector<float>> getPoints();
	float getTime();

	void setPoints(vector<vector<float>> p);
	void setTime(float newTime);

};


#endif
