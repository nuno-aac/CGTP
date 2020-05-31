#include "headers/VerticesList.h"
#include <iostream>

VerticesList::VerticesList(){
}

VerticesList::VerticesList(vector<float> p){
  points = p;
}

vector<float> VerticesList::getPoints(){
  return points;
}

vector<float> VerticesList::getNormals() {
    return normals;
}

vector<float> VerticesList::getTextures() {
    return textures;
}

void VerticesList::normalize(float* a) {

    float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0] / l;
    a[1] = a[1] / l;
    a[2] = a[2] / l;
}

void VerticesList::setPoints(vector<float> p){
  points = p;
}

void VerticesList::addPoint(float x, float y, float z){
  points.push_back(x);
  points.push_back(y);
  points.push_back(z);
}

void VerticesList::addNormal(float x, float y, float z){
    float n[3] = { x,y,z };
    normalize(n);
    normals.push_back(n[0]);
    normals.push_back(n[1]);
    normals.push_back(n[2]);
}

void VerticesList::addTextures(float x, float y){
  textures.push_back(x);
  textures.push_back(y);
  //textures.push_back(z);
}

void VerticesList::plane(float size){
  float cord = size/2;
  //T1
    addPoint(cord, 0, cord);
    addNormal(0,1,0);
    addTextures(1, 0);
    addPoint(cord,0,-cord);
    addNormal(0,1,0);
    addTextures(1, 1);
    addPoint(-cord, 0, cord);
    addNormal(0,1,0);
    addTextures(0, 0);
  //T2
    addPoint(-cord, 0, -cord);
    addNormal(0,1,0);
    addTextures(0, 1);
    addPoint(-cord, 0, cord);
    addNormal(0,1,0);
    addTextures(0, 0);
    addPoint(cord, 0, -cord);
    addNormal(0,1,0);
    addTextures(1, 1);
}

void VerticesList::box(float x, float y, float z){
  float cordX = x/2;
  float cordY = y/2;
  float cordZ = z/2;

  //Topo
    //T1
    addPoint(cordX, cordY, cordZ);
    addNormal(0,1,0);
    addTextures(1, 0);
    addPoint(cordX,cordY,-cordZ);
    addNormal(0,1,0);
    addTextures(1, 1);
    addPoint(-cordX, cordY, cordZ);
    addNormal(0,1,0);
    addTextures(0, 0);
    //T2
    addPoint(-cordX, cordY, -cordZ);
    addNormal(0,1,0);
    addTextures(0, 1);
    addPoint(-cordX, cordY, cordZ);
    addNormal(0,1,0);
    addTextures(0, 0);
    addPoint(cordX, cordY, -cordZ);
    addNormal(0,1,0);
    addTextures(1, 1);

  //Base
    //T1
    addPoint(cordX, -cordY, cordZ);
    addNormal(0,-1,0);
    addTextures(1, 0);
    addPoint(-cordX, -cordY, cordZ);
    addNormal(0,-1,0);
    addTextures(1, 1);
    addPoint(cordX,-cordY,-cordZ);
    addNormal(0,-1,0);
    addTextures(0, 0);
    //T2
    addPoint(-cordX, -cordY, -cordZ);
    addNormal(0,-1,0);
    addTextures(0, 1);
    addPoint(cordX, -cordY, -cordZ);
    addNormal(0,-1,0);
    addTextures(0, 0);
    addPoint(-cordX, -cordY, cordZ);
    addNormal(0,-1,0);
    addTextures(1, 1);

  //Lado X positivo
    //T1
    addPoint(cordX, cordY, cordZ);
    addNormal(1,0,0);
    addTextures(0, 1);
    addPoint(cordX, -cordY, cordZ);
    addNormal(1,0,0);
    addTextures(0, 0);
    addPoint(cordX,cordY,-cordZ);
    addNormal(1,0,0);
    addTextures(1, 1);
    //T2
    addPoint(cordX, -cordY, -cordZ);
    addNormal(1,0,0);
    addTextures(1, 0);
    addPoint(cordX, cordY, -cordZ);
    addNormal(1,0,0);
    addTextures(1, 1);
    addPoint(cordX, -cordY, cordZ);
    addNormal(1,0,0);
    addTextures(0, 0);

  //Lado X negativo
    //T1
    addPoint(-cordX, cordY, cordZ);
    addNormal(-1,0,0);
    addTextures(1, 1);
    addPoint(-cordX,cordY,-cordZ);
    addNormal(-1,0,0);
    addTextures(0, 1);
    addPoint(-cordX, -cordY, cordZ);
    addNormal(-1,0,0);
    addTextures(1, 0);
    //T2
    addPoint(-cordX, -cordY, -cordZ);
    addNormal(-1,0,0);
    addTextures(0, 0);
    addPoint(-cordX, -cordY, cordZ);
    addNormal(-1,0,0);
    addTextures(1, 0);
    addPoint(-cordX, cordY, -cordZ);
    addNormal(-1,0,0);
    addTextures(0, 1);

  //Lado Z positivo
    //T1
    addPoint(cordX, cordY, cordZ);
    addNormal(0,0,1);
    addTextures(1, 1);
    addPoint(-cordX, cordY, cordZ);
    addNormal(0,0,1);
    addTextures(0, 1);
    addPoint(cordX, -cordY, cordZ);
    addNormal(0,0,1);
    addTextures(1, 0);
    //T2
    addPoint(-cordX, -cordY, cordZ);
    addNormal(0,0,1);
    addTextures(0, 0);
    addPoint(cordX, -cordY, cordZ);
    addNormal(0,0,1);
    addTextures(1, 0);
    addPoint(-cordX, cordY, cordZ);
    addNormal(0,0,1);
    addTextures(0, 1);

  //Lado Z negativo
    //T1
    addPoint(cordX, cordY, -cordZ);
    addNormal(0,0,-1);
    addTextures(0, 1);
    addPoint(cordX,-cordY, -cordZ);
    addNormal(0,0,-1);
    addTextures(0, 0);
    addPoint(-cordX, cordY, -cordZ);
    addNormal(0,0,-1);
    addTextures(1, 1);
    //T2
    addPoint(-cordX, -cordY, -cordZ);
    addNormal(0,0,-1);
    addTextures(1, 0);
    addPoint(-cordX, cordY, -cordZ);
    addNormal(0,0,-1);
    addTextures(1, 1);
    addPoint(cordX, -cordY, -cordZ);
    addNormal(0,0,-1);
    addTextures(0, 0);
}

