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

float p[5][3] = {{-1,-1,0},{-1,1,0},{1,1,0},{0,0,0},{1,-1,0}};

using namespace std;

vector<Group*> scene;
vector<vector<float>> vertices;

GLuint modelsBuf[300];
int currentModel = 0;

float time = 0.0f;
float vert = 0.0f;
float hor = 0.0f;
float ang = 0.0f;
float zoom = 0.0f;

bool showOrbit;

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

void drawCurve(Catmull* c) {
	float pos[3];
	float deriv[3];
	vector<float> curveVertex;
	for (float tcurve = 0; tcurve < c->getTime(); tcurve += 0.01) {
		getGlobalCatmullRomPoint(tcurve, c, pos, deriv);
		//cout << pos[0] << pos[1] << pos[2] << '\n';
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

	rotA = g->getRotationAnimation();
	if (rotA != NULL) {
		glRotatef(getRotationAngle(rotA), rotA->getX(), rotA->getY(), rotA->getZ());
	}

	t = g -> getTranslation();
	if (t != NULL) {
		glTranslatef(t->getX(), t -> getY(), t -> getZ());
	}

	t = g -> getStaticRotation();
	if(t != NULL)
		glRotatef(t -> getAngle(), t->getX(), t->getY(), t->getZ());

	t = g -> getScale();
	if (t != NULL)
		glScalef(t->getX(), t->getY(), t->getZ());
}


void drawModel(Model* model) {
	vector<float> modelVertex = model->getVertices();
	glColor3f(model -> getR(), model -> getG(), model -> getB());
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, modelsBuf[currentModel]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model->getVertices().size(), &modelVertex[0], GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, model->getVertices().size()/3);
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

void renderScene(void) {
	int size, numFigura;
	// clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(zoom+vert, zoom, zoom + hor,
		vert, 0.0, hor,
		0.0f, 1.0f, 0.0f);

	// put the geometric transformations here


	//glTranslatef(-5.0f, 0.0f, 0.0f);


	// put drawing instructions here
			//AXIS
	glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
	//glTranslatef(vert, 0.0f, hor);
	glRotatef(ang, 0.0f, 1.0f, 0.0f);
	for (int i = 0; i < scene.size(); i++) {
		drawGroup(scene[i]);
	}
	currentModel = 0;
	// End of frame
	glutSwapBuffers();
	time = glutGet(GLUT_ELAPSED_TIME);
	time /= 1000;
}

void keyUp(int keyCode, int x, int y){

	switch (keyCode)    {
	case GLUT_KEY_UP:
		vert -= 0.05f;
		break;
	case GLUT_KEY_DOWN:
		vert += 0.05f;
		break;
	case GLUT_KEY_RIGHT:
		hor -= 0.05f;
		break;
	case GLUT_KEY_LEFT:
		hor += 0.05f;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void keyRotate(unsigned char keyCode, int x, int y) {

	switch (keyCode) {
	case '-':
		zoom += 0.5;
		break;
	case '+':
		zoom -= 0.5;
		break;
	case 'z':
		ang += 1;
		break;
	case 'x':
		ang -= 1;
		break;
	case 's':
		showOrbit = !showOrbit;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

using namespace std;

int main(int argc, char** argv) {
	//Get models
	scene = parseXML("SolarSystem.xml");
	cout << "|+/- = zoom \n|z/x = rotacao \n|arrow keys = mover o modelo\n|s = toogle orbitas";
	zoom = 300;
	vert = 0;
	hor = 0;
	showOrbit = false;

	// put GLUT init here
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Trabalho fase 1");

	glewInit();
	glGenBuffers(300, modelsBuf);
	// put callback registration here
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	//key handling
	glutSpecialFunc(keyUp);
	glutKeyboardFunc(keyRotate);

	// OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// enter GLUT's main loop
	glutMainLoop();

	return 1;
}

