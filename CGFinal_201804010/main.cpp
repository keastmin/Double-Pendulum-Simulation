#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GL\glut.h"
#include "Vec3.h"
#include "doublePendulum.h"
#include "SinglePendulum.h"

//#define SOLO_DOUBLE_PENDULUM
//#define THREE_DOUBLE_PENDULUM
//#define SOLO_SINGLE_PENDULUM
#define FOUR_SINGLE_PENDULUM

bool _simulation = false;
float _zoom = 15.0f; // 화면 확대,축소
float _rot_x = 0.0f; // x축 회전
float _rot_y = 0.0f; // y축 회전
float _trans_x = 0.0f; // x축 이동
float _trans_y = 0.0f; // y축 이동
int _last_x = 0; // 이전 마우스 클릭 x위치
int _last_y = 0; // 이전 마우스 클릭 y위치
unsigned char _buttons[3] = { 0 }; // 마우스 상태(왼쪽,오른쪽,휠 버튼)
unsigned char _button[2] = { 0 };	// 추를 잡았는지 확인할 상태

// 각도 변화량에 영향끼칠 랜덤값
double rvalue1 = 0;
double rvalue2 = 0;
double rvalue3 = 0;


// 이중진자 1개 선언
#ifdef SOLO_DOUBLE_PENDULUM
doublePendulum* _double1;
#endif

// 이중진자 3개 선언
#ifdef THREE_DOUBLE_PENDULUM
doublePendulum* _double1;
doublePendulum* _double2;
doublePendulum* _double3;
#endif

// 일중진자 선언
#ifdef SOLO_SINGLE_PENDULUM
SinglePendulum* _single1;
#endif

// 일중진자 4개 선언
#ifdef FOUR_SINGLE_PENDULUM
SinglePendulum* _single1;
SinglePendulum* _single2;
SinglePendulum* _single3;
SinglePendulum* _single4;
#endif

void Init(void)
{
	// 깊이값 사용 여부
	glEnable(GL_DEPTH_TEST);

	// 중복되는 수 없이 1.6~2 사이의 값으로 각도를 결정하는 수식에 관여
#ifdef THREE_DOUBLE_PENDULUM
	srand(time(NULL));
	while (rvalue1 == rvalue2 || rvalue1 == rvalue3 || rvalue3 == rvalue2) {
		rvalue1 = (rand() % 5) + 16.0;
		rvalue2 = (rand() % 5) + 16.0;
		rvalue3 = (rand() % 5) + 16.0;
		rvalue1 /= 10.0;
		rvalue2 /= 10.0;
		rvalue3 /= 10.0;
	}
#endif

	// 이중진자 1개 선언
#ifdef SOLO_DOUBLE_PENDULUM
	_double1 = new doublePendulum(0.0, 0.0, 0.0, 3, 1, 1);
#endif

	// 이중진자 3개 선언
#ifdef THREE_DOUBLE_PENDULUM
	_double1 = new doublePendulum(0.0, 0.0, 0.0, 3, 1, 1);
	_double2 = new doublePendulum(0.0, 0.0, 0.0, 3, 1, 1);
	_double3 = new doublePendulum(0.0, 0.0, 0.0, 3, 1, 1);
#endif

	// 일중진자 1개 선언
#ifdef SOLO_SINGLE_PENDULUM
	_single1 = new SinglePendulum(0.0, 0.0, 0.0, 4);
#endif

	// 일중진자 4개 선언
#ifdef FOUR_SINGLE_PENDULUM
	_single1 = new SinglePendulum(0.0, 0.0, 0.0, 2);
	_single2 = new SinglePendulum(0.0, 0.0, -1.0, 3);
	_single3 = new SinglePendulum(0.0, 0.0, -2.0, 4);
	_single4 = new SinglePendulum(0.0, 0.0, -3.0, 5);
#endif
}

