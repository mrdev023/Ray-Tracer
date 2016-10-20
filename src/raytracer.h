#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

struct vecteur {
	float x, y, z;
};

struct point {
	float x, y, z;
};

struct ray {
	point start;
	vecteur dir;
};

struct material {
	float red, green, blue, reflection;
};

struct sphere {
	point pos;
	float size;
	int material;
};

struct light {
	point pos;
	float red, green, blue;
};

struct scene {
	std::vector<material> matTab;
	std::vector<sphere>   sphTab;
	std::vector<light>    lgtTab;
	int sizex, sizey;
};

std::istream & operator >> ( std::istream &inputFile,  point& p ) {
	return inputFile >> p.x >> p.y >> p.z ; 
}

std::istream & operator >> ( std::istream &inputFile,  vecteur& v ) {
	return inputFile >> v.x >> v.y >> v.z ; 
}

point operator + (const point&p, const vecteur &v){
	point p2={p.x + v.x, p.y + v.y, p.z + v.z };
	return p2;
}

point operator - (const point&p, const vecteur &v){
	point p2={p.x - v.x, p.y - v.y, p.z - v.z };
	return p2;
}

vecteur operator - (const point&p1, const point &p2){
	vecteur v={p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
	return v;
}

vecteur operator * (float c, const vecteur &v)
{
	vecteur v2={v.x *c, v.y * c, v.z * c };
	return v2;
}

vecteur operator - (const vecteur&v1, const vecteur &v2){
	vecteur v={v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return v;
}

float operator * (const vecteur&v1, const vecteur &v2 ) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

std::istream & operator >> ( std::istream &inputFile, material& mat ) {
	return inputFile >> mat.red >> mat.green >> mat.blue >> mat.reflection; 
}

std::istream & operator >> ( std::istream &inputFile, sphere& sph ) {
	return inputFile >> sph.pos >> sph.size >> sph.material;
}

std::istream & operator >> ( std::istream &inputFile, light& lig ) {
	return inputFile >> lig.pos >> lig.red >> lig.green >> lig.blue;
}



#endif