#ifndef __GROUP_H__
#define __GROUP_H__

#include <vector>
#include "transformation.h"
#include "model.h"
#include "catmull.h"

class Group {
	Catmull* translationAnimation;
	Transformation* rotation;
	Transformation* translation;
	Transformation* scale;
	vector<Model*> models;
	vector<Group*> childs;

public:
	Group();

	Transformation* getRotation();
	Transformation* getTranslation();
	Transformation* getScale();
	Catmull* getCatmull();
	vector<Model*> getModels();
	vector<Group*> getNextGroups();

	void setRotation(Transformation* r);
	void setTranslation(Transformation* t);
	void setScale(Transformation* s);
	void setCatmull(Catmull* c);
	void pushModel(Model* m);
	void pushGroup(Group* g);

};


#endif