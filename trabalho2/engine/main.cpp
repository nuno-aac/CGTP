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
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES


using namespace std;

vector<Group*> scene;
vector<vector<float>> vertices;

float vert = 0.0f;
float hor = 0.0f;
float ang = 0.0f;
float zoom = 0.0f;

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

void applyTransformations(Group* g) {
	Transformation* t;

	t = g -> getTranslation();
	if (t != NULL) {
		glTranslatef(t->getX(), t->getY(), t->getZ());
	}

	t = g -> getRotation();
	if(t != NULL)
		glRotatef(t -> getAngle(), t->getX(), t->getY(), t->getZ());

	t = g -> getScale();
	if (t != NULL)
		glScalef(t->getX(), t->getY(), t->getZ());
}

void drawModel(Model* model) {
	glColor3f(model -> getR(), model -> getG(), model -> getB());

	glBegin(GL_TRIANGLES);
	for (int v = 0; v < model->getVertices().size() - 2; v+=3) {
		glVertex3f(model -> getVertice(v), model->getVertice(v+1), model->getVertice(v + 2));
	}
	glEnd();
}

void drawModels(vector<Model*> models){
	for (int i = 0; i < models.size(); i++) {
		drawModel(models[i]);
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

	// End of frame
	glutSwapBuffers();
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
	default:
		break;
	}
	glutPostRedisplay();
}
 
using namespace std;

int main(int argc, char** argv) {
	//Get models
	scene = parseXML("SolarSystem.xml");
	cout << "+/- = zoom | z/x = rotacao | arrow keys = mover o modelo";
	zoom = 23;
	vert = 25;

	// put GLUT init here
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Trabalho fase 1");

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