void Draw(void)
{
	glEnable(GL_LIGHTING); // 조명 활성화
	glEnable(GL_LIGHT0); // 첫번째 조명

	// 이중진자 1개 그리기
#ifdef SOLO_DOUBLE_PENDULUM
	_double1->draw();
#endif

	// 이중진자 3개 그리기
#ifdef THREE_DOUBLE_PENDULUM
	_double1->draw();
	_double2->draw();
	_double3->draw();
#endif

	// 일중진자 1개 그리기
#ifdef SOLO_SINGLE_PENDULUM
	_single1->draw();
#endif

	// 일중진자 4개 그리기
#ifdef FOUR_SINGLE_PENDULUM
	_single1->draw();
	_single2->draw();
	_single3->draw();
	_single4->draw();
#endif

	glDisable(GL_LIGHTING);
}

void Display(void)
{
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0, 0, -_zoom);
	glTranslatef(_trans_x, _trans_y, 0);
	glRotatef(_rot_x, 1, 0, 0);
	glRotatef(_rot_y, 0, 1, 0);

	Draw();
	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	if (w == 0) {
		h = 1;
	}
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / h, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'Q':
	case 'q':
		exit(0);
	case ' ':
		_simulation = !_simulation;
		break;
	case 'r':
	case 'R':
		_zoom = 15.0f; 
		_rot_x = 0.0f; 
		_rot_y = 0.0f;
		_trans_x = 0.0f; 
		_trans_y = 0.0f; 
		break;
	case 'd':
	case 'D':
		// 2중진자의 추 끝을 따라 그려지는 선 활성화
#ifdef SOLO_DOUBLE_PENDULUM
		_double1->drawLine = !_double1->drawLine;
		_double1->drawX.clear();
		_double1->drawY.clear();
#endif
#ifdef THREE_DOUBLE_PENDULUM
		_double1->drawLine = !_double1->drawLine;
		_double1->drawX.clear();
		_double1->drawY.clear();

		_double2->drawLine = !_double2->drawLine;
		_double2->drawX.clear();
		_double2->drawY.clear();

		_double3->drawLine = !_double3->drawLine;
		_double3->drawX.clear();
		_double3->drawY.clear();
#endif
		break;
	case 'a':
	case 'A':
#ifdef SOLO_SINGLE_PENDULUM
		_single1->accDraw = !_single1->accDraw;		
#endif
		break;
	case 'v':
	case 'V':
#ifdef SOLO_SINGLE_PENDULUM
		_single1->velDraw = !_single1->velDraw;		
#endif
		break;
	case 'g':
	case 'G':
#ifdef SOLO_SINGLE_PENDULUM
		_single1->graDraw = !_single1->graDraw;
