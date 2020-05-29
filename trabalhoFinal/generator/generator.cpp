#include "headers/VerticesList.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

vector<float> normalX;
vector<float> normalY;
vector<float> normalZ;

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



float calculatePoints(float ti, float tj, vector<float> vet, int patch, int flagU, int flagV) {
	int m[4][4]{ {-1,3,-3,1},{3,-6,3,0},{-3,3,0,0},{1,0,0,0} };
	float u[4] = { ti *ti*ti,ti *ti,ti,1 };
	if (flagU == 1) {
		u[0] = 3 * ti * ti;
		u[1] = 2 * ti;
		u[2] = 1;
		u[3] = 0;
	}
	float v[4] = { tj * tj * tj,tj * tj,tj,1 };
	if (flagV == 1) {
		v[0] = 3 * tj * tj;
		v[1] = 2 * tj;
		v[2] = 1;
		v[3] = 0;
	}
	float r[4] = { 0 };
	float ret = 0;
	/*if (print) {
		for (int i = 0; i < 4; i++) {
			cout << "u " << u[i] << " ";
		}
		cout << '\n';
	}
	if (print) {
		for (int i = 0; i < 4; i++) {
			cout << "v " << v[i] << " ";
		}
		cout << '\n';
	}*/
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			r[i] += u[j] * m[j][i];
		}
	}
	/*if (print) {
		for (int i = 0; i < 4; i++) {
			cout << "r1 " << r[i] << " ";
		}
		cout << '\n';
	}*/
	for (int i = 0; i < 4; i++) {
		u[i] = 0;
	}	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			u[i] += r[j] * vet.at(4*j + i + (16 * patch));
			//if(print) cout << "vet " << vet.at(4 * i + j + (16 * patch)) << ' ';
		}
		//if(print) cout << '\n';
	}
	/*if (print) {
		for (int i = 0; i < 4; i++) {
			cout << "u1 " << u[i] << " ";
		}
		cout << '\n';
	}*/
	for (int i = 0; i < 4; i++) {
		r[i] = 0;
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			r[i] += u[j] * m[j][i];
		}
	}
	/*if (print) {
		for (int i = 0; i < 4; i++) {
			cout << "r2 " << r[i] << " ";
		}
		cout << '\n';
	}*/
	for (int j = 0; j < 4; j++) {
		ret += r[j] * v[j];
	}
	/*if (print) {
		cout << "ret " << ret << '\n';
	}*/
	return ret;
}


void cross(float a0, float a1, float a2, float b0, float b1, float b2, float* res) {
	res[0] = a1 * b2 - a2 * b1;
	res[1] = a2 * b0 - a0 * b2;
	res[2] = a0 * b1 - a1 * b0;
}


void normalize(float* a) {

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = -a[0] / l;
	a[1] = -a[1] / l;
	a[2] = -a[2] / l;
}

void computeNormal(float uX, float uY, float uZ, float vX, float vY, float vZ, float* normalV, float z) {
	cross(uX, uY, uZ, vX, vY, vZ, normalV);
	normalize(normalV);

	if (normalV[0] != normalV[0] || normalV[1] != normalV[1] || normalV[2] != normalV[2]) {
		if (z > 0) {
			normalV[0] = 0;
			normalV[1] = 0;
			normalV[2] = 1;
		}
		if (z <= 0) {
			normalV[0] = 0;
			normalV[1] = 0;
			normalV[2] = -1;
		}
	}
}

