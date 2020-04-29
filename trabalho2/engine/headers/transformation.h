#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

class Transformation {
	float x, y, z, angle;

public:
	Transformation();
	Transformation(float newX, float newY, float newZ);
	Transformation(float newX, float newY, float newZ, float newAngle);

	float getX();
	float getY();
	float getZ();
	float getAngle();

	void setX(float newX);
	void setY(float newY);
	void setZ(float newZ);
	void setAngle(float newAngle);

};


#endif