#include "headers/light.h"

Light::Light() {
	posX = posY = posZ = 0;
	dirX = dirY = dirZ = 0;
	ang = 0;
	att = 1;
	exp = 0;
	type = L_POINT;
}

Light::Light(int newType, float newX, float newY, float newZ) {
	posX = newX;
	posY = newY;
	posZ = newZ;
	dirX = newX;
	dirY = newY;
	dirZ = newZ;
	type = newType;
}


float Light::getPosX(){
	return posX;
}

float Light::getPosY(){
	return posY;
}

float Light::getPosZ(){
	return posZ;
}

float Light::getDirX() {
	return dirX;
}

float Light::getDirY() {
	return dirY;
}

float Light::getDirZ() {
	return dirZ;
}

float Light::getAngCuttof() {
	return ang;
}

float Light::getAtt() {
	return att;
}
int Light::getExponent() {
	return exp;
}

int Light::getType(){
	return type;
}

void Light::setPosX(float px){
	posX = px;
}

void Light::setPosY(float py){
	posY = py;
}

void Light::setPosZ(float pz){
	posZ = pz;
}

void Light::setDirX(float dx) {
	dirX = dx;
}

void Light::setDirY(float dy) {
	dirY = dy;
}

void Light::setDirZ(float dz) {
	dirZ = dz;
}

void Light::setAngCuttof(float newAng) {
	ang = newAng;
}

void Light::setAtt(float a) {
	att = a;
}

void Light::setExponent(int e) {
	exp = e;
}

void Light::setType(int newType){
	type = newType;
}