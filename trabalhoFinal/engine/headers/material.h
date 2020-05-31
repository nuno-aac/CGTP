#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <vector>

using namespace std;

class Material {
	float diff[4];
	float amb[4];
	float spec[4];
	float emmissive[4];
	int shininess;

public:
	Material();

	float* getDiff();
	float getDiffR();
	float getDiffG();
	float getDiffB();
	float* getAmb();
	float getAmbR();
	float getAmbG();
	float getAmbB();
	float* getSpec();
	float getSpecR();
	float getSpecG();
	float getSpecB();
	float* getEmissive();
	float getEmissiveR();
	float getEmissiveG();
	float getEmissiveB();
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
	void setEmissiveR(float er);
	void setEmissiveG(float eg);
	void setEmissiveB(float eb);
	void setShininess(int s);


};


#endif