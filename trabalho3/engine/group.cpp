#include "headers/group.h"

Group::Group() {
	rotation = NULL;
	translation = NULL;
	scale = NULL;
}

Transformation* Group::getRotation(){
	return rotation;
}

Transformation* Group::getTranslation(){
	return translation;
}

Transformation* Group::getScale(){
	return scale;
}

Catmull* Group::getCatmull() {
	return translationAnimation;
}

vector<Model*> Group::getModels(){
	return models;
}

vector<Group*> Group::getNextGroups(){
	return childs;
}

void Group::setRotation(Transformation* r){
	rotation = r;
}
void Group::setTranslation(Transformation* t){
	translation = t;
}

void Group::setScale(Transformation* s){
	scale = s;
}

void Group::setCatmull(Catmull* c) {
	translationAnimation = c;
}

void Group::pushModel(Model* m) {
	models.push_back(m);
}

void Group::pushGroup(Group* g){
	childs.push_back(g);
}
