#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include "light.h"
#include "group.h"


class Scene {
	vector<Group*> scene;
	vector<Light*> lights;

public:
	Scene();
	Scene(vector<Group*> s, vector<Light*> l);

	vector<Group*> getScene();
	vector<Light*> getLights();


	void setScene(vector<Group*> s);
	void setScene(vector<Light*> l);

};


#endif