#include "doublePendulum.h"

doublePendulum::doublePendulum(double x, double y, double z, double length, double mass1, double mass2) {
	l = length;
	m1 = mass1;
    m2 = mass2;
	g = -9.8;
    vel1 = 0;
    vel2 = 0;
    theta1 = (3.141562 / 2);
    theta2 = (3.141562 / 2);
    xPos = x;
    yPos = y;
    zPos = z;
	p0.Set(x, y, z);
	p1.Set(x + l, y, z);
	p2.Set(x + l + l, y, z);
    referVec.Set(0.0, 5.0, 0.0);
    referVec2.Set(0.0, -5.0, 0.0);

    p1referVec.Set(p1.x(), p1.y() + 5, p1.z());
    p1referVec2.Set(p1.x(), p1.y() - 5, p1.z());

    drawLine = true;

    //drawX.resize(100);
    //drawY.resize(100);
}

void doublePendulum::pendulumSet(double dt, double rvalue) {
    // 시간에 따른 다음 각도를 결정할 수식
    double nA1 = ((-g * (rvalue * m1 + m2) *
        sin(theta1)) + (-m2 * g * sin(theta1 - 2 * theta2)) + 
        (-2 * sin(theta1 - theta2) * m2) * (pow(vel2, 2) * l + pow(vel1, 2) * l * cos(theta1 - theta2)))
        / (l * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2)));

    double nA2 = ((2 * sin(theta1 - theta2)) * 
            (((pow(vel1, 2) * l * (m1 + m2)) + (g * (m1 + m2) * 
                cos(theta1)) + (pow(vel2, 2) * l * m2 * cos(theta1 - theta2))))
            / (l * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2))));

    //cout << "sys calc angle2 = " << theta2 * 180 / 3.141592 << endl;

    double x1 = l * sin(theta1);
    double y1 = l * cos(theta1);

    double x2 = x1 + l * sin(theta2);
    double y2 = y1 + l * cos(theta2);

    // 포지션 적용
    p1.Set(x1 + xPos, y1 + yPos, zPos);
    p2.Set(x2 + xPos, y2 + yPos, zPos);

    // 각속도 계산
    vel1 += nA1 * dt * 0.99;
    vel2 += nA2 * dt * 0.99;

    // 다음 각도 계산을 위한 각도 더하기
    theta1 += vel1 * dt;
    theta2 += vel2 * dt;

    p1referVec.Set(p1.x(), p1.y() + 5, p1.z());
    p1referVec2.Set(p1.x(), p1.y() - 5, p1.z());


    // 추 간의 각도 디버깅
    Vec3<double> dir;
    Vec3<double> rdir;
    dir = p1 - p0;
    rdir = referVec - p0;
    double theta;
    theta = dir.Dot(rdir);
    theta /= (dir.Length() * rdir.Length());
    //cout << "my calc theta = " << theta << endl;
    double angle = acos(theta);
    if (p1.x() <= p0.x()) {
        Vec3<double> dir2;
        Vec3<double> rdir2;
        rdir2 = referVec2 - p0;
        dir2 = p1 - p0;
        theta = dir2.Dot(rdir2);
        theta /= (dir2.Length() * rdir2.Length());
        angle = 3.141592 + acos(theta);
    }
    //cout << "my calc theta = " << angle << endl << endl;   
    //cout << "my calc angle1 = " << angle * 180 / 3.141592 << endl;


    dir = p2 - p1;

    rdir = p1referVec - p1;
    theta = dir.Dot(rdir);
    theta /= (dir.Length() * rdir.Length());
    angle = acos(theta);
    if (p2.x() <= p1.x()) {
        Vec3<double> dir2;
        Vec3<double> rdir2;       
        dir2 = p2 - p1;
        rdir2 = p1referVec2 - p1;
        theta = dir2.Dot(rdir2);
        theta /= (dir2.Length() * rdir2.Length());
        angle = 3.141592 + acos(theta);
    }
    //cout << "my calc theta2 = " << angle << endl << endl;
    //cout << "my calc angle2 = " << angle * 180 / 3.141592 << endl;


    if (drawLine) {
        drawX.push_back(p2.x());
        drawY.push_back(p2.y());
    }
}

void doublePendulum::draw() {
    glDisable(GL_LIGHTING);



    // p0과 p1 사이의 라인
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(p0.x(), p0.y(), p0.z());
    glVertex3f(p1.x(), p1.y(), p1.z());
    glEnd();

    // p1의 추
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        double angle = i * 3.141592 / 180;
        double x = 0.2 * cos(angle) + p1.x();
        double y = 0.2 * sin(angle) + p1.y();
        glVertex3f(x, y, zPos);
    }
    glEnd();

    // p1과 p2사이의 라인
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(p1.x(), p1.y(), p1.z());
    glVertex3f(p2.x(), p2.y(), p2.z());
    glEnd();

    // p2의 추
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        double angle = i * 3.141592 / 180;
        double x = 0.2 * cos(angle) + p2.x();
        double y = 0.2 * sin(angle) + p2.y();
        glVertex3f(x, y, zPos);
    }
    glEnd();

    // p2가 지나갔던 위치
    if (drawLine) {
        glBegin(GL_LINES);
        glColor3f(0.0, 0.0, 0.0);
        if (drawX.size() > 2) {
            for (int i = 0; i < drawX.size() - 2; i++) {
                glVertex3f(drawX[i], drawY[i], 0.0f);
                glVertex3f(drawX[i + 1], drawY[i + 1], 0.0f);
            }
        }
        glEnd();
    }


    // 기준점 디버깅
    //glLineWidth(1.0f);
    //glBegin(GL_LINES);
    //glColor3f(1.0f, 0.0f, 1.0f);
    //glVertex3f(p1.x(), p1.y(), p1.z());
    //glVertex3f(p1referVec.x(), p1referVec.y(), p1referVec.z());
    //glEnd();
    //glLineWidth(1.0f);
    //glBegin(GL_LINES);
    //glColor3f(1.0f, 0.0f, .0f);
    //glVertex3f(p1.x(), p1.y(), p1.z());
    //glVertex3f(p1referVec2.x(), p1referVec2.y(), p1referVec2.z());
    //glEnd();
}