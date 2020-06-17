#include <stdlib.h>
#include <string>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "tinyxml2.cpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES


using namespace std;



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

vector<vector<float>> vertices;

float vert = 0.0f;
float hor = 0.0f;
float ang = 0.0f;

using namespace tinyxml2;

vector<string> readXML(string nome) {
	vector<string> res;
	XMLDocument d;
	string str;
	string path = "..\\XML\\" + nome;
	if (!(d.LoadFile(path.c_str()))) {
		XMLElement* root = d.FirstChildElement("scene");
		if (root == nullptr) return res;
		XMLElement* elemento = root->FirstChildElement("model");
		while (elemento != nullptr) {
			str = elemento->Attribute("file");
			res.push_back(str);
			elemento = elemento->NextSiblingElement();
		}
		return res;
	}
	return res;
}

void parseFile(string file) {
	float x, y, z;
	vector<float> figura;
	ifstream infile(file);
	infile >> x;
	while (infile >> x >> y >> z) {
		figura.push_back(x);
		figura.push_back(y);
		figura.push_back(z);
	}
	vertices.push_back(figura);
}

void renderScene(void) {
	int size, numFigura;
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0, 5.0, 5.0,
		0.0, 0.0, 0.0,
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
	glTranslatef(vert, 0.0f, hor);
	glRotatef(ang, 0.0f, 0.0f, 1.0f);
		//TRIANGLE
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (numFigura = vertices.size() - 1; numFigura >= 0; numFigura--) {
		size = vertices.at(numFigura).size();
		for (int i = 0; i < size - 2; i += 3) {
			glColor3f(0.6f + i / 50, 0.6f + i / 10000, 0.6f + i / 100000);
			glVertex3f(vertices.at(numFigura).at(i), vertices.at(numFigura).at(i + 1), vertices.at(numFigura).at(i + 2));
		}
	}
	glEnd();
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
	case 'z':
		ang -= 1.0f;
		break;
	case 'x':
		ang += 1.0f;
		break;

	default:
		break;
	}
	glutPostRedisplay();
}
 
using namespace std;

int main(int argc, char** argv) {
	//Get models
	vector<string> models = readXML("modelBox.xml");
	for (int i = models.size() - 1; i >= 0; i--)
		parseFile("..\\..\\generator\\3dfiles\\" + models.at(i));

	// put GLUT init here
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Trabalho fase 1");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

