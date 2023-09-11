#pragma once
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Vec3.h"
#include "GL/glut.h"
using namespace std;

class doublePendulum
{
public:
	double l;
	double m1;
	double m2;
	double g;
	double vel1;
	double vel2;
	double theta1;
	double theta2;

	double xPos;
	double yPos;
	double zPos;

	bool drawLine;
public:
	Vec3<double> p0;
	Vec3<double> p1;
	Vec3<double> p2;
	Vec3<double> referVec;
	Vec3<double> referVec2;
	Vec3<double> p1referVec;
	Vec3<double> p1referVec2;

	vector<double> drawX;
	vector<double> drawY;
public:
	doublePendulum(double x, double y, double z, double length, double mass1, double mass2);
public:
	void pendulumSet(double dt, double rvalue);
	void draw();
};

