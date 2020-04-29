#include "headers/VerticesList.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

void geraPontosFich(VerticesList * vL, string figura, float r, float g, float b) {
  fstream fs;
  fs.open("3dfiles/" + figura + ".3d", fstream::out);

  vector <float> vector = vL->getPoints();

  fs << to_string(vector.size()/3) << endl;
  fs << r << " ";
  fs << g << " ";
  fs << b << endl;

  for(int i = 0; i < vector.size() ; i++) {
    if (i % 3 == 2) fs << vector[i] << endl;
    else fs << vector[i] << " ";
  }

  fs.close();

}


int main(int argc, char* argv[]) {
	VerticesList* vL = new VerticesList();
	string figura;

	if (strcmp(argv[1], "plane") == 0 && argc == 7) {
		float size = stof(argv[2]);
		float r, g, b;
		r = stof(argv[3]);
		g = stof(argv[4]);
		b = stof(argv[5]);
		figura = argv[6];
		vL->plane(size);
		geraPontosFich(vL, figura,r,g,b);
		return 0;
	}

  if (strcmp(argv[1], "box") == 0 && argc == 9) {
    float coord_x = stof(argv[2]);
    float coord_y = stof(argv[3]);
    float coord_z = stof(argv[4]);
	float r, g, b;
	r = stof(argv[5]);
	g = stof(argv[6]);
	b = stof(argv[7]);
    vL->box(coord_x,coord_y,coord_z);
    figura = argv[8];
    geraPontosFich(vL, figura,r,g,b);
    return 0;
  }

	if (strcmp(argv[1], "sphere") == 0 && argc == 9) {
		float raio = stof(argv[2]);
		int slices = atoi(argv[3]);
		int stacks = atoi(argv[4]);
		float r, g, b;
		r = stof(argv[5]);
		g = stof(argv[6]);
		b = stof(argv[7]);
		figura = argv[8];
		vL->sphere(raio, slices, stacks);
		geraPontosFich(vL, figura,r,g,b);
		return 0;
	}

	if (strcmp(argv[1], "cone") == 0 && argc == 10) {
		float raio = stof(argv[2]);
		float altura = stof(argv[3]);
		int slices = atoi(argv[4]);
		int stacks = atoi(argv[5]);
		int r, g, b;
		r = stof(argv[6]);
		g = stof(argv[7]);
		b = stof(argv[8]);
		figura = argv[9];
		vL->cone(raio,altura,slices,stacks);
		geraPontosFich(vL, figura,r,g,b);
		return 0;
	}
}
