#include "headers/rotationAnimation.h"

RotationAnimation::RotationAnimation() {
	x = 0;
	y = 0;
	z = 0;
	time = 0;
}

RotationAnimation::RotationAnimation(float newX, float newY, float newZ, float newTime) {
	x = newX;
	y = newY;
	z = newZ;
	time = newTime;
}

float RotationAnimation::getX(){
	return x;
}

float RotationAnimation::getY(){
	return y;
}

float RotationAnimation::getZ(){
	return z;
}

float RotationAnimation::getTime(){
	return time;
}

void RotationAnimation::setX(float newX){
	x = newX;
}

void RotationAnimation::setY(float newY){
	y = newY;
}

void RotationAnimation::setZ(float newZ){
	z = newZ;
}

void RotationAnimation::setTime(float newTime){
 	time = newTime;
}
