#include "headers/transformation.h"

Transformation::Transformation() {
	x = 0;
	y = 0;
	z = 0;
	angle = 0;
}

Transformation::Transformation(float newX, float newY, float newZ) {
	x = newX;
	y = newY;
	z = newZ;
	angle = 0;
}


Transformation::Transformation(float newX, float newY, float newZ, float newAngle) {
	x = newX;
	y = newY;
	z = newZ;
	angle = newAngle;
}

float Transformation::getX(){
	return x;
}

float Transformation::getY(){
	return y;
}

float Transformation::getZ(){
	return z;
}

float Transformation::getAngle(){
	return angle;
}

void Transformation::setX(float newX){
	x = newX;
}

void Transformation::setY(float newY){
	y = newY;
}

void Transformation::setZ(float newZ){
	z = newZ;
}


void Transformation::setAngle(float newAngle){
	angle = newAngle;
}