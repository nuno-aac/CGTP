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

void geraPontosBezier(VerticesList* vl, string file, string figura, int tesselation) {
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
		cout << n << "\n";
		patches = n;
		totalControlOrder = numPerPatch * patches;
		int pointsOrder[140]; ///////////////////////////////////////////////////////////// MUDAR PARA totalControlOrder /////////////////////////////////////////////////////////////
		while (i < patches) {
			while (j < numPerPatch && fs >> skipws >> n >> v) {
				pointsOrder[k] = n;
				k++;
				j++;				
			}
			j = 1;
			fs >> n;
			pointsOrder[k] = n;
			k++;
			i++;
		}
		fs >> n;
		totalControlPoints = n;
		getline(fs, line);
		cout << line;
		float pointsX[4096]; ///////////////////////////////////////////////////////////// MUDAR PARA totalControlPoints /////////////////////////////////////////////////////////////
		float pointsZ[4096]; ///////////////////////////////////////////////////////////// MUDAR PARA totalControlPoints /////////////////////////////////////////////////////////////
		float pointsY[4096]; ///////////////////////////////////////////////////////////// MUDAR PARA totalControlPoints /////////////////////////////////////////////////////////////
		for (i = 0; i < totalControlPoints; i++) {
			(fs >> skipws >> f >> v);
			cout << f << " ";
			pointsX[l] = f;
			l++;
			(fs >> skipws >> f >> v);
			cout << f << " ";
			pointsZ[l] = f;
			l++;
			(fs >> skipws >> f);
			cout << f << '\n';
			pointsY[l] = f;
			l++;
		}
		for (k = 0; k < totalControlOrder; k++) {
			cout << pointsOrder[k] << " ";
		}
		cout << '\n';
		for (l = 0; l < totalControlPoints; l++) {
			cout << pointsX[l] << " ";
			cout << pointsZ[l] << " ";
			cout << pointsY[l] << "\n";
		}
		float bezCurve[84]; ///////////////////////////////////////////////////////////// MUDAR PARA totalControlPoints * 3 /////////////////////////////////////////////////////////////
		float auxX[15]; ///////////////////////////////////////////////////////////// MUDAR PARA (tesselation + 1) * (sqrt(numPerPatch) - 1) /////////////////////////////////////////////////////////////
		float auxZ[15]; ///////////////////////////////////////////////////////////// MUDAR PARA (tesselation + 1) * (sqrt(numPerPatch) - 1) /////////////////////////////////////////////////////////////
		float auxY[15]; ///////////////////////////////////////////////////////////// MUDAR PARA (tesselation + 1) * (sqrt(numPerPatch) - 1) /////////////////////////////////////////////////////////////
		for (n = 0; n < patches; n++) { // patches
			for(j = 0; j < sqrt(numPerPatch); j++){ //curves
				for (k = sqrt(numPerPatch) - 1; k > 0; k--) { //level of func
					for (i = 0; i < k; i++) { //func
						for (l = 0; l <= tesselation; l++) { // individual point
							//aux[((i + 1) * (tesselation + 1)) - (tesselation + 1) + l] = (1 - ((1/tesselation + 1) * l))
						}
						cout << "....... calc ......." << '\n';
					}
					cout << ",,,,,,, level " << k <<" ,,,,,,," << '\n';
				}
				cout << "------ curve " << j + 1 << " -------" << '\n';
			}
			cout << "======= patch " << n + 1 << " =======" << '\n';
		}
		free(pointsOrder);
		free(pointsX);
		free(pointsZ);
		free(pointsY);
		free(bezCurve);
		free(auxX);
		free(auxZ);
		free(auxZ);
		fs.close();
	}

	else cout << "Unable to open file";

}


int main(int argc, char* argv[]) {
	VerticesList* vL = new VerticesList();
	string figura;

	geraPontosBezier(vL, argv[1], argv[2], atoi(argv[3]));

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
