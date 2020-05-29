#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <vector>

using namespace std;

class Material {
	float diffR, diffG, diffB;
	float ambR, ambG, ambB;
	float specR, specG, specB;
	int shininess;

public:
	Material();

	float getDiffR();
	float getDiffG();
	float getDiffB();
	float getAmbR();
	float getAmbG();
	float getAmbB();
	float getSpecR();
	float getSpecG();
	float getSpecB();
	int getShininess();

	void setDiffR(float dr);
	void setDiffG(float dg);
	void setDiffB(float db);
	void setAmbR(float ar);
	void setAmbG(float ag);
	void setAmbB(float ab);
	void setSpecR(float sr);
	void setSpecG(float sg);
	void setSpecB(float sb);
	void setShininess(int s);


};


#endif