#include <iostream>
using namespace std;
#include <GL/glut.h>
#include <math.h>

// Stuct untuk menyimpan data titik, vektor dan matriks
typedef struct {
	float x;
	float y;
	float z;
} Point3D_t;
typedef struct {
	float m[3][3];
} Matrix3D_t;
typedef struct {
	float v[3];
} Vector3D_t;
typedef struct {
	float r;
	float g;
	float b;
} Color_t;

// Variabel global objek
void drawLine(Point3D_t[], int, Color_t);
void timer(int);
void display(void);
void initialize(void);
void drawPolygon(Point3D_t[], int, Color_t);
void drawFillPolygon(Point3D_t[], int, Color_t);
void makeCircle(Point3D_t, int, bool, Color_t);
Matrix3D_t rotationZ(float);
Matrix3D_t translation(int, int);
Matrix3D_t scalingZ(float);
Matrix3D_t createIdentity();
Vector3D_t point2Vector3d(Point3D_t point);
Point3D_t vector2Point3d(Vector3D_t vector);
Vector3D_t operator*(Matrix3D_t, Vector3D_t);
Vector3D_t operator+(Matrix3D_t, Vector3D_t);
Matrix3D_t operator * (Matrix3D_t, Matrix3D_t);

void bola_pantul();
void animasi_pacman();
void drawPacman(Point3D_t center, int radius, Color_t color, bool
	open, int right);
Point3D_t putar_titik_pada_roda(Point3D_t, bool, int, int);

// Pre Draw Function
void drawPolygon(Point3D_t pnt[], int n, Color_t color) {
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for (size_t i = 0; i < n; i++)
	{
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}
void drawFillPolygon(Point3D_t pnt[], int n, Color_t color) {
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POLYGON);
	for (size_t i = 0; i < n; i++)
	{
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}
void makeCircle(Point3D_t center, int radius, bool filled,
	Color_t color) {
	Point3D_t circle[360];
	for (size_t i = 0; i < 360; i++)
	{
		circle[i].x = (float)center.x + (radius * cos(i * 3.14 / 180));
		circle[i].y = (float)center.y + (radius * sin(i * 3.14 / 180));
	}
	if (filled) {
		drawFillPolygon(circle, 360, color);
	}
	else {
		drawPolygon(circle, 360, color);
	}
}
void drawPacman(Point3D_t center, int radius, Color_t color, bool
	open, int right) {
	Point3D_t circle[360];
	Point3D_t eye = {
	center.x,
	center.y + 20,
	0
	};
	for (size_t i = 0; i < 360; i++)
	{
		circle[i].x = (float)center.x + (right * radius *
			cos(i * 3.14 / 180));
		circle[i].y = (float)center.y + (radius * sin(i * 3.14 / -180));
	}
	int angle = 0;
	if (open) {
		circle[315].x = center.x;
		circle[315].y = center.y;
		angle = 316;
	}
	else {
		angle = 360;
	}
	makeCircle(eye, 5, true, { 0,0,1 });
	drawPolygon(circle, angle, color);
}


// Function untuk membuat matiks identitas (diagonal utama 1)
Matrix3D_t createIdentity() {
	Matrix3D_t a;
	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 3; j++)
			a.m[i][j] = 0;
		a.m[i][i] = 1;
	}
	return a;
}
Matrix3D_t rotationZ(float theta) {
	Matrix3D_t rotate = createIdentity();
	float cs = cos(theta / 57.3);
	float sn = sin(theta / 57.3);
	rotate.m[0][0] = cs; rotate.m[0][1] = -sn;
	rotate.m[1][0] = sn; rotate.m[1][1] = cs;
	return rotate;
}
Matrix3D_t translation(int dx, int dy) {
	Matrix3D_t trans = createIdentity();
	trans.m[0][2] = dx;
	trans.m[1][2] = dy;
	return trans;
}
Matrix3D_t scalingZ(float m) {
	Matrix3D_t u = createIdentity();
	u.m[1][1] = m;
	u.m[0][0] = m;
	return u;
}
Vector3D_t point2Vector3d(Point3D_t point) {
	Vector3D_t vec;
	vec.v[0] = point.x;
	vec.v[1] = point.y;
	vec.v[2] = point.z;
	return vec;
}
Point3D_t vector2Point3d(Vector3D_t vector) {
	Point3D_t pnt;
	pnt.x = vector.v[0];
	pnt.y = vector.v[1];
	pnt.z = vector.v[2];
	return pnt;
}
Vector3D_t operator*(Matrix3D_t a, Vector3D_t b)
{
	Vector3D_t c;
	for (int i = 0; i < 3; i++) {
		c.v[i] = 0;
		for (int j = 0; j < 3; j++)
			c.v[i] += a.m[i][j] * b.v[j];
	}
	return c;
}
Vector3D_t operator + (Matrix3D_t a, Vector3D_t b) {
	Vector3D_t c;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			c.v[i] = a.m[i][j] + b.v[i];
		}
	}
	return c;
}
Matrix3D_t operator * (Matrix3D_t a, Matrix3D_t b)
{
	Matrix3D_t c;
	int i, j, k;
	for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) {
		c.m[i][j] = 0;
		for (k = 0; k < 3; k++)
			c.m[i][j] += a.m[i][k] * b.m[k][j];
	}
	return c;
}
Point3D_t putar_titik_pada_roda(Point3D_t center, bool filled, int radius,
	int right) {
	static int speed = 1;
	float theta = (float)(speed / 57.3);
	center.x = (int)(right * radius * cos(theta)) + center.x;
	center.y = (int)(right * radius * sin(theta)) + center.y;
	if (filled) {
		makeCircle(center, 10, true, { 0,1,0 });
	}
	speed += 1;
	return center;
}

