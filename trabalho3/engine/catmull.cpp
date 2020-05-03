#include "headers/catmull.h"

using namespace std;

Catmull::Catmull() {
    points = vector<vector<float>>();
	time = 0;
}

Catmull::Catmull(vector<vector<float>> v, float t) {
	points = vector<vector<float>>(v);
	time = t;

}

vector<vector<float>> Catmull::getPoints() {
	vector<vector<float>> res (points);
	return res;
}

float Catmull::getTime() {
	return time;
}

void Catmull::setPoints(vector<vector<float>> p) {
	points = vector<vector<float>>(p);
}

void Catmull::addPoint(float x, float y, float z) {
	vector<float> point;
	point.push_back(x);
	point.push_back(y);
	point.push_back(z);
	points.push_back(point);
}

void Catmull::setTime(float newTime) {
	time = newTime;
}

