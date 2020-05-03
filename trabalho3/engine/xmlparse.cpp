#include "headers/xmlparse.h"

using namespace std;

Model* parse3dFile(string file) {
	float x, y, z;
	float r, g, b;
	Model* m;

	ifstream infile(file);
	
	infile >> x;
	
	infile >> r >> g >> b;

	m = new Model(r, g, b);
	m->setNumPontos(x);

	while (infile >> x >> y >> z) {
		m->pushVertice(x);
		m->pushVertice(y);
		m->pushVertice(z);
	}

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

	cout << x << y << z << time << '\n';

	RotationAnimation* res = new RotationAnimation(x, y, z, time);

	return res;
}

Group* parseGroup(XMLElement* g) {
	Group* group = new Group();
	string fileName;

	XMLElement* translation = g ->FirstChildElement("translate");
	if (translation != nullptr) {
		if (translation->Attribute("time")) group->setCatmull(parseCatmull(translation));
		else group->setTranslation(parseTransformation(translation));
	}

	XMLElement* rotation = g -> FirstChildElement("rotate");
	if (rotation != nullptr) {
		if (rotation->Attribute("time")) group->setRotationAnimation(parseRotation(rotation));
		else group->setStaticRotation(parseTransformation(rotation));
	}

	XMLElement* scale = g -> FirstChildElement("scale");
	if (scale != nullptr) group -> setScale(parseTransformation(scale));

	XMLElement* models = g -> FirstChildElement("models");
	XMLElement* model = models -> FirstChildElement("model");
	while (model != nullptr) {
		fileName = model->Attribute("file");
		group -> pushModel(parse3dFile("..\\..\\generator\\3dfiles\\" + fileName));
		model = model->NextSiblingElement();
	}

	XMLElement* nextGroup = g -> FirstChildElement("group");
	while (nextGroup != nullptr) {;
		group -> pushGroup(parseGroup(nextGroup));
		nextGroup = nextGroup->NextSiblingElement();
	}

	return group;
}


vector<Group*> parseXML(string nome) {
	vector<Group*> scene;
	XMLDocument d;
	string str;

	string path = "..\\XML\\" + nome;

	if (!(d.LoadFile(path.c_str()))) {
		XMLElement* root = d.FirstChildElement("scene");

		if (root == nullptr) 
			return scene;

		XMLElement* elemento = root->FirstChildElement("group");

		while (elemento != nullptr) {

			scene.push_back(parseGroup(elemento));
			elemento = elemento->NextSiblingElement();
		}

		return scene;
	}
	else {
		std::cout << "Error: " << d.ErrorStr() << "\n";
	}
	return scene;
}