// Function untuk menggambar object
void bola_pantul() {
	int speed = 2;
	int ballCount = 2;
	Point3D_t kotak[4] = {
	{-200,200,0},
	{200,200,0},
	{200,-200,0},
	{-200,-200}
	};
	static Point3D_t centerBall[2] = {
	{-190,-160,0},
	{190,80,0}
	};
	Color_t colorBall[2] = {
	{1,0,0},
	{0,0,1}
	};
	static int indexH[2] = { 1,1 };
	static int indexV[2] = { 1,1 };
	for (size_t i = 0; i < ballCount; i++)
	{
		// Untuk cek posisi horizontal
		if (centerBall[i].x >= 190) {
			indexH[i] = -1;
		}
		else if (centerBall[i].x <= -190) {
			indexH[i] = 1;
		}
		// Untuk cek posisi vertikal
		if (centerBall[i].y >= 190) {
			indexV[i] = -1;
		}
		else if (centerBall[i].y <= -190) {
			indexV[i] = 2;
		}
		// Translasikan
		Matrix3D_t move = translation(indexH[i] * speed, indexV[i] * speed);
		Vector3D_t newCenter =
			operator+(move, point2Vector3d(centerBall[i]));
		centerBall[i] = vector2Point3d(newCenter);
		makeCircle(centerBall[i], 20, true, colorBall[i]);
	}
	drawPolygon(kotak, 4, { 0,1,0 });
}

void animasi_pacman() {
	int speed = 1;
	static bool open = false;
	static int direction = 1;
	static Point3D_t centerPacman = { -200,0,0 };
	static Point3D_t centerRoda[2] = {
	{-320, 150, 0},
	{-470, 150, 0}
	};
	static Point3D_t batangRoda[2];
	static Point3D_t batangRoda1[2];
	if (centerPacman.x >= 220) {
		direction = -1;
	}
	else if (centerPacman.x <= -220) {
		direction = 1;
	}
	if (((int)centerPacman.x % 5) == 0) {
		open = !open;
	}
	// Translasi pacman
	Matrix3D_t move = translation(direction * speed, 0);
	Vector3D_t newCenter =
		operator+(move, point2Vector3d(centerPacman));
	centerPacman = vector2Point3d(newCenter);
	drawPacman(centerPacman, 40, { 0,0,1 }, open, direction);
	
	// Translasi dan rotasi roda
	for (size_t i = 0; i < 2; i++)
	{
		Matrix3D_t move1 = translation(speed, 0);
		Vector3D_t newCenter1 = operator+(move1,
			point2Vector3d(centerRoda[i]));
		centerRoda[i] = vector2Point3d(newCenter1);
		makeCircle(centerRoda[i], 50, false, { 1,0,0 });
		batangRoda[i] = putar_titik_pada_roda(centerRoda[i], true, 50, 1);
		batangRoda1[i] = putar_titik_pada_roda(centerRoda[i], false, 50, -1);
	}
	if (centerRoda[1].x >= 370) {
		centerRoda[0] = { -350, 150, 0 };
		centerRoda[1] = { -500, 150, 0 };
	}
	drawLine(batangRoda, 2, { 0,1,0 });
	drawLine(batangRoda1, 2, { 0,1,0 });
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	initialize();
	glutDisplayFunc(display);
	glutTimerFunc(1, timer, 0);
	glutMainLoop();
	return 0;
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	bola_pantul();
	//animasi_pacman();
	glutSwapBuffers();
}
void drawLine(Point3D_t pnt[], int n, Color_t color) {
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	for (size_t i = 0; i < n; i++)
	{
		glVertex2d(pnt[i].x, pnt[i].y);
	}
	glEnd();
}
void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(50, timer, 0);
}
void initialize(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutCreateWindow("2103181060 - Nova Andre Saputra");
	glClearColor(1, 1, 1, 0);
	gluOrtho2D(-320, 320, -240, 240);
}