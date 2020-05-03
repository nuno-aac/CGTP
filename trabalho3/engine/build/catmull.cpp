//#include "headers/catmull.h"
#include "catmull.h"
using namespace std;

CatMull::CatMull() {
    points = vector<vector<float>>();
	time = 0;
}

CatMull::CatMull(vector<vector<float>> v, float t) {
	points = vector<vector<float>>(v);
	time = t;

}

vector<vector<float>> CatMull::getPoints() {
	vector<vector<float>> res (points);
	return res;
}

float CatMull::getTime() {
	return time;
}

void CatMull::setPoints(vector<vector<float>> p) {
	points = vector<vector<float>>(p);
}

void CatMull::setTime(float newTime) {
	time = newTime;
}

