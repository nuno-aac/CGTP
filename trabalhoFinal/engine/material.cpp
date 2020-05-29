#include "headers/material.h"

Material::Material() {
	diffR = diffG = diffB = 0;
	ambR = ambG = ambB = 0;
	specR = specG = specB = 0;
	shininess = 0;
}


float Material::getDiffR() {
	return diffR;
}

float Material::getDiffG() {
	return diffG;
}

float Material::getDiffB() {
	return diffB;
}

float Material::getAmbR() {
	return ambR;
}

float Material::getAmbG() {
	return ambG;
}

float Material::getAmbB() {
	return ambB;
}

float Material::getSpecR() {
	return specR;
}

float Material::getSpecG() {
	return specG;
}

float Material::getSpecB() {
	return specB;
}

int Material::getShininess() {
	return shininess;
}


void Material::setDiffR(float dr) {
	diffR = dr;
}

void Material::setDiffG(float dg) {
	diffG = dg;
}

void Material::setDiffB(float db) {
	diffB = db;
}

void Material::setAmbR(float ar) {
	ambR = ar;
}

void Material::setAmbG(float ag) {
	ambG = ag;
}

void Material::setAmbB(float ab) {
	ambB = ab;
}

void Material::setSpecR(float sr) {
	specR = sr;
}

void Material::setSpecG(float sg) {
	specG = sg;
}

void Material::setSpecB(float sb) {
	specB = sb;
}

void Material::setShininess(int s) {
	shininess = s;
}