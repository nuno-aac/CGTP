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

void VerticesList::setPoints(vector<float> p){
  points = p;
}

void VerticesList::addPoint(float x, float y, float z){
  points.push_back(x);
  points.push_back(y);
  points.push_back(z);
}

void VerticesList::plane(float size){
  float cord = size/2;
  //T1
    addPoint(cord, 0, cord);
    addPoint(cord,0,-cord);
    addPoint(-cord, 0, cord);
  //T2
    addPoint(-cord, 0, -cord);
    addPoint(-cord, 0, cord);
    addPoint(cord, 0, -cord);
}

void VerticesList::box(float x, float y, float z){
  float cordX = x/2;
  float cordY = y/2;
  float cordZ = z/2;

  //Topo
    //T1
    addPoint(cordX, cordY, cordZ);
    addPoint(cordX,cordY,-cordZ);
    addPoint(-cordX, cordY, cordZ);
    //T2
    addPoint(-cordX, cordY, -cordZ);
    addPoint(-cordX, cordY, cordZ);
    addPoint(cordX, cordY, -cordZ);

  //Base
    //T1
    addPoint(cordX, -cordY, cordZ);
    addPoint(-cordX, -cordY, cordZ);
    addPoint(cordX,-cordY,-cordZ);
    //T2
    addPoint(-cordX, -cordY, -cordZ);
    addPoint(cordX, -cordY, -cordZ);
    addPoint(-cordX, -cordY, cordZ);

  //Lado X positivo
    //T1
    addPoint(cordX, cordY, cordZ);
    addPoint(cordX, -cordY, cordZ);
    addPoint(cordX,cordY,-cordZ);
    //T2
    addPoint(cordX, -cordY, -cordZ);
    addPoint(cordX, cordY, -cordZ);
    addPoint(cordX, -cordY, cordZ);

  //Lado X negativo
    //T1
    addPoint(-cordX, cordY, cordZ);
    addPoint(-cordX,cordY,-cordZ);
    addPoint(-cordX, -cordY, cordZ);
    //T2
    addPoint(-cordX, -cordY, -cordZ);
    addPoint(-cordX, -cordY, cordZ);
    addPoint(-cordX, cordY, -cordZ);

  //Lado Z positivo
    //T1
    addPoint(cordX, cordY, cordZ);
    addPoint(-cordX, cordY, cordZ);
    addPoint(cordX, -cordY, cordZ);
    //T2
    addPoint(-cordX, -cordY, cordZ);
    addPoint(cordX, -cordY, cordZ);
    addPoint(-cordX, cordY, cordZ);

  //Lado Z negativo
    //T1
    addPoint(cordX, cordY, -cordZ);
    addPoint(cordX,-cordY, -cordZ);
    addPoint(-cordX, cordY, -cordZ);
    //T2
    addPoint(-cordX, -cordY, -cordZ);
    addPoint(-cordX, cordY, -cordZ);
    addPoint(cordX, -cordY, -cordZ);
}

void VerticesList::sphere(float r, int slices, int stacks){
  float stackStep = (float) (M_PI) / stacks;
  float sliceStep = (float) (2 * M_PI) / slices;
  float alpha0, beta0, alpha1, beta1;
  float height, heightBChange, x, xAChange, xBChange, xABChange, z, zAChange, zBChange, zABChange;

  alpha0 = 0;
  alpha1 = sliceStep;

  for(int a = 0; a < slices; a++){

    beta0 = (M_PI/2) * 1;
    beta1 = beta0 + stackStep;

    for(int b = 0; b < stacks; b++){

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

      if(b  == 0){
        //TOPO
        addPoint(0, r, 0);
        addPoint(xBChange , heightBChange, zBChange);
        addPoint(xABChange , heightBChange, zABChange);
      }
      else{
        //CORPO
        addPoint(x, height, z);
        addPoint(xAChange, height, zAChange);
        addPoint(xABChange, heightBChange, zABChange);

        addPoint(x, height, z);
        addPoint(xABChange, heightBChange, zABChange);
        addPoint(xBChange, heightBChange, zBChange);
      }
      //FUNDO
      addPoint(0, -r, 0);
      addPoint(xAChange , height, zAChange);
      addPoint(x , height, z);
      beta1 -= stackStep;
      beta0 -= stackStep;
    }
    alpha0 += sliceStep;
    alpha1 += sliceStep;
  }
}

void VerticesList::cone(float r, float maxHeight, int slices, int stacks){
  float stackStep = (float) maxHeight / stacks;
  float sliceStep = (float) (2 * M_PI) / slices;
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

      if(b == 0){
        addPoint(0, 0, 0);
        addPoint(x, 0 ,z);
        addPoint(nextX, 0, nextZ);
      }
      addPoint(x, height, z);
      addPoint(xHigher, nextHeight, zHigher);
      addPoint(nextX, height, nextZ);

      addPoint(nextXHigher, nextHeight, nextZHigher);
      addPoint(nextX, height, nextZ);
      addPoint(xHigher, nextHeight, zHigher);
    }
  }
}