void VerticesList::sphere(float r, int slices, int stacks){
  float stackStep = (float) (M_PI) / stacks;
  float sliceStep = (float) (2 * M_PI) / slices;
  float stackText = 1.0 / stacks;
  float sliceText = 1.0 / slices;
  float alpha0, beta0, alpha1, beta1;
  float height, heightBChange, x, xAChange, xBChange, xABChange, z, zAChange, zBChange, zABChange;
  int numStacks = 0;
  alpha0 = 0;
  alpha1 = sliceStep;

  for(int a = 0; a < slices; a++){

    beta0 = (M_PI/2) * 1;
    beta1 = beta0 - stackStep;

    for(int b = 0; b < stacks -1; b++){
      numStacks ++;
      height = sin(beta0) * r;
      heightBChange = sin(beta1) * r;

      x = cos(beta0) * sin(alpha0) * r;
      xAChange = cos(beta0) * sin(alpha1) * r;
      xBChange = cos(beta1) * sin(alpha0) * r;
      xABChange = cos(beta1) * sin(alpha1) * r;

      z = cos(beta0) * cos(alpha0) * r;
      zAChange = cos(beta0) * cos(alpha1) * r;
      zBChange = cos(beta1) * cos(alpha0) * r;
      zABChange = cos(beta1) * cos(alpha1) * r;

      if(b == 0){
        addPoint(0, r, 0);
        addNormal(0, r, 0);
        addTextures(sliceText * a + (sliceText/2), 1);
        addPoint(xBChange , heightBChange, zBChange);
        addNormal(xBChange , heightBChange, zBChange);
        addTextures(sliceText * a, 1 - (stackText * (b+1)));
        addPoint(xABChange , heightBChange, zABChange);
        addNormal(xABChange , heightBChange, zABChange);
        addTextures(sliceText * (a+1), 1 - (stackText * (b+1)));
        if(a == 0){
          cout << "BOTTOM TEXTURES: " << 1 - (stackText * (b+1)) << "| TOP TEXTURE: " << 1 << '\n';
        }
      }
      else {
        if(a == 0){
          cout << "BOTTOM TEXTURES: " << 1 - (stackText * (b+1)) << "| TOP TEXTURE: " << 1 - (stackText * (b)) << '\n';
        }
        addPoint(x, height, z);
        addNormal(x, height, z);
        addTextures(sliceText * a, 1 - (stackText * b));
        addPoint(xABChange, heightBChange, zABChange);
        addNormal(xABChange, heightBChange, zABChange);
        addTextures(sliceText * (a+1), 1 - (stackText * (b+1)));
        addPoint(xAChange, height, zAChange);
        addNormal(xAChange, height, zAChange);
        addTextures(sliceText * (a+1), 1 - (stackText * b));

        addPoint(x, height, z);
        addNormal(x, height, z);
        addTextures(sliceText * a, 1 - (stackText * b));
        addPoint(xBChange, heightBChange, zBChange);
        addNormal(xBChange, heightBChange, zBChange);
        addTextures(sliceText * a, 1 - (stackText * (b+1)));
        addPoint(xABChange, heightBChange, zABChange);
        addNormal(xABChange, heightBChange, zABChange);
        addTextures(sliceText * (a+1), 1 - (stackText * (b+1)));
      }
      beta1 -= stackStep;
      beta0 -= stackStep;
    }
    height = sin(beta0) * r;
    heightBChange = sin(beta1) * r;

    x = cos(beta0) * sin(alpha0) * r;
    xAChange = cos(beta0) * sin(alpha1) * r;
    xBChange = cos(beta1) * sin(alpha0) * r;
    xABChange = cos(beta1) * sin(alpha1) * r;

    z = cos(beta0) * cos(alpha0) * r;
    zAChange = cos(beta0) * cos(alpha1) * r;
    zBChange = cos(beta1) * cos(alpha0) * r;
    zABChange = cos(beta1) * cos(alpha1) * r;
    //FUNDO
    addPoint(0, -r, 0);
    addNormal(0, -r, 0);
    addTextures(sliceText * a + (sliceText/2), 0);
    addPoint(xAChange, height, zAChange);
    addNormal(xAChange, height, zAChange);
    addTextures(sliceText * (a+1), stackText);
    addPoint(x, height, z);
    addNormal(x, height, z);
    addTextures(sliceText * a, stackText);
    if(a == 0){
      cout << "BOTTOM TEXTURES: " << 0 << "| TOP TEXTURE: " << stackText << '\n';
    }
    alpha0 += sliceStep;
    alpha1 += sliceStep;
  }
}

