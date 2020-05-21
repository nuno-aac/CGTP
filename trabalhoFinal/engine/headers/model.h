#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

using namespace std;

class Model {
	int numPontos;
	float colorR, colorG, colorB;
	vector<float> vertices;
	vector<float> normals;

public:
	Model();
	Model(float r, float g, float b);

	int getNumPontos();
	float getR();
	float getG();
	float getB();
	vector<float> getVertices();
	vector<float> getNormals();
	float getVertice(int i);

	void setNumPontos(int n);
	void setR(float r);
	void setG(float g);
	void setB(float b);
	void pushVertice(float v);
	void pushNormal(float n);

};


#endif