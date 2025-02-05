#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

using namespace std;

class Model {
	int numPontos;
	float colorR, colorG, colorB;
	vector<float> vertices;

public:
	Model();
	Model(float r, float g, float b);

	int getNumPontos();
	float getR();
	float getG();
	float getB();
	vector<float> getVertices();
	float getVertice(int i);

	void setNumPontos(int n);
	void setR(float r);
	void setG(float g);
	void setB(float b);
	void pushVertice(float v);

};


#endif