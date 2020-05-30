#include "headers/material.h"

Material::Material() {
	for (int i = 0; i < 3; i++) {
		diff[i] = amb[i] = spec[i] = 0;
	}
	diff[3] = amb[3] = spec[3] = 1;
	shininess = 0;
}

float* Material::getDiff() {
	return diff;
}

float Material::getDiffR() {
	return diff[0];
}

float Material::getDiffG() {
	return diff[1];
}

float Material::getDiffB() {
	return diff[2];
}

float* Material::getAmb() {
	return amb;
}

float Material::getAmbR() {
	return amb[0];
}

float Material::getAmbG() {
	return amb[1];
}

float Material::getAmbB() {
	return amb[2];
}

float* Material::getSpec() {
	return spec;
}

float Material::getSpecR() {
	return spec[0];
}

float Material::getSpecG() {
	return spec[1];
}

float Material::getSpecB() {
	return spec[2];
}

int Material::getShininess() {
	return shininess;
}


void Material::setDiffR(float dr) {
	diff[0] = dr;
}

void Material::setDiffG(float dg) {
	diff[1]= dg;
}

void Material::setDiffB(float db) {
	diff[2] = db;
}

void Material::setAmbR(float ar) {
	amb[0] = ar;
}

void Material::setAmbG(float ag) {
	amb[1]= ag;
}

void Material::setAmbB(float ab) {
	amb[2] = ab;
}

void Material::setSpecR(float sr) {
	spec[0] = sr;
}

void Material::setSpecG(float sg) {
	spec[1]= sg;
}

void Material::setSpecB(float sb) {
	spec[2] = sb;
}

void Material::setShininess(int s) {
	shininess = s;
}