void VerticesList::cone(float r, float maxHeight, int slices, int stacks){
  float stackStep = (float) maxHeight / stacks;
  float sliceStep = (float) (2 * M_PI) / slices;
  float stackText = 0.625 / stacks;
  float sliceText = 1.0 / slices;
  float ang(atan(r / maxHeight));
  float vY = cos(ang) * (sin(ang) * r);
  float vR = r - sin(ang) * (sin(ang) * r);
  float vX, vNextX, vZ, vNextZ;
  float currentR, nextR;
  float height, nextHeight, x, nextX, z, nextZ, xHigher, nextXHigher, zHigher, nextZHigher;

  for(int a = 0; a < slices; a++){
    for(int b = 0; b < stacks; b++){
      //GET COORDS
      height = stackStep * b;
      nextHeight = stackStep * (b + 1);

      //GET RADIUS
      currentR =  (r * (maxHeight - height)) / maxHeight;
      nextR =  (r * (maxHeight - nextHeight)) / maxHeight;

      //GET CORDS
      x = cos(a * sliceStep) * currentR;
      xHigher = cos(a * sliceStep) * nextR;
      nextX = cos((a + 1)* sliceStep) * currentR;
      nextXHigher = cos((a + 1)* sliceStep) * nextR;

      z = sin(a * sliceStep) * currentR;
      zHigher = sin(a * sliceStep) * nextR;
      nextZ = sin((a + 1)* sliceStep) * currentR;
      nextZHigher = sin((a + 1)* sliceStep) * nextR;

      //GET NORM
      vX = cos(a * sliceStep) * vR;
      vNextX = cos((a + 1) * sliceStep * vR);
      vZ = sin(a * sliceStep) * vR;
      vNextZ = sin((a + 1) * sliceStep * vR);

      if(b == 0){
        addPoint(0, 0, 0);
        addNormal(0, -1, 0);
        addTextures(0.8125, 0.1875);
        addPoint(x, 0 ,z);
        addNormal(0, -1, 0);
        addTextures(0.8125 + 0.1875 * cos(sliceStep*a), 0.1875 + 0.1875 * sin(sliceStep * a));
        addPoint(nextX, 0, nextZ);
        addNormal(0, -1, 0);
        addTextures(r * cos(sliceStep * (a + 1)), r * sin(sliceStep * (a + 1)));
      }
      addPoint(x, height, z);
      addNormal(vX, vY, vZ);
      addTextures(a * sliceText, 0.375 + stackText * b);
      addPoint(xHigher, nextHeight, zHigher);
      addNormal(vX, vY, vZ);
      addTextures(a*sliceText,0.375 + stackText*(b+1));
      addPoint(nextX, height, nextZ);
      addNormal(vNextX, vY, vNextZ);
      addTextures((a + 1) * sliceText, 0.375 + stackText * b);

      addPoint(nextXHigher, nextHeight, nextZHigher);
      addNormal(vNextX, vY, vNextZ);
      addTextures((a + 1) * sliceText, 0.375 + stackText * (b+1));
      addPoint(nextX, height, nextZ);
      addNormal(vNextX, vY, vNextZ);
      addTextures((a + 1) * sliceText, 0.375 + stackText * b);
      addPoint(xHigher, nextHeight, zHigher);
      addNormal(vX, vY, vZ);
      addTextures(a * sliceText, 0.375 + stackText * (b+1));
    }
  }
}
