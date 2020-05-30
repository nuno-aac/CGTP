#include "headers/scene.h"
#include "headers/light.h"

Scene::Scene() {
	camX = 0;
	camY = 0;
	camZ = 0;
	camDirX = 0;
	camDirY = 0;
	camDirZ = 0;
	camGroup = nullptr;
}

Scene::Scene(vector<Group*> s, vector<Light*> l) {
	lights = l;
	scene = s;
}
vector<Group*> Scene::getScene() {
	return scene;
}

vector<Light*> Scene::getLights() {
	return lights;
}

Group* Scene::getCamGroup() {
	return camGroup;
}

float Scene::getCamX() {
	return camX;
}

float Scene::getCamY() {
	return camY;
}

float Scene::getCamZ() {
	return camZ;
}

float Scene::getCamDirX() {
	return camDirX;
}

float Scene::getCamDirY() {
	return camDirY;
}

float Scene::getCamDirZ() {
	return camDirZ;
}

void Scene::setScene(vector<Group*> s) {
	scene = s;
}
void Scene::setLights(vector<Light*> l) {
	lights = l;
}

void Scene::setCamGroup(Group* g) {
	camGroup = g;
}

void Scene::setCamPos(float x, float y, float z) {
	camX = x;
	camY = y;
	camZ = z;
}

void Scene::setCamDir(float x, float y, float z) {
	camDirX = x;
	camDirY = y;
	camDirZ = z;
}