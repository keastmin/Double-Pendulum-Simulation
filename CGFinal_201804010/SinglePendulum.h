#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include "Vec3.h"
#include "GL/glut.h"

using namespace std;

class SinglePendulum
{
public:
	double l;
	Vec3<double> p0;
	Vec3<double> p1;
	Vec3<double> vel;

	Vec3<double> drawAccGra;
	Vec3<double> drawAccCos;
	Vec3<double> drawVel;
	Vec3<double> drawDir;
	Vec3<double> drawtmp;

	Vec3<double> cosTest;
	int i;
	bool tf;

	bool accDraw;
	bool velDraw;
	bool graDraw;
public:
	SinglePendulum(double x, double y, double z, double length);
public:
	void testCos(double dt);
	void setPendulum(double dt);
public:
	void draw();
};
