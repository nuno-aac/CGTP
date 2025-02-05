#include "headers/group.h"

Group::Group() {
	rotation = NULL;
	translation = NULL;
	scale = NULL;
	rotationAnimation = NULL;
	translationAnimation = NULL;
}

Transformation* Group::getStaticRotation(){
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

RotationAnimation* Group::getRotationAnimation() {
	return rotationAnimation;
}

vector<Model*> Group::getModels(){
	return models;
}

vector<Group*> Group::getNextGroups(){
	return childs;
}

void Group::setStaticRotation(Transformation* r){
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

void Group::setRotationAnimation(RotationAnimation* r){
	rotationAnimation = r;
}

void Group::pushModel(Model* m) {
	models.push_back(m);
}

void Group::pushGroup(Group* g){
	childs.push_back(g);
}
