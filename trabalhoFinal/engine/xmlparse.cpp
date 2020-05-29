#include "headers/xmlparse.h"

using namespace std;

/*void parseMaterialFile(string file, Model* m) {
	string title;
	float val1, val2, val3;
	int valInt;

	Material * mat = new Material();

	ifstream infile(file);

	if (infile.is_open != 1) {
		cout << "Invalida Materials file" << file;
		return;
	}

	while (infile >> title) {
		if (strcmp(title.c_str(), "Diffuse") == 0) {
			infile >> val1 >> val2 >> val3;
			mat->setDiffR(val1);
			mat->setDiffG(val2);
			mat->setDiffB(val3);
		}
		if (strcmp(title.c_str(), "Ambient") == 0) {
			infile >> val1 >> val2 >> val3;
			mat->setAmbR(val1);
			mat->setAmbG(val2);
			mat->setAmbB(val3);
		}
		if (strcmp(title.c_str(), "Specular") == 0) {
			infile >> val1 >> val2 >> val3;
			mat->setSpecR(val1);
			mat->setSpecG(val2);
			mat->setSpecB(val3);
		}
		if (strcmp(title.c_str(), "Shininess") == 0) {
			infile >> valInt;
			mat->setShininess(valInt);
		}

	}
}*/

void parse3dFile(string file, Model * m) {
	float x, y, z;
	float r, g, b;

	ifstream infile(file);
	
	infile >> x;
	
	infile >> r >> g >> b;

	m ->setR(r);
	m->setG(g);
	m->setB(b);
	m->setNumPontos(x);

	while (infile >> x >> y >> z) {
		m->pushVertice(x);
		m->pushVertice(y);
		m->pushVertice(z);
	}
}

void parseNormalFile(string file, Model* m) {
	float x, y, z;

	ifstream infile(file+"n");


	while (infile >> x >> y >> z) {
		m->pushNormal(x);
		m->pushNormal(y);
		m->pushNormal(z);
	}
}
 
Model* parseFiles(string file) {
	Model* m = new Model();
	parse3dFile(file, m);
	parseNormalFile(file, m);
	return m;
}

Catmull* parseCatmull(XMLElement* t) {
	float x, y, z;
	Catmull* res = new Catmull();

	res->setTime(stof(t->Attribute("time")));

	XMLElement* point = t->FirstChildElement("point");
	while (point != nullptr) {
		x = stof(point->Attribute("X"));
		y = stof(point->Attribute("Y"));
		z = stof(point->Attribute("Z"));

		res->addPoint(x, y, z);

		point = point->NextSiblingElement();
	}
	
	return res;
}

Transformation* parseTransformation(XMLElement* t) {
	float x = 0, y = 0, z = 0, angle = 0;

	if (t->Attribute("angle")) angle = stof(t->Attribute("angle"));

	if (t->Attribute("X")) x = stof(t->Attribute("X"));

	if (t->Attribute("Y")) y = stof(t->Attribute("Y"));	

	if (t->Attribute("Z")) z = stof(t->Attribute("Z"));

	Transformation* trans = new Transformation(x, y, z, angle);

	return trans;
}

RotationAnimation* parseRotation(XMLElement* t) {
	float x = 0, y = 0, z = 0, time = 0;

	if (t->Attribute("time")) time = stof(t->Attribute("time"));

	if (t->Attribute("X")) x = stof(t->Attribute("X"));

	if (t->Attribute("Y")) y = stof(t->Attribute("Y"));

	if (t->Attribute("Z")) z = stof(t->Attribute("Z"));

	RotationAnimation* res = new RotationAnimation(x, y, z, time);

	return res;
}

Group* parseGroup(XMLElement* g) {
	Group* group = new Group();
	string fileName;

	XMLElement* translation = g->FirstChildElement("translate");
	if (translation != nullptr) {
		if (translation->Attribute("time")) group->setCatmull(parseCatmull(translation));
		else group->setTranslation(parseTransformation(translation));
	}

	XMLElement* rotation = g->FirstChildElement("rotate");
	if (rotation != nullptr) {
		if (rotation->Attribute("time")) group->setRotationAnimation(parseRotation(rotation));
		else group->setStaticRotation(parseTransformation(rotation));
	}

	XMLElement* scale = g->FirstChildElement("scale");
	if (scale != nullptr) group->setScale(parseTransformation(scale));

	XMLElement* models = g->FirstChildElement("models");
	XMLElement* model = models->FirstChildElement("model");
	while (model != nullptr) {
		fileName = model->Attribute("file");
		group->pushModel(parseFiles("..\\..\\generator\\3dfiles\\" + fileName));
		model = model->NextSiblingElement();
	}

	XMLElement* nextGroup = g->FirstChildElement("group");
	while (nextGroup != nullptr) {
		;
		group->pushGroup(parseGroup(nextGroup));
		nextGroup = nextGroup->NextSiblingElement();
	}

	return group;
}

Light* parseLight(XMLElement* l) {
	string typeS;
	float x, y, z;
	int type;
	type = L_POINT;
	string fileName;
	Light * light = new Light();

	if (l->Attribute("type")) typeS = l->Attribute("type");
	if (typeS.compare("POINT") == 0) {
		type = L_POINT;
	}
	if (typeS.compare("DIRECTIONAL") == 0) {
		type = L_DIRECTIONAL;
	}
	if (typeS.compare("SPOTLIGHT") == 0) {
		type = L_SPOTLIGHT;
	}

	if (l->Attribute("X")) x = atof(l->Attribute("X"));
	if (l->Attribute("Y")) y = atof(l->Attribute("Y"));
	if (l->Attribute("Z")) z = atof(l->Attribute("Z"));
}


Scene* parseXML(string nome) {
	vector<Group*> groups;
	vector<Light*> lights;
	XMLDocument d;
	string str;

	string path = "..\\XML\\" + nome;

	if (!(d.LoadFile(path.c_str()))) {
		XMLElement* root = d.FirstChildElement("scene");

		if (root == nullptr) 
			return NULL;

		XMLElement* xmllights = root->FirstChildElement("lights");
		XMLElement* light = nullptr;
		if(xmllights) light = xmllights->FirstChildElement("light");

		while (light != nullptr) {

			lights.push_back(parseLight(light));
			light = light->NextSiblingElement();
		}

		XMLElement* grupo = root->FirstChildElement("group");

		while (grupo != nullptr) {

			groups.push_back(parseGroup(grupo));
			grupo = grupo->NextSiblingElement();
		}

		return new Scene(groups, lights);
	}
	else {
		std::cout << "Error: " << d.ErrorStr() << "\n";
	}
	return NULL;
}