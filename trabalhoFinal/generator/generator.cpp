#include "headers/VerticesList.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

vector<float> normal;

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

/*
void geraPontosNormal(VerticesList* vN, string figura) {
	fstream fs;
	fs.open("3dfiles/" + figura + ".3dn", fstream::out);

	vector <float> vector = vN->getPoints();

	for (int i = 0; i < vector.size(); i++) {
		if (i % 3 == 2) fs << vector[i] << endl;
		else fs << vector[i] << " ";
	}

	fs.close();

}*/

int countPontos(string line) {
	int r = 0;
	for (int i = 0; i < line.length(); i++) {
		if (line[i] == ',') r++;
	}
	r++;
	return r;
}
/*
void cross(float* a, float* b, float* res) {
	//std::cout << a[0] << '/' << a[1] << '/' << a[2] << '\n';
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}


void normalize(float* a) {

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}

void subtractV(float* a, float* b, float* res) {
	//std::cout << a[0] << '/' << a[1] << '/' << a[2] << '\n';
	res[0] = a[0] - b[0];
	res[1] = a[1] - b[1];
	res[2] = a[2] - b[2];
}

void computeNormal(float p1[], float p2[], float p3[], float p4[]) {
	float v1[3];
	float v2[3];
	float normalV[3];

	subtractV(p2, p1, v1);
	subtractV(p4, p3, v2);

	
	cross(v1, v2, normalV);
	normalize(normalV);
	//cout << normalV[0] << '/' << normalV[1] << '/' << normalV[2] << '\n';

	normal.push_back(normalV[0]);
	normal.push_back(normalV[1]);
	normal.push_back(normalV[2]);
}*/

float calculateNormal(float ti, float tj, vector<float> vet, int patch) {
	int m[4][4]{ {-1,3,-3,1},{3,-6,3,0},{-3,3,0,0},{1,0,0,0} };
	float u[4] = { ti *ti*ti,ti *ti,ti,1 };
	float v[4] = { tj * tj * tj,tj * tj,tj,1 };
	float r[4] = { 0 };
	float ret = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			r[i] += u[j] * m[j][i];
		}
	}
	for (int i = 0; i < 4; i++) {
		u[i] = 0;
	}	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			u[i] += r[j] * vet.at(4*j + i + (16 * patch));
		}
	}
	for (int i = 0; i < 4; i++) {
		r[i] = 0;
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			r[i] += u[j] * m[j][i];
		}
	}
	for (int j = 0; j < 4; j++) {
		ret += r[j] * v[j];
	}
	return ret;
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
					//points

					float p1[3] = { bezPatchX.at((k * ((tesselation + 1) * (tesselation + 1))) + i + (j * (tesselation + 1))), bezPatchY.at((k * ((tesselation + 1) * (tesselation + 1))) + i + (j * (tesselation + 1))), bezPatchZ.at((k * ((tesselation + 1) * (tesselation + 1))) + i + (j * (tesselation + 1))) };
					float p2[3] = { bezPatchX.at((k * ((tesselation + 1) * (tesselation + 1))) + i + ((j + 1) * (tesselation + 1))), bezPatchY.at((k * ((tesselation + 1) * (tesselation + 1))) + i + ((j + 1) * (tesselation + 1))),bezPatchZ.at((k * ((tesselation + 1) * (tesselation + 1))) + i + ((j + 1) * (tesselation + 1))) };
					float p3[3] = { bezPatchX.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + (j * (tesselation + 1))), bezPatchY.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + (j * (tesselation + 1))), bezPatchZ.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + (j * (tesselation + 1))) };
					float p4[3] = { bezPatchX.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + ((j + 1) * (tesselation + 1))),  bezPatchY.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + ((j + 1) * (tesselation + 1))), bezPatchZ.at((k * ((tesselation + 1) * (tesselation + 1))) + i + 1 + ((j + 1) * (tesselation + 1))) };
					

					//write to file

					fs << p1[0] << " " << p1[1] << " " << p1[2] << endl;
					fs << p2[0] << " " << p2[1] << " " << p2[2] << endl;
					fs << p3[0] << " " << p3[1] << " " << p3[2] << endl;

					fs << p2[0] << " " << p2[1] << " " << p2[2] << endl;
					fs << p4[0] << " " << p4[1] << " " << p4[2] << endl;
					fs << p3[0] << " " << p3[1] << " " << p3[2] << endl;
					
				}
			}
		}
		fs.close();

		//calculate normals

		vector<float> tempX;
		vector<float> tempY;
		vector<float> tempZ;

		for each (int var in pointsOrder)
		{
			tempX.push_back(pointsX.at(var));
			tempY.push_back(pointsY.at(var));
			tempZ.push_back(pointsZ.at(var));
		}

		float mX[5][5] = { 0 };
		float mY[5][5] = { 0 };
		float mZ[5][5] = { 0 };

		fs.open("3dfiles/wowow.3d", fstream::out);

		fs << to_string(patches * tesselation * tesselation * 6) << endl;
		fs << r << " ";
		fs << g << " ";
		fs << b << endl;


		for (int patch = 0; patch < patches; patch++) {
			for (int j = 0; j <= tesselation; j++) {
				for (int i = 0; i <= tesselation; i++) {
					float ti = (1.0 / (float)tesselation) * i;
					float tj = (1.0 / (float)tesselation) * j;

					mX[i][j] = calculateNormal(ti, tj, tempX, patch);
					mY[i][j] = calculateNormal(ti, tj, tempY, patch);
					mZ[i][j] = calculateNormal(ti, tj, tempZ, patch);

					cout << mX[i][j] << ' ';
					cout << mY[i][j] << ' ';
					cout << mZ[i][j] << '\n';

				}
			}

			for (int j = 0; j < tesselation; j++) {
				for (int i = 0; i < tesselation; i++) {

					fs << mX[i][j] << " " << mY[i][j] << " " << mZ[i][j] << endl;
					fs << mX[i][j + 1] << " " << mY[i][j + 1] << " " << mZ[i][j + 1] << endl;
					fs << mX[i + 1][j] << " " << mY[i + 1][j] << " " << mZ[i + 1][j] << endl;

					fs << mX[i][j + 1] << " " << mY[i][j + 1] << " " << mZ[i][j + 1] << endl;
					fs << mX[i + 1][j + 1] << " " << mY[i + 1][j + 1] << " " << mZ[i + 1][j + 1] << endl;
					fs << mX[i + 1][j] << " " << mY[i + 1][j] << " " << mZ[i + 1][j] << endl;

				}
			}
		}

		fs.close();
	}

	else cout << "Unable to open file";

}

void geraNormaisFich(string figura) {
	fstream fs;
	fs.open("3dfiles/" + figura + ".3dn", fstream::out);

	for (int i = 0; i < normal.size(); i++) {
		if (i % 3 == 2) fs << normal[i] << endl;
		else fs << normal[i] << " ";
	}

	fs.close();

}

int main(int argc, char* argv[]) {
	VerticesList* vL = new VerticesList();
	VerticesList* vN = new VerticesList();
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
		geraNormaisFich(fileOut);
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
		vN->sphereNormal(raio, slices, stacks);
		geraPontosFich(vL, figura,r,g,b);
		//geraPontosNormal(vN, figura);
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