#endif
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	_last_x = x;
	_last_y = y;

	// 현재 보여지는 화면과 추의 위치 링크
	int xtmp = (x - 400) / 60;
	int ytmp = -(y - 400) / 60;
	

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		//cout << "x = " << xtmp << "y = " << ytmp << endl;
		if (!_simulation) {
#ifdef SOLO_DOUBLE_PENDULUM
			if (xtmp > _double1->p1.x() - 1 && xtmp < _double1->p1.x() + 1 && ytmp > _double1->p1.y() - 1 && ytmp < _double1->p1.y() + 1) {
				_button[0] = state == GLUT_DOWN ? 1 : 0;	// 1번째 추 클릭
			}
			else if (xtmp > _double1->p2.x() - 1 && xtmp < _double1->p2.x() + 1 && ytmp > _double1->p2.y() - 1 && ytmp < _double1->p2.y() + 1) {
				_button[1] = state == GLUT_DOWN ? 1 : 0;	// 2번째 추 클릭
			}
			else {
				_buttons[0] = state == GLUT_DOWN ? 1 : 0;	// 그 외의 화면 클릭
			}
#endif

#ifdef THREE_DOUBLE_PENDULUM
			_buttons[0] = state == GLUT_DOWN ? 1 : 0;
#endif

#ifdef SOLO_SINGLE_PENDULUM
			if (xtmp > _single1->p1.x() - 1 && xtmp < _single1->p1.x() + 1 && ytmp > _single1->p1.y() - 1 && ytmp < _single1->p1.y() + 1) {
				_button[0] = state == GLUT_DOWN ? 1 : 0;	// 1번째 추 클릭
			}
			else {
				_buttons[0] = state == GLUT_DOWN ? 1 : 0;	// 그 외의 화면 클릭
			}
#endif

#ifdef FOUR_SINGLE_PENDULUM
			_buttons[0] = state == GLUT_DOWN ? 1 : 0;
#endif
		}
		else
			_buttons[0] = state == GLUT_DOWN ? 1 : 0;
		break;
	case GLUT_MIDDLE_BUTTON:
		_buttons[1] = state == GLUT_DOWN ? 1 : 0;
		break;
	case GLUT_RIGHT_BUTTON:
		_buttons[2] = state == GLUT_DOWN ? 1 : 0;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void Motion(int x, int y)
{
	int diff_x = x - _last_x;
	int diff_y = y - _last_y;
	_last_x = x;
	_last_y = y;

	// 현재 보여지는 화면과 추의 위치 링크
	double xtmp = ((double)x - 400.0) / 60.0;
	double ytmp = -((double)y - 400.0) / 60.0;
	

	if (_buttons[2]) {
		_zoom -= (float)0.02f * diff_x;
	}
	else if (_buttons[1]) {
		_trans_x += (float)0.02f * diff_x;
		_trans_y -= (float)0.02f * diff_y;
	}
	else if (_buttons[0]) {
		_rot_x += (float)0.2f * diff_y;
		_rot_y += (float)0.2f * diff_x;
	}
	else if (_button[0]) {	// _double1 이중진자의 1번째 추 마우스
#ifdef SOLO_DOUBLE_PENDULUM
		// 마우스 커서 위치 조정 및 길이 원래값 유지
		_double1->p1.SetX(xtmp);
		_double1->p1.SetY(ytmp);

		Vec3<double> dir1;
		Vec3<double> dir2;

		dir1 = _double1->p1 - _double1->p0;
		dir1.Normalize();
		_double1->p1 = _double1->p0 + (dir1 * _double1->l);

		dir2 = _double1->p2 - _double1->p1;
		dir2.Normalize();
		_double1->p2 = _double1->p1 + (dir2 * _double1->l);

		_double1->p2.SetX(_double1->p2.x());
		_double1->p2.SetY(_double1->p2.y());

		// p0과 p1사이의 각도
		Vec3<double> p0p1;
		Vec3<double> p0re;
		p0p1 = _double1->p1 - _double1->p0;
		p0re = _double1->referVec - _double1->p0;
		double theta1 = p0p1.Dot(p0re);
		theta1 /= (p0p1.Length() * p0re.Length());
		theta1 = acos(theta1);
		if (_double1->p1.x() <= _double1->p0.x()) {
			p0p1 = _double1->p1 - _double1->p0;
			p0re = _double1->referVec2 - _double1->p0;
			theta1 = p0p1.Dot(p0re);
			theta1 /= (p0p1.Length() * p0re.Length());
			theta1 = 3.141592 + acos(theta1);
		}
		_double1->theta1 = theta1;

		// p1과 p2사이의 각도
		Vec3<double> p1p2;
		Vec3<double> p1re;
		_double1->p1referVec.Set(_double1->p1.x(), _double1->p1.y() + 5, _double1->p1.z());
		_double1->p1referVec2.Set(_double1->p1.x(), _double1->p1.y() - 5, _double1->p1.z());
		p1p2 = _double1->p2 - _double1->p1;
		p1re = _double1->p1referVec - _double1->p1;
		double theta2 = p1p2.Dot(p1re);
		theta2 /= (p1p2.Length() * p1re.Length());
		theta2 = acos(theta2);
		if (_double1->p2.x() <= _double1->p1.x()) {
			p1p2 = _double1->p2 - _double1->p1;
			p1re = _double1->p1referVec2 - _double1->p1;
			theta2 = p1p2.Dot(p1re);
			theta2 /= (p1p2.Length() * p1re.Length());
			theta2 = 3.141592 + acos(theta2);
		}
		_double1->theta2 = theta2;
		//cout << "_double1->theta2 = " << _double1->theta2 << endl;

		// 각도 축적 초기화
		_double1->vel1 = 0;
		_double1->vel2 = 0;

		// 추의 움직임을 나타내는 선 초기화
		_double1->drawX.clear();
		_double1->drawY.clear();
#endif

#ifdef SOLO_SINGLE_PENDULUM
		// 마우스로 위치 조정
		_single1->p1.SetX(xtmp);
		_single1->p1.SetY(ytmp);
		Vec3<double> dir;
		dir = _single1->p1 - _single1->p0;
		dir.Normalize();
		_single1->p1 = _single1->p0 + (dir * _single1->l);
		
		// 속도 초기화
		_single1->vel.Clear();
#endif
	}
	else if (_button[1]) {	// _double1 이중진자의 2번째 추 마우스
#ifdef SOLO_DOUBLE_PENDULUM
		// 마우스 커서 위치 조정 및 길이 원래값 유지
		_double1->p2.SetX(xtmp);
		_double1->p2.SetY(ytmp);
		Vec3<double> dir;
		dir = _double1->p2 - _double1->p1;
		dir.Normalize();
		_double1->p2 = _double1->p1 + (dir * _double1->l);

		// p1과 p2 사이의 각도 조절
		Vec3<double> p1p2;
		Vec3<double> p1re;
		p1p2 = _double1->p2 - _double1->p1;
		p1re = _double1->p1referVec - _double1->p1;
		double theta2 = p1p2.Dot(p1re);
		theta2 /= (p1p2.Length() * p1re.Length());
		theta2 = acos(theta2);
		if (_double1->p2.x() <= _double1->p1.x()) {
			p1p2 = _double1->p2 - _double1->p1;
			p1re = _double1->p1referVec2 - _double1->p1;
			theta2 = p1p2.Dot(p1re);
			theta2 /= (p1p2.Length() * p1re.Length());
			theta2 = 3.141592 + acos(theta2);
		}
		_double1->theta2 = theta2;

		// 각도 축적 초기화
		_double1->vel1 = 0;
		_double1->vel2 = 0;

		// 추의 움직임을 나타내는 선 초기화
		_double1->drawX.clear();
		_double1->drawY.clear();
#endif
	}
	glutPostRedisplay();
}

void Update(void)
{
	if (_simulation) {
		// 추의 개수 조절

		//이중진자 1개 시뮬레이션
#ifdef SOLO_DOUBLE_PENDULUM
		_double1->pendulumSet(0.006, 2);
#endif

		// 이중진자 3개 시뮬레이션
#ifdef THREE_DOUBLE_PENDULUM
		_double1->pendulumSet(0.006, rvalue1);
		_double2->pendulumSet(0.006, rvalue2);
		_double3->pendulumSet(0.006, rvalue3);
#endif

		// 일중진자 1개 시뮬레이션
#ifdef SOLO_SINGLE_PENDULUM
		_single1->setPendulum(0.01);
#endif

		// 일중진자 4개 시뮬레이션
#ifdef FOUR_SINGLE_PENDULUM
		_single1->setPendulum(0.01);
		_single2->setPendulum(0.01);
		_single3->setPendulum(0.01);
		_single4->setPendulum(0.01);
#endif
	}
	::glutPostRedisplay();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pendulum");
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	glutIdleFunc(Update);
	Init();
	glutMainLoop();
}