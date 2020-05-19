#ifndef __LIGHT_H__
#define __LIGHT_H__

const int L_POINT = 0;
const int L_DIRECTIONAL = 0;
const int L_SPOTLIGHT = 0;

class Light {
	int type;
	float x, y, z;

public:
	Light();
	Light(int type, float newX, float newY, float newZ);

	float getX();
	float getY();
	float getZ();
	float getType();

	void setX(float newX);
	void setY(float newY);
	void setZ(float newZ);
	void setType(int newType);
};


#endif