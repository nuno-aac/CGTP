#include "headers/model.h"

Model::Model() {
	numPontos = 0;
	colorR = 1;
	colorG = 1;
	colorB = 1;
}

Model::Model(float r, float g, float b) {
	numPontos = 0;
	colorR = r;
	colorG = g;
	colorB = b;
}

int Model::getNumPontos() {
	return numPontos;
}

float Model::getR() {
	return colorR;
}

float Model::getG() {
	return colorG;
}

float Model::getB() {
	return colorB;
}

vector<float> Model::getVertices() {
	return vertices;
}

float Model::getVertice(int i) {
	return vertices.at(i);
}

void Model::setNumPontos(int n) {
	numPontos = n;
}

void Model::setR(float r) {
	colorR = r;
}

void Model::setG(float g) {
	colorG = g;
}

void Model::setB(float b) {
	colorB = b;
}

void Model::pushVertice(float v) {
	numPontos++;
	vertices.push_back(v);
}