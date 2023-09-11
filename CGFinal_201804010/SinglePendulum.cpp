#include "SinglePendulum.h"

SinglePendulum::SinglePendulum(double x, double y, double z, double length) {
	l = length;
	p0.Set(x, y, z);
	p1.Set(x + length, y, z);
	vel.Clear();
	cosTest.Clear();

	graDraw = false;
	accDraw = false;
	velDraw = false;

	i = 0;
	tf = false;
}

void SinglePendulum::testCos(double dt) {
	cosTest += cos(i * 3.141592 / 180) * 0.1;
	cout << i << endl;
	if (i == 360)
		tf = true;
	else if (i == 0)
		tf = false;

	if (!tf)
		i++;
	else
		i--;
}

void SinglePendulum::setPendulum(double dt) {
	Vec3<double> gravity;
	Vec3<double> acc;
	gravity.Set(0.0, -9.8, 0.0);
	acc += gravity;
	drawAccGra = acc * 0.7;
	//acc.Print();

	Vec3<double> dir;
	dir = p1 - p0;
	double theta = dir.Dot(acc);
	theta /= (acc.Length() * dir.Length());		// cos(theta);
	double angle = acos(theta);
	//cout << angle * 180 / 3.141592 << endl;
	
	dir = p1 - p0;
	dir.Normalize();
	acc -= (dir * (theta * acc.Length()));
	drawAccCos = acc * 0.7;

	vel += acc * dt;
	//vel.Print();
	p1 += vel * dt;

	dir = p1 - p0;
	dir.Normalize();
	p1 = p0 + (dir * l);

	double v = vel.Length();
	vel = vel.Cross(dir);
	vel = dir.Cross(vel);
	vel *= v / vel.Length();
	drawVel = vel * 0.7;
}

void SinglePendulum::draw() {
	glDisable(GL_LIGHTING);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(p0.x(), p0.y(), p0.z());
	glVertex3f(p1.x(), p1.y(), p1.z());
	glEnd();
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		double angle = i * 3.141592 / 180;
		double x = 0.5 * cos(angle) + p1.x();
		double y = 0.5 * sin(angle) + p1.y();
		glVertex3f(x, y, p1.z());
	}
	glEnd();

	//glBegin(GL_LINES);
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(p0.x(), p0.y(), p0.z());
	//glVertex3f(drawtmp.x(), drawtmp.y(), drawtmp.z());
	//glEnd();

	if (graDraw) {
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(p1.x(), p1.y(), p1.z());
		glVertex3f(p1.x(), drawAccGra.y(), p1.z());
		glEnd();
	}

	if (accDraw) {
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(p1.x(), p1.y(), p1.z());
		glVertex3f(drawAccCos.x() + p1.x(), drawAccCos.y() + p1.y(), drawAccCos.z() + p1.z());
		glEnd();
	}

	if (velDraw) {
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(p1.x(), p1.y(), p1.z());
		glVertex3f(drawVel.x() + p1.x(), drawVel.y() + p1.y(), drawVel.z() + p1.z());
		glEnd();
	}

	//glBegin(GL_LINES);
	//glColor3f(1.0f, 1.0f, 0.0f);
	//glVertex3f(p1.x(), p1.y(), p1.z());
	//glVertex3f(drawDir.x(), drawDir.y(), drawDir.z());
	//glEnd();

	//glPointSize(10.0f);
	//glBegin(GL_POINTS);
	//glVertex3f(cosTest.x(), 0, 0);
	//glEnd();

	glEnable(GL_LIGHTING);
}
