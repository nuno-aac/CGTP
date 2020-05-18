#include "headers/VerticesList.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>

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

int countPontos(string line) {
	int r = 0;
	for (int i = 0; i < line.length(); i++) {
		if (line[i] == ',') r++;
	}
	r++;
	return r;
}

void geraPontosBezier(string file, string figura, int tesselation,float r, float g, float b) {
	int i = 0;
	int k = 0;
	int n;
	char o;
	float f;
	int j = 1;
	char v;
	int l = 0;
	int patches;
	int numPerPatch = 0;
	int totalControlOrder;
	int totalControlPoints;
	fstream fs;
	string line;
	fs.open(file, fstream::in);
	if (fs.is_open())
	{
		fstream fin("file", fstream::in);
		getline(fs, line);
		getline(fs, line);
		numPerPatch = countPontos(line);
		fs.close();
		fs.open(file, fstream::in);
		fs >> skipws >> n;
		patches = n;
		totalControlOrder = numPerPatch * patches;
		vector <int> pointsOrder;
		while (i < patches) {
			while (j < numPerPatch && fs >> skipws >> n >> v) {
				pointsOrder.push_back(n);
				j++;				
			}
			j = 1;
			fs >> n;
			pointsOrder.push_back(n);
			i++;
		}
		fs >> n;
		totalControlPoints = n;
		getline(fs, line);
		vector <float> pointsX;
		vector <float> pointsY;
		vector <float> pointsZ;
		for (i = 0; i < totalControlPoints; i++) {
			(fs >> skipws >> f >> v);
			pointsX.push_back(f);
			l++;
			(fs >> skipws >> f >> v);
			pointsY.push_back(f);
			l++;
			(fs >> skipws >> f);
			pointsZ.push_back(f);
			l++;
		}
		vector <float> bezCurveX;
		vector <float> bezCurveY;
		vector <float> bezCurveZ;
		vector <float> auxX;
		vector <float> auxY;
		vector <float> auxZ;
		for (i = 0; i < (tesselation + 1) * (sqrt(numPerPatch) - 1); i++) {
			auto iteX = auxX.begin() + i;
			auto iteY = auxY.begin() + i;
			auto iteZ = auxZ.begin() + i;
			auxX.insert(iteX, 0);
			auxY.insert(iteY, 0);
			auxZ.insert(iteZ, 0);
		}
		for (n = 0; n < patches; n++) { // patches
			for (j = 0; j < sqrt(numPerPatch); j++) { //curves
				for (k = sqrt(numPerPatch) - 1; k > 0; k--) { //level of func
					for (i = 0; i < k; i++) { //func
						for (l = 0; l <= tesselation; l++) { // individual point
							int tes = i * (tesselation + 1) + l;
							float t = ((1 / ((float) tesselation)) * l);
							if (k == sqrtf(numPerPatch) - 1) {
								auxX[tes] = (1 - t) * pointsX.at(pointsOrder.at((n * numPerPatch) + (i + (sqrt(numPerPatch) * j)))) + (t * pointsX.at(pointsOrder.at((n * numPerPatch) + (i + (sqrt(numPerPatch) * j)) + 1)));
								auxY[tes] = (1 - t) * pointsY.at(pointsOrder.at((n * numPerPatch) + (i + (sqrt(numPerPatch) * j)))) + (t * pointsY.at(pointsOrder.at((n * numPerPatch) + (i + (sqrt(numPerPatch) * j)) + 1)));
								auxZ[tes] = (1 - t) * pointsZ.at(pointsOrder.at((n * numPerPatch) + (i + (sqrt(numPerPatch) * j)))) + (t * pointsZ.at(pointsOrder.at((n * numPerPatch) + (i + (sqrt(numPerPatch) * j)) + 1)));
							}
							else {
								auxX[tes] = (1 - t) * auxX.at(((tesselation + 1) * i) + l) + (t * auxX.at(((tesselation + 1) * (i + 1)) + l));
								auxY[tes] = (1 - t) * auxY.at(((tesselation + 1) * i) + l) + (t * auxY.at(((tesselation + 1) * (i + 1)) + l));
								auxZ[tes] = (1 - t) * auxZ.at(((tesselation + 1) * i) + l) + (t * auxZ.at(((tesselation + 1) * (i + 1)) + l));
							}
							if (k == 1) {
								bezCurveX.push_back(auxX.at(tes));
								bezCurveY.push_back(auxY.at(tes));
								bezCurveZ.push_back(auxZ.at(tes));
							}
						}
					}
				}
			}
		}
		vector <float> bezPatchX;
		vector <float> bezPatchY;
		vector <float> bezPatchZ;

		for (n = 0; n < patches; n++) { // patches
			for (j = 0; j < tesselation + 1; j++) { //tesselation
				for (k = sqrt(numPerPatch) - 1; k > 0; k--) { //level of func
					for (i = 0; i < k; i++) { //func
						for (l = 0; l <= tesselation; l++) { // indivual point
							int tes = i * (tesselation + 1) + l;
							float t = ((1 / ((float)tesselation)) * l);

							


							if (k == sqrtf(numPerPatch) - 1) {
								auxX[tes] = (1 - t) * bezCurveX.at((n * ((tesselation + 1) * sqrt(numPerPatch))) + ((tesselation + 1) * i) + j) + (t * bezCurveX.at((n * ((tesselation + 1) * sqrt(numPerPatch))) + ((tesselation + 1) * (i + 1)) + j));
								auxY[tes] = (1 - t) * bezCurveY.at((n * ((tesselation + 1) * sqrt(numPerPatch))) + ((tesselation + 1) * i) + j) + (t * bezCurveY.at((n * ((tesselation + 1) * sqrt(numPerPatch))) + ((tesselation + 1) * (i + 1)) + j));
								auxZ[tes] = (1 - t) * bezCurveZ.at((n * ((tesselation + 1) * sqrt(numPerPatch))) + ((tesselation + 1) * i) + j) + (t * bezCurveZ.at((n * ((tesselation + 1) * sqrt(numPerPatch))) + ((tesselation + 1) * (i + 1)) + j));
							}
							else {
								auxX[tes] = (1 - t) * auxX.at(((tesselation + 1) * i) + l) + (t * auxX.at(((tesselation + 1) * (i + 1)) + l));
								auxY[tes] = (1 - t) * auxY.at(((tesselation + 1) * i) + l) + (t * auxY.at(((tesselation + 1) * (i + 1)) + l));
								auxZ[tes] = (1 - t) * auxZ.at(((tesselation + 1) * i) + l) + (t * auxZ.at(((tesselation + 1) * (i + 1)) + l));
							}
							if (k == 1) {
								bezPatchX.push_back(auxX.at(tes));
								bezPatchY.push_back(auxY.at(tes));
								bezPatchZ.push_back(auxZ.at(tes));
							}
						}
					}
				}
			}
		}
		fs.close();

		fs.open("3dfiles/" + figura + ".3d", fstream::out);

		fs << to_string(patches * tesselation * tesselation * 6) << endl;
		fs << r << " ";
		fs << g << " ";
		fs << b << endl;

		for (k = 0; k < patches; k++) {
			for (j = 0; j < tesselation; j++) {
				for (i = 0; i < tesselation; i++) {
					fs << bezPatchX.at((k * ((tesselation + 1) * (tesselation + 1))) + i + (j * (tesselation + 1))) << " " << bezPatchY.at((k * ((tesselation + 1) * (tesselation + 1))) + i + (j * (tesselation + 1))) << " " << bezPatchZ.at((k * ((tesselation + 1) * (tesselation + 1))) + i + (j * (tesselation + 1))) << endl;
					fs << bezPatchX.at((k * ((tesselation + 1) * (tesselation + 1))) + i + ((j + 1) * (tesselation + 1))) << " " << bezPatchY.at((k * ((tesselation + 1) * (tesselation + 1))) + i + ((j + 1) * (tesselation + 1))) << " " << bezPatchZ.at((k * ((tesselation + 1) * (tesselation + 1))) + i + ((j + 1) * (tesselation + 1))) << endl;
					fs << bezPatchX.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + (j * (tesselation + 1))) << " " << bezPatchY.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + (j * (tesselation + 1))) << " " << bezPatchZ.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + (j * (tesselation + 1))) << endl;

					fs << bezPatchX.at((k * ((tesselation + 1) * (tesselation + 1))) + i + ((j + 1) * (tesselation + 1))) << " " << bezPatchY.at((k * ((tesselation + 1) * (tesselation + 1))) + i + ((j + 1) * (tesselation + 1))) << " " << bezPatchZ.at((k * ((tesselation + 1) * (tesselation + 1))) + i + ((j + 1) * (tesselation + 1))) << endl;
					fs << bezPatchX.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + ((j + 1) * (tesselation + 1))) << " " << bezPatchY.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + ((j + 1) * (tesselation + 1))) << " " << bezPatchZ.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + ((j + 1) * (tesselation + 1))) << endl;
					fs << bezPatchX.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + (j * (tesselation + 1))) << " " << bezPatchY.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + (j * (tesselation + 1))) << " " << bezPatchZ.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + (j * (tesselation + 1))) << endl;
				}
			}
		}

		fs.close();
	}

	else cout << "Unable to open file";

}


int main(int argc, char* argv[]) {
	VerticesList* vL = new VerticesList();
	string figura;

	if (strcmp(argv[1], "bezier") == 0 && argc == 8) {
		string fileIn = argv[2];
		string fileOut = argv[3];
		int tesselations = atoi(argv[4]);
		float r, g, b;
		r = stof(argv[5]);
		g = stof(argv[6]);
		b = stof(argv[7]);
		geraPontosBezier(fileIn, fileOut, tesselations,r,g,b);
		return 0;
	}
	

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
