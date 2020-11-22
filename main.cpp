using namespace std;
#include <iostream>
#include <GL/glut.h>
#include <math.h>
typedef struct {
	float x;
	float y;
	float z;
} Point3D_t;
typedef struct {
	float m[3][3];
} Matrix3D_t;
typedef struct {
	float v[4];
} Vector3D_t;
typedef struct {
	float r;
	float g;
	float b;
} Color_t;
typedef struct {
	int numberOfVertices;
	short int pnt[32];
} Face_t;
typedef struct {
	int numberOfVertices;
	Point3D_t pnt[32];
	int numberOfFaces;
	Face_t face[32];
} Object3D_t;
void drawLine(Point3D_t[], int, Color_t);
void timer(int);
void display(void);
void initialize(void);
void sumbuKoordinat();
void drawPolygon(Point3D_t[], int, Color_t);
void drawFillPolygon(Point3D_t[], int, Color_t);
void makeCircle(Point3D_t, int, bool, Color_t);
void drawObject3D(Object3D_t, int);
Matrix3D_t rotationX(float);
Matrix3D_t rotationY(float);
Matrix3D_t rotationZ(float);
Matrix3D_t translation(int, int);
Matrix3D_t scalingZ(float);
Matrix3D_t createIdentity();
Vector3D_t point2Vector3d(Point3D_t point);
Point3D_t vector2Point3d(Vector3D_t vector);
Vector3D_t operator*(Matrix3D_t, Vector3D_t);
Vector3D_t operator+(Matrix3D_t, Vector3D_t);
Matrix3D_t operator * (Matrix3D_t, Matrix3D_t);
void makeCube();
void makeCube2();
void sumbuKoordinat() {
	Point3D_t sumbuX[2] = { {-320,0},{320,0} };
	Point3D_t sumbuY[2] = { {0,-240},{0,240} };
	Color_t col = { 0,0,1 };
	drawLine(sumbuX, 2, col);
	drawLine(sumbuY, 2, col);
}
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
void drawObject3D(Object3D_t object, int n) {
	static int sudut = 10;
	static Matrix3D_t matrix3DX;
	static Matrix3D_t matrix3DY;
	static Matrix3D_t matrix3DZ;
	static Vector3D_t vec3D;
	Color_t col = { 0,0,1 };
	matrix3DX = rotationX(sudut);
	matrix3DY = rotationY(sudut);
	matrix3DZ = rotationZ(sudut);
	for (size_t i = 0; i < object.numberOfVertices; i++)
	{
		vec3D = point2Vector3d(object.pnt[i]);
		vec3D = operator*(matrix3DZ, vec3D);
		vec3D = operator*(matrix3DY, vec3D);
		vec3D = operator*(matrix3DX, vec3D);
		object.pnt[i] = vector2Point3d(vec3D);
	}
	Point3D_t pnt3D[4];
	sudut += 1;
	for (size_t i = 0; i < object.numberOfFaces; i++)
	{
		for (size_t j = 0; j < object.face[i].numberOfVertices; j++)
		{
			pnt3D[j] = object.pnt[object.face[i].pnt[j]];
		}
		drawPolygon(pnt3D, object.face[i].numberOfVertices, col);
	}
}
Matrix3D_t createIdentity() {
	Matrix3D_t a;
	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 3; j++)
			a.m[i][j] = 0;
		a.m[i][i] = 1;
	}
	return a;
}
Matrix3D_t rotationX(float theta) {
	Matrix3D_t rotate = createIdentity();
	float cs = cos(theta / 57.3);
	float sn = sin(theta / 57.3);
	rotate.m[1][1] = cs; rotate.m[1][2] = -sn;
	rotate.m[2][1] = sn; rotate.m[2][2] = cs;
	return rotate;
}
Matrix3D_t rotationY(float theta) {
	Matrix3D_t rotate = createIdentity();
	float cs = cos(theta / 57.3);
	float sn = sin(theta / 57.3);
	rotate.m[0][0] = cs; rotate.m[0][2] = sn;
	rotate.m[2][0] = -sn; rotate.m[2][2] = cs;
	return rotate;
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
Vector3D_t operator+(Matrix3D_t a, Vector3D_t b) {
	Vector3D_t c;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			c.v[i] = a.m[i][j] + b.v[i];
		}
	}
	c.v[3] = 1;
	return c;
}
Matrix3D_t operator * (Matrix3D_t a, Matrix3D_t b)
{
	Matrix3D_t c;
	int i, j, k;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++) {
			c.m[i][j] = 0;
			for (k = 0; k < 3; k++)
				c.m[i][j] += a.m[i][k] * b.m[k][j];
		}
	return c;
}
void makeCube() {
	static Object3D_t kubus = {
	8,
	{
	{-100, -100, 100},
	{ 100, -100, 100},
	{ 100, 100, 100},
	{-100, 100, 100},
	{-100, -100, -100},
	{ 100, -100, -100},
	{ 100, 100, -100},
	{-100, 100, -100}
	},
	6,
	{
	{4, {0,1,2,3}},
	{4, {1,5,6,2}},
	{4, {4,7,6,5}},
	{4, {0,3,7,4}},
	{4, {4,5,1,0}},
	{4, {2,3,7,6}}
	}
	};
	drawObject3D(kubus, 4);
}
void makeCube2() {
	static Object3D_t kubus = {
	8,
	{
	{-100, -100, 100},
	{ 100, -100, 100},
	{ 100, 100, 100},
	{-100, 100, 100},
	{-100, -100, -100},
	{ 100, -100, -100},
	{ 100, 100, -100},
	{-100, 100, -100}
	},
	12,
	{
	{3, {0,3,2}},
	{3, {0,2,1}},
	{3, {1,6,2}},
	{3, {1,5,6}},
	{3, {4,5,6}},
	{3, {4,6,7}},
	{3, {3,0,7}},
	{3, {0,4,7}},
	{3, {3,2,6}},
	{3, {3,6,7}},
	{3, {0,1,5}},
	{3, {0,5,4}}
	}
	};
	drawObject3D(kubus, 3);
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
	//makeCube();
	makeCube2();
	glFlush();
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-320, 320, -240, 240);
}