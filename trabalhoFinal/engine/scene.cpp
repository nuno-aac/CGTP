#include "headers/scene.h"
#include "headers/light.h"

Scene::Scene() {
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

void Scene::setScene(vector<Group*> s) {
	scene = s;
}
void Scene::setScene(vector<Light*> l) {
	lights = l;
}