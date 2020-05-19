#include "headers/light.h"

Light::Light() {
	x = 0;
	y = 0;
	z = 0;
	type = L_POINT;
}

Light::Light(int t, float newX, float newY, float newZ) {
	x = newX;
	y = newY;
	z = newZ;
	type = t;
}


float Light::getX(){
	return x;
}

float Light::getY(){
	return y;
}

float Light::getZ(){
	return z;
}

float Light::getType(){
	return type;
}

void Light::setX(float newX){
	x = newX;
}

void Light::setY(float newY){
	y = newY;
}

void Light::setZ(float newZ){
	z = newZ;
}

void Light::setType(int newType){
	type = newType;
}