#include <stdlib.h>
#include <string>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "headers/xmlparse.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


#define _USE_MATH_DEFINES

using namespace std;

Scene* fullScene;
vector<Group*> scene;
vector<Light*> lights;
vector<vector<float>> vertices;

GLuint modelsBuf[300];
GLuint normalsBuf[300];
GLuint texturesBuf[300];
int currentModel = 0;

float time = 0.0f;

float camX, camY, camZ;
float vcamX, vcamY, vcamZ;
float angHor, angVert;

bool showOrbit, isPause;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

float getRotationAngle(RotationAnimation * r) {
	float aux, percentage;
	aux = time / r->getTime();
	percentage = aux - floor(aux);
	return (percentage*360);
}

void multMatrixVector(float* m, float* v, float* res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}
}

void getCatmullRomPoint(float t, float* p0, float* p1, float* p2, float* p3, float* pos, float* deriv) {

	// catmull-rom matrix
	float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f} };
	float a[4];
	float te[4] = { t * t * t , t * t , t , 1 };
	float telinha[4] = { 3 * t * t , 2 * t , 1 , 0 };
	for (int i = 0; i < 3; i++) {
		float p[4] = { p0[i], p1[i],p2[i],p3[i] };
		multMatrixVector(*m, p, a);

		pos[i] = te[0] * a[0] + te[1] * a[1] + te[2] * a[2] + te[3] * a[3];
		deriv[i] = telinha[0] * a[0] + telinha[1] * a[1] + telinha[2] * a[2] + telinha[3] * a[3];
	}

}


// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, Catmull* catmull, float* pos, float* deriv) {
	int pointCount;
	float catmullTime;
	vector<vector<float>> points;

	points = catmull->getPoints();
	pointCount = points.size();
	catmullTime = catmull->getTime();

	float t = (gt/catmullTime) * pointCount; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment
	// indices store the points

	int indices[4];
	indices[0] = (index + pointCount - 1) % pointCount;
	indices[1] = (indices[0] + 1) % pointCount;
	indices[2] = (indices[1] + 1) % pointCount;
	indices[3] = (indices[2] + 1) % pointCount;

	getCatmullRomPoint(t, &(points[indices[0]])[0], &(points[indices[1]])[0], &(points[indices[2]])[0], &(points[indices[3]])[0], pos, deriv);
}

void defaultMaterials() {
	float amb[4] = { 0.2, 0.2, 0.2, 1.0 };
	float diff[4] = { 0.8, 0.8, 0.8, 1.0 };
	float spec[4] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
}

void drawCurve(Catmull* c) {
	float amb[4] = { 0.4, 0.4, 0.4, 1.0 };
	float diff[4] = { 0, 0, 0, 1.0 };
	float spec[4] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	
	float pos[3];
	float deriv[3];
	vector<float> curveVertex;
	for (float tcurve = 0; tcurve < c->getTime(); tcurve += 0.01) {
		getGlobalCatmullRomPoint(tcurve, c, pos, deriv);
		curveVertex.push_back(pos[0]);
		curveVertex.push_back(pos[1]);
		curveVertex.push_back(pos[2]);
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, modelsBuf[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * curveVertex.size(), &curveVertex[0], GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_LINE_LOOP, 0, curveVertex.size() / 3);
	defaultMaterials();
}

void applyTransformations(Group* g) {
	Transformation* t;
	Catmull* c;
	RotationAnimation* rotA;
	float pos[3];
	float deriv[3];

	c = g->getCatmull();
	if (c != NULL) {
		if (showOrbit) {
			drawCurve(c);
		}
		getGlobalCatmullRomPoint(time,c, pos, deriv);
		glTranslatef(pos[0], pos[1], pos[2]);
		currentModel++;
	}

	t = g -> getTranslation();
	if (t != NULL) {
		glTranslatef(t->getX(), t -> getY(), t -> getZ());
	}

	rotA = g->getRotationAnimation();
	if (rotA != NULL) {
		glRotatef(getRotationAngle(rotA), rotA->getX(), rotA->getY(), rotA->getZ());
	}

	t = g -> getStaticRotation();
	if(t != NULL)
		glRotatef(t -> getAngle(), t->getX(), t->getY(), t->getZ());

	t = g -> getScale();
	if (t != NULL)
		glScalef(t->getX(), t->getY(), t->getZ());
}

void drawModel(Model* model) {
	Material * material = model->getMaterial();
	vector<float> modelVertex = model->getVertices();
	vector<float> normals = model->getNormals();
	vector<float> textures = model->getTextures();
	glColor3f(model -> getR(), model -> getG(), model -> getB());
	if (material) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->getAmb());
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->getDiff());
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->getSpec());
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material->getEmissive());
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->getShininess());
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, modelsBuf[currentModel]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model->getVertices().size(), &modelVertex[0], GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuf[currentModel]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
	glNormalPointer(GL_FLOAT, 0, 0);
	if (textures.size() != 0) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, model->getTextureID());
		glBindBuffer(GL_ARRAY_BUFFER, texturesBuf[currentModel]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textures.size(), &textures[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, texturesBuf[currentModel]);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
	}

	glDrawArrays(GL_TRIANGLES, 0, model->getVertices().size()/3);
	glBindTexture(GL_TEXTURE_2D, 0);
	defaultMaterials();
}

void drawModels(vector<Model*> models){

	for (int i = 0; i < models.size(); i++) {
		drawModel(models[i]);
		currentModel++;
	}
}

