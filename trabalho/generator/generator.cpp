#include "headers/VerticesList.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

void geraPontosFich(VerticesList * vL, string figura) {
  fstream fs;
  fs.open("3dfiles/" + figura + ".3d", fstream::out);

  vector <float> vector = vL->getPoints();

  fs << to_string(vector.size()/3) << endl;

  for(int i = 0; i < vector.size() ; i++) {
    if (i % 3 == 2) fs << vector[i] << endl;
    else fs << vector[i] << " ";
  }

  fs.close();

}


int main(int argc, char* argv[]) {
	VerticesList* vL = new VerticesList();
	string figura;

	if (strcmp(argv[1], "plane") == 0 && argc == 4) {
		float size = stof(argv[2]);
		figura = argv[3];
		vL->plane(size);
		geraPontosFich(vL, figura);
		return 0;
	}

  if (strcmp(argv[1], "box") == 0 && argc == 6) {
    float coord_x = stof(argv[2]);
    float coord_y = stof(argv[3]);
    float coord_z = stof(argv[4]);
    vL->box(coord_x,coord_y,coord_z);
    figura = argv[5];
    geraPontosFich(vL, figura);
    return 0;
  }

	if (strcmp(argv[1], "sphere") == 0 && argc == 6) {
		float raio = stof(argv[2]);
		int slices = atoi(argv[3]);
		int stacks = atoi(argv[4]);
		figura = argv[5];
		vL->sphere(raio, slices, stacks);
		geraPontosFich(vL, figura);
		return 0;
	}

	if (strcmp(argv[1], "cone") == 0 && argc == 7) {
		float raio = stof(argv[2]);
		float altura = stof(argv[3]);
		int slices = atoi(argv[4]);
		int stacks = atoi(argv[5]);
		figura = argv[6];
		vL->cone(raio,altura,slices,stacks);
		geraPontosFich(vL, figura);
		return 0;
	}
}
