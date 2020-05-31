#ifndef __LIGHT_H__
#define __LIGHT_H__

const int L_POINT = 0;
const int L_DIRECTIONAL = 1;
const int L_SPOTLIGHT = 2;

class Light {
	int type;
	float posX, posY, posZ;
	float dirX, dirY, dirZ;
	float ang;
	float att;
	int exp;

public:
	Light();
	Light(int type, float newX, float newY, float newZ);
	Light(int type, float px, float py, float pz, float dx, float dy, float dz, float ang);

	float getPosX();
	float getPosY();
	float getPosZ();
	float getDirX();
	float getDirY();
	float getDirZ();
	float getAngCuttof();
	float getAtt();
	int getExponent();
	int getType();


	void setPosX(float px);
	void setPosY(float py);
	void setPosZ(float pz);
	void setDirX(float dx);
	void setDirY(float dy);
	void setDirZ(float dz);
	void setAngCuttof(float newAng);
	void setAtt(float a);
	void setExponent(int e);
	void setType(int newType);
};


#endif