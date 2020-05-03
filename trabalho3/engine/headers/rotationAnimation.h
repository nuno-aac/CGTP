#ifndef __ROTATIONANIMATION_H__
#define __ROTATIONANIMATION_H__

#define ROT_ANIMATION 1
#define ROT_STATIC 0


class RotationAnimation {
	float x, y, z, time;

public:
	RotationAnimation();
	RotationAnimation(float newX, float newY, float newZ, float newTime);

	float getX();
	float getY();
	float getZ();
	float getTime();

	void setX(float newX);
	void setY(float newY);
	void setZ(float newZ);
	void setTime(float newTime);

};


#endif
