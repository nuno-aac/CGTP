#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include "light.h"
#include "group.h"


class Scene {
	vector<Group*> scene;
	vector<Light*> lights;
	Group* camGroup;
	float camX, camY, camZ, camDirX, camDirY, camDirZ;

public:
	Scene();
	Scene(vector<Group*> s, vector<Light*> l);

	vector<Group*> getScene();
	vector<Light*> getLights();
	Group* getCamGroup();
	float getCamX();
	float getCamY();
	float getCamZ();
	float getCamDirX();
	float getCamDirY();
	float getCamDirZ();


	void setScene(vector<Group*> s);
	void setLights(vector<Light*> l);
	void setCamGroup(Group* g);
	void setCamPos(float x, float y, float z);
	void setCamDir(float x, float y, float z);
};


#endif