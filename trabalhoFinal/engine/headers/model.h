#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "material.h"

using namespace std;

class Model {
	int numPontos;
	float colorR, colorG, colorB;
	int textureID;
	Material * material;
	vector<float> vertices;
	vector<float> normals;
	vector<float> textures;

public:
	Model();
	Model(float r, float g, float b);

	int getNumPontos();
	float getR();
	float getG();
	float getB();
	vector<float> getVertices();
	vector<float> getNormals();
	vector<float> getTextures();
	Material * getMaterial();
	float getVertice(int i);
	int getTextureID();

	void setNumPontos(int n);
	void setR(float r);
	void setG(float g);
	void setB(float b);
	void setMaterial(Material * m);
	void setTextureID(int id);
	void pushVertice(float v);
	void pushNormal(float n);
	void pushTexture(float t);

};


#endif