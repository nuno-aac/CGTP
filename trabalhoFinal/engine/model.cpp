#include "headers/model.h"

Model::Model() {
	numPontos = 0;
	colorR = 1;
	colorG = 1;
	colorB = 1;
	material = nullptr;
}

Model::Model(float r, float g, float b) {
	numPontos = 0;
	colorR = r;
	colorG = g;
	colorB = b;
	material = nullptr;
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

Material * Model::getMaterial() {
	return material;
}

vector<float> Model::getVertices() {
	return vertices;
}

vector<float> Model::getNormals() {
	return normals;
}

vector<float> Model::getTextures() {
	return textures;
}

int Model::getTextureID() {
	return textureID;
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

void Model::setMaterial(Material * m) {
	material = m;
}

void Model::setTextureID(int id) {
	textureID = id;
}

void Model::pushVertice(float v) {
	numPontos++;
	vertices.push_back(v);
}

void Model::pushNormal(float n) {
	normals.push_back(n);
}

void Model::pushTexture(float n) {
	textures.push_back(n);
}