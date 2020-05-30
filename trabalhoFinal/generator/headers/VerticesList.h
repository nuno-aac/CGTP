#ifndef VERTICESLIST_H__
#define VERTICESLIST_H__

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>

using namespace std;

class VerticesList {
	vector<float> points;
	vector<float> normals;
	vector<float> textures;

public:
	VerticesList();
	VerticesList(vector<float>);
	vector<float> getPoints();
	vector<float> getNormals();
	vector<float> getTextures();
	void normalize(float*);
	void setPoints(vector<float>);
  void addPoint(float, float, float);
	void addNormal(float, float, float);
	void addTextures(float, float);
	void plane(float);
  void box(float, float, float);
	void sphere(float, int, int);
	void sphereNormal(float,int,int);
	void cone(float, float, int, int);

};

#endif
