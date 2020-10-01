#include <GL/glut.h>
#include <iostream>
#include <GL/freeglut.h>
using namespace std;

float angle[5] = { 0.0, 0.0, 0.0, 0.0, 0.0 };

typedef struct {
	float x;
	float y;
} Point2D_t;

typedef struct {
	float r;
	float g;
	float b;
} Color_t;

void setColor(Color_t col)
{
	glColor3f(col.r, col.g, col.b);
}

void drawLine(Point2D_t pnt[], int n, Color_t color)
{
	int i;
	setColor(color);
	glBegin(GL_LINES);
	for (i = 0; i < n; i++) {
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawPolyline(Point2D_t pnt[], int n, Color_t color)
{
	int i;
	setColor(color);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i < n; i++) {
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawDot(int x, int y) {
	glColor3f(0.0, 0.0, 1.0);
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void sumbu_koordinat() {
	Point2D_t sumbuX[2] = { {-200.0, 0.0}, {200.0, 0.0} };
	Point2D_t sumbuY[2] = { {0.0, -200.0}, {0.0, 200.0} };
	Color_t col = { 0.0, 0.0, 1.0 };
	drawLine(sumbuX, 2, col);
	drawLine(sumbuY, 2, col);
}

void circle()
{
	Point2D_t circle1[360];
	Point2D_t circle2[360];
	Point2D_t circle3[360];
	Point2D_t circle4[360];
	Point2D_t circle5[360];
	Color_t col2 = { 0.0, 1.0, 0.0 };
	Color_t col = { 1.0, 0.0, 0.0 };
	float r = 50.0;
	for (int i = 0; i < 360; i++) {
		circle1[i].x = (float)(r * cos(i * 3.14 / 100));
		circle1[i].y = (float)(r * sin(i * 3.14 / 100));
	}
	for (int i = 0; i < 360; i++) {
		circle2[i].x = (float)(r * cos(i * 3.14 / 100)) + 100;
		circle2[i].y = (float)(r * sin(i * 3.14 / 100)) + 100;
	}
	for (int i = 0; i < 360; i++) {
		circle3[i].x = (float)(r * cos(i * 3.14 / 100)) - 100;
		circle3[i].y = (float)(r * sin(i * 3.14 / 100)) - 100;
	}

	for (int i = 0; i < 360; i++) {
		circle4[i].x = (float)(r * cos(i * 3.14 / 100)) + 100;
		circle4[i].y = (float)(r * sin(i * 3.14 / 100)) - 100;
	}

	for (int i = 0; i < 360; i++) {
		circle5[i].x = (float)(r * cos(i * 3.14 / 100)) - 100;
		circle5[i].y = (float)(r * sin(i * 3.14 / 100)) + 100;
	}

	drawPolyline(circle1, 360, col);
	drawPolyline(circle2, 360, col2);
	drawPolyline(circle3, 360, col2);
	drawPolyline(circle4, 360, col2);
	drawPolyline(circle5, 360, col2);
}

void titik_berputar(int r)
{
	float teta = (float)(angle[0] / 57.3);
	int x = (int)(r * cos(teta));
	int y = (int)(r * sin(teta));
	drawDot(x, y);
	angle[0] = angle[0] + 1;
	if (angle[0] <= -360) angle[0] = 0.0;
}

void titik_berputar1(int r) // posisi kiri bawah
{
	float teta = (float)(angle[1] / 57.3);
	int x = (int)(r * cos(teta) + 100);
	int y = (int)(r * sin(teta) + 100);
	drawDot(x, y);
	angle[1] = angle[1] + 0.5;
	if (angle[1] <= -360) angle[1] = 0.0;
}

void titik_berputar2(int r) // posisi kanan atas
{
	float teta = (float)(angle[2] / 57.3);
	int x = (int)(r * cos(teta) - 100);
	int y = (int)(r * sin(teta) - 100);
	drawDot(x, y);
	angle[2] = angle[2] - 0.3;
	if (angle[2] <= -360) angle[2] = 0.0;
}

void titik_berputar3(int r) // posisi kanan bawah
{
	float teta = (float)(angle[3] / 57.3);
	int x = (int)(r * cos(teta) + 100);
	int y = (int)(r * sin(teta) - 100);
	drawDot(x, y);
	angle[3] = angle[3] + 2;
	if (angle[3] <= -360) angle[3] = 0.0;
}

void titik_berputar4(int r) //kiri atas
{
	float teta = (float)(angle[4] / 57.3);
	int x = (int)(r * cos(teta) - 100);
	int y = (int)(r * sin(teta) + 100);
	drawDot(x, y);
	angle[4] = angle[4] + 0.7;
	if (angle[4] <= -360) angle[4] = 0.5;
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	sumbu_koordinat();
	circle();
	titik_berputar(50);
	titik_berputar1(50);
	titik_berputar2(50);
	titik_berputar3(50);
	titik_berputar4(50);
	glFlush();
}

void draw0() {
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void Initialize()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-200.0, 200.0, -200.0, 200.0);
}

void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(400, 400);
	glutCreateWindow("2103181060 - Nova Andre Saputra");
	Initialize();
	glutDisplayFunc(display);
	glutTimerFunc(10, timer, 0);
	glutMainLoop();
	return 0;
}