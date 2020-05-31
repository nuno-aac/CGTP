#include "headers/xmlparse.h"
#include <IL/il.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;

void loadTexture(std::string s, Model * model) {

	unsigned int t, tw, th;
	unsigned char* texData;
	unsigned int texID;
	cout << s << '\n';
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)s.c_str());
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	cout << tw << "/" << th << '\n';
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();

	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	cout << "TEXID LOAD" << texID << '\n';

	model -> setTextureID(texID);

}

void parseMaterialFile(string file, Model* m) {
	string title;
	float val1, val2, val3;
	int valInt;

	Material * mat = new Material();

	ifstream infile(file);

	if (infile.is_open() != 1) {
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
	m->setMaterial(mat);
}

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

	infile.close();
}

void parseNormalFile(string file, Model* m) {
	float x, y, z;

	ifstream infile(file+"n");


	while (infile >> x >> y >> z) {
		m->pushNormal(x);
		m->pushNormal(y);
		m->pushNormal(z);
	}

	infile.close();
}

void parseTextureFile(string file, Model* m) {
	float x, y;

	ifstream infile(file + "t");


	while (infile >> x >> y) {
		m->pushTexture(x);
		m->pushTexture(y);
	}

	infile.close();
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
	Model* currentModel;
	string fileName, texName;

	if (g == nullptr) {
		return nullptr;
	}

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
		currentModel = parseFiles("..\\..\\generator\\3dfiles\\" + fileName);
		if (model->Attribute("material")){
			fileName = model->Attribute("material");
			parseMaterialFile("..\\materials\\" + fileName, currentModel);
		}
		if (model->Attribute("texture")) {
			texName = model->Attribute("texture");
			loadTexture("..\\texturas\\" + texName, currentModel);
			parseTextureFile("..\\..\\generator\\3dfiles\\" + fileName, currentModel);
		}
		group->pushModel(currentModel);
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

	if (l == nullptr) return nullptr;

	if (l->Attribute("type")) typeS = l->Attribute("type");
	if (typeS.compare("POINT") == 0) {
		light -> setType(L_POINT);
		if (l->Attribute("posX")) light->setPosX(atof(l->Attribute("posX")));
		if (l->Attribute("posY")) light->setPosY(atof(l->Attribute("posY")));
		if (l->Attribute("posZ")) light->setPosZ(atof(l->Attribute("posZ")));
		if (l->Attribute("att")) light->setAtt(atof(l->Attribute("att")));
	}
	if (typeS.compare("DIRECTIONAL") == 0) {
		light->setType(L_DIRECTIONAL);
		if (l->Attribute("dirX")) light->setDirX(atof(l->Attribute("dirX")));
		if (l->Attribute("dirY")) light->setDirY(atof(l->Attribute("dirY")));
		if (l->Attribute("dirZ")) light->setDirZ(atof(l->Attribute("dirZ")));
	}
	if (typeS.compare("SPOTLIGHT") == 0) {
		light->setType(L_SPOTLIGHT);
		if (l->Attribute("posX")) light->setPosX(atof(l->Attribute("posX")));
		if (l->Attribute("posY")) light->setPosY(atof(l->Attribute("posY")));
		if (l->Attribute("posZ")) light->setPosZ(atof(l->Attribute("posZ")));
		if (l->Attribute("dirX")) light->setDirX(atof(l->Attribute("dirX")));
		if (l->Attribute("dirY")) light->setDirY(atof(l->Attribute("dirY")));
		if (l->Attribute("dirZ")) light->setDirZ(atof(l->Attribute("dirZ")));
		if (l->Attribute("ang")) light->setAngCuttof(atof(l->Attribute("ang")));
		if (l->Attribute("exponent")) light->setExponent(atof(l->Attribute("exponent")));
		if (l->Attribute("att")) light->setAtt(atof(l->Attribute("att")));
	}
	return light;
}

void parseCamera(XMLElement * cam, Scene* scene) {
	float pX, pY, pZ, dX, dY, dZ;
	int camType;
	Group* camGroup;
	Light* camLight;
	camType = 0;
	
	pX = pY = pZ = dX = dY = dZ = 0;

	if (cam->Attribute("posX")) pX = atof(cam->Attribute("posX"));
	if (cam->Attribute("posY")) pY = atof(cam->Attribute("posY"));
	if (cam->Attribute("posZ")) pZ = atof(cam->Attribute("posZ"));
	if (cam->Attribute("dirX")) dX = atof(cam->Attribute("dirX"));
	if (cam->Attribute("dirY")) dY = atof(cam->Attribute("dirY"));
	if (cam->Attribute("dirZ")) dZ = atof(cam->Attribute("dirZ"));
	if (cam->Attribute("type")) camType = atof(cam->Attribute("type"));

	XMLElement* xmlgroup = cam->FirstChildElement("group");
	camGroup = parseGroup(xmlgroup);
	XMLElement* xmllight = cam->FirstChildElement("light");
	camLight = parseLight(xmllight);

	scene->setCamLight(camLight);
	scene->setCamPos(pX, pY, pZ);
	scene->setCamDir(dX, dY, dZ);
	scene->setCamGroup(camGroup);
	scene->setCamType(camType);

}


Scene* parseXML(string nome) {
	vector<Group*> groups;
	vector<Light*> lights;
	XMLDocument d;
	string str;
	Scene* scene = new Scene();

	string path = "..\\XML\\" + nome;

	if (!(d.LoadFile(path.c_str()))) {
		XMLElement* root = d.FirstChildElement("scene");

		if (root == nullptr) 
			return NULL;

		XMLElement* xmlcam = root->FirstChildElement("camera");
		parseCamera(xmlcam, scene);

		XMLElement* xmllights = root->FirstChildElement("lights");
		XMLElement* light = nullptr;
		if(xmllights) light = xmllights->FirstChildElement("light");

		while (light != nullptr) {

			lights.push_back(parseLight(light));
			light = light->NextSiblingElement();
		}
		scene->setLights(lights);

		XMLElement* grupo = root->FirstChildElement("group");

		while (grupo != nullptr) {

			groups.push_back(parseGroup(grupo));
			grupo = grupo->NextSiblingElement();
		}
		scene->setScene(groups);

		return scene;
	}
	else {
		std::cout << "Error: " << d.ErrorStr() << "\n";
	}
	return NULL;
}