void drawGroup(Group* g){
	glPushMatrix();

	applyTransformations(g);

	drawModels(g->getModels());

	vector<Group*> nextGroups = g->getNextGroups();
	for (int i = 0; i < nextGroups.size(); i++) {
		drawGroup(nextGroups[i]);
	}

	glPopMatrix();
}

void setupLight(Light* l, int i) {
	float pos[4], dir[4];
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0 + i);
	if (l->getType() == L_POINT) {
		pos[0] = l->getPosX(); pos[1] = l->getPosY(); pos[2] = l->getPosZ(); pos[3] = 1;
		glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, l->getAtt());
		glLightfv(GL_LIGHT0 + i, GL_POSITION, pos);
	}
	if (l->getType() == L_DIRECTIONAL) {
		dir[0] = l->getDirX(); dir[1] = l->getDirY(); dir[2] = l->getDirZ(); dir[3] = 0;
		glLightfv(GL_LIGHT0 + i, GL_POSITION, dir);
	}
	if (l->getType() == L_SPOTLIGHT) {
		pos[0] = l->getPosX(); pos[1] = l->getPosY(); pos[2] = l->getPosZ(); pos[3] = 1;
		dir[0] = l->getDirX(); dir[1] = l->getDirY(); dir[2] = l->getDirZ(); dir[3] = 1;
		glLightfv(GL_LIGHT0 + i, GL_POSITION, pos);
		glLightfv(GL_LIGHT0 + i, GL_SPOT_DIRECTION, dir);
		glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, l->getAngCuttof());
		glLightf(GL_LIGHT0 + i, GL_SPOT_EXPONENT, l->getExponent());
		glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, l->getAtt());
	}
	GLfloat qaAmbientLight[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat qaDiffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0 + i, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0 + i, GL_SPECULAR, qaSpecularLight);
}

void setupLights(vector<Light*> l, int isCamLight) {
	for (int i = isCamLight; i < l.size() + isCamLight; i++) {
		setupLight(l[i-isCamLight], i);
	}
}

void polar2Cartesian(float r, float angH, float angV, float* x, float* y, float* z) {
	*x = r * sin(angV) * cos(angH);
	*y = r * cos(angV);
	*z = r * sin(angV) * sin(angH);
}

void renderScene(void) {
	int size, numFigura, isCamLight;
	isCamLight = 0;
	float lookX, lookY, lookZ;
	// clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if (fullScene->getCamGroup() != nullptr) {
		drawGroup(fullScene->getCamGroup());
	}
	if (fullScene->getCamLight() != nullptr) {
		setupLight(fullScene->getCamLight(), 0);
		isCamLight = 1;
	}
	// set the camera
	polar2Cartesian(1, angHor, angVert, &lookX, &lookY, &lookZ);
	gluLookAt(camX, camY, camZ,
		camX + lookX, camY + lookY, camZ + lookZ,
		0.0f, 1.0f, 0.0f);

	setupLights(lights, isCamLight);

	for (int i = 0; i < scene.size(); i++) {
		drawGroup(scene[i]);
	}
	currentModel = 0;
	// End of frame
	glutSwapBuffers();
	if (!isPause) {
		time = glutGet(GLUT_ELAPSED_TIME);
		time /= 1000;
	}
}

void setupCam() {
	camX = fullScene ->getCamX();
	camY = fullScene->getCamY();
	camZ = fullScene->getCamZ();
	vcamX = -fullScene->getCamDirX();
	vcamY = fullScene->getCamDirY(),
	vcamZ = -fullScene->getCamDirZ();


	angHor = -3.14+atan2(vcamZ,vcamX);
	angVert = atan2(sqrt(vcamX*vcamX+vcamZ*vcamZ),vcamY);
}

void keyUp(int keyCode, int x, int y) {
	if (fullScene->getCamType() == 1) {
		switch (keyCode) {
		case GLUT_KEY_UP:
			angVert -= 0.02f;
			break;
		case GLUT_KEY_DOWN:
			angVert += 0.02f;
			break;
		case GLUT_KEY_RIGHT:
			angHor += 0.02f;
			break;
		case GLUT_KEY_LEFT:
			angHor -= 0.02f;
			break;
		default:
			break;
		}
	}
	glutPostRedisplay();
}


void processKeys(unsigned char key, int xx, int yy) {
	float newX, newZ, newY;
	switch (key) {

	case 'w':
		polar2Cartesian(5, angHor, angVert, &newX, &newY, &newZ);
		camX += newX;
		camZ += newZ;
		camY += newY;
		break;

	case 's':
		polar2Cartesian(5, angHor, angVert, &newX, &newY, &newZ);
		camX -= newX;
		camZ -= newZ;
		camY -= newY;
		break;
	case 'q':
		showOrbit = !showOrbit;
		break;
	case 'p':
		isPause = !isPause;
		break;
	default:
		break;
	}
}

using namespace std;

int main(int argc, char** argv) {
	// put GLUT init here
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("Trabalho fase 1");

	glewInit();
	glGenBuffers(300, modelsBuf);
	glGenBuffers(300, texturesBuf);
	glGenBuffers(300, normalsBuf);
	// put callback registration here
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	//key handling
	glutSpecialFunc(keyUp);
	glutKeyboardFunc(processKeys);

	// OpenGL settings
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //WIRES
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//Get models
	fullScene = parseXML("SolarSystem.xml");
	scene = fullScene->getScene();
	lights = fullScene->getLights();
	cout << "|GUIDE|\n|w/s = move/zoom \n|arrow keys = look-around\n|q = toogle orbitas\n|p = pause \n";
	showOrbit = false;
	isPause = false;

	setupCam();
	// enter GLUT's main loop
	glutMainLoop();

	return 1;
}