void geraPontosBezier(string file, string figura, int const tesselation,float r, float g, float b) {
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
	fstream fsNorms;
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
		fs.close();
		//calculate points

		vector<float> tempX;
		vector<float> tempY;
		vector<float> tempZ;

		for each (int var in pointsOrder)
		{
			tempX.push_back(pointsX.at(var));
			tempY.push_back(pointsY.at(var));
			tempZ.push_back(pointsZ.at(var));
		}

		float mX[20][20] = { 0 };
		float mY[20][20] = { 0 };
		float mZ[20][20] = { 0 };
		float mUX[20][20] = { 0 };
		float mUY[20][20] = { 0 };
		float mUZ[20][20] = { 0 };
		float mVX[20][20] = { 0 };
		float mVY[20][20] = { 0 };
		float mVZ[20][20] = { 0 };

		fs.open("3dfiles/" + figura + ".3d", fstream::out);

		fs << to_string(patches * tesselation * tesselation * 6) << endl;
		fs << r << " ";
		fs << g << " ";
		fs << b << endl;

		//calculate normals
		fsNorms.open("3dfiles/" + figura + ".3dn", fstream::out);

		float norm[3] = { 0 };
		float mNormaisX[20][20] = { 0 };
		float mNormaisY[20][20] = { 0 };
		float mNormaisZ[20][20] = { 0 };

		for (int patch = 0; patch < patches; patch++) {
			

			for (int j = 0; j <= tesselation; j++) {
				for (int i = 0; i <= tesselation; i++) {
					float ti = (1.0 / (float)tesselation) * i;
					float tj = (1.0 / (float)tesselation) * j;

					mX[i][j] = calculatePoints(ti, tj, tempX, patch, 0, 0);
					mY[i][j] = calculatePoints(ti, tj, tempY, patch, 0, 0);
					mZ[i][j] = calculatePoints(ti, tj, tempZ, patch, 0, 0);

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

			for (int j = 0; j <= tesselation; j++) {
				for (int i = 0; i <= tesselation; i++) {
					float ti = (1.0 / (float)tesselation) * i;
					float tj = (1.0 / (float)tesselation) * j;

					float flagX, flagY, flagZ;

					mUX[i][j] = calculatePoints(ti, tj, tempX, patch, 1, 0);
					mUY[i][j] = calculatePoints(ti, tj, tempY, patch, 1, 0);
					mUZ[i][j] = calculatePoints(ti, tj, tempZ, patch, 1, 0);

					mVX[i][j] = calculatePoints(ti, tj, tempX, patch, 0, 1);
					mVY[i][j] = calculatePoints(ti, tj, tempY, patch, 0, 1);
					mVZ[i][j] = calculatePoints(ti, tj, tempZ, patch, 0, 1);

				}
			}

			for (int j = 0; j <= tesselation; j++) {
				for (int i = 0; i <= tesselation; i++) {
					computeNormal(mUX[i][j], mUY[i][j], mUZ[i][j], mVX[i][j], mVY[i][j], mVZ[i][j], norm, mZ[i][j]);

					mNormaisX[i][j] = norm[0];
					mNormaisY[i][j] = norm[1];
					mNormaisZ[i][j] = norm[2];
				}
			}

			for (int j = 0; j < tesselation; j++) {
				for (int i = 0; i < tesselation; i++) {

					fsNorms << mNormaisX[i][j] << " " << mNormaisY[i][j] << " " << mNormaisZ[i][j] << endl;
					fsNorms << mNormaisX[i][j + 1] << " " << mNormaisY[i][j + 1] << " " << mNormaisZ[i][j + 1] << endl;
					fsNorms << mNormaisX[i + 1][j] << " " << mNormaisY[i + 1][j] << " " << mNormaisZ[i + 1][j] << endl;

					fsNorms << mNormaisX[i][j + 1] << " " << mNormaisY[i][j + 1] << " " << mNormaisZ[i][j + 1] << endl;
					fsNorms << mNormaisX[i + 1][j + 1] << " " << mNormaisY[i + 1][j + 1] << " " << mNormaisZ[i + 1][j + 1] << endl;
					fsNorms << mNormaisX[i + 1][j] << " " << mNormaisY[i + 1][j] << " " << mNormaisZ[i + 1][j] << endl;

				}
			}
		}

		fs.close();
		fsNorms.close();
	}

	else cout << "Unable to open file";

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
