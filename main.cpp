#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Struct untuk menyimpan koordinat titik (x,y)
typedef struct {
	float x;
	float y;
} point2D_t;

// Struct untuk membuat warna rgb
typedef struct {
	float r;
	float g;
	float b;
}color_t;

// Fungsi untuk set warna
void setColor(color_t col) {
	glColor3f(col.r, col.g, col.b);
}

// Fungsi untuk menggambar garis
void drawLine(point2D_t pnt[], int n, color_t color) {
	int i;
	setColor(color);
	glBegin(GL_LINES);
	for (i = 0; i < n; i++) {
		glVertex2f(pnt[i].x, pnt[i].y);
	}glEnd();
}

// Fungsi untuk menggambar polygon (kurva tertutup)
void drawPolygon(point2D_t pnt[], int n, color_t color) {
	int i;
	setColor(color);
	glBegin(GL_POLYGON);
	for (i = 0; i < n; i++) {
		glVertex2f(pnt[i].x, pnt[i].y);
	}glEnd();
}

// Fungsi untuk menggambar polyline (kurva terbuka)
void drawPolyline(point2D_t pnt[], int n, color_t col1) {
	int i;
	//setColor(color);
	glColor3f(col1.r, col1.g, col1.b);
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < n; i++) {
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

// Fungsi untuk membuat sumbu koordinat (sb. x dan sb. y)
void sumbu_koordinat() {
	point2D_t sumbux[2] = { {-200.0,0.0},{200.0,0.0} };
	point2D_t sumbuy[2] = { {0.0,-200.0},{0.0,200.0} };
	color_t col = { 0.0,0.0,1.0 };
	drawLine(sumbux, 2, col);
	drawLine(sumbuy, 2, col);
}

// Fungsi untuk membuat segitiga
void segitiga() {
	point2D_t segitiga01[3] = { {-20.0,60.0},{20.0,60.0},{0.0,100.0} };
	point2D_t segitiga02[3] = { {-20.0,-60.0},{20.0,-60.0},{0.0,-100.0} };
	point2D_t segitiga03[3] = { {60.0,20.0},{60.0,-20.0},{100.0,0.0} };
	point2D_t segitiga04[3] = { {-60.0,20.0},{-60.0,-20.0},{-100.0,0.0} };
	color_t col = { 0.0, 1.0, 0.0 };
	drawPolygon(segitiga01, 3, col);
	drawPolygon(segitiga02, 3, col);
	drawPolygon(segitiga03, 3, col);
	drawPolygon(segitiga04, 3, col);
	color_t col1 = { 1.0, 0.0, 0.0 };
	point2D_t segitiga05[3] = { {40.0,40.0},{60.0,100.0},{80.0,40.0} };
	point2D_t segitiga06[3] = { {-40.0,40.0},{-60.0,100.0},{-80.0,40.0} };
	point2D_t segitiga07[3] = { {-40.0,-40.0},{-60.0,-100.0},{-80.0,-40.0} };
	point2D_t segitiga08[3] = { {40.0,-40.0},{60.0,-100.0},{80.0,-40.0} };
	drawPolyline(segitiga05, 3, col1);
	drawPolyline(segitiga06, 3, col1);
	drawPolyline(segitiga07, 3, col1);
	drawPolyline(segitiga08, 3, col1);
}

// Fungsi untuk menggambar kotak
void kotak() {
	point2D_t kotak01[4] = { {100.0,40.0}, {100.0, 80.0}, {140.0, 80.0},
	{140.0,40.0} };
	point2D_t kotak02[4] = { {-100.0,40.0}, {-100.0, 80.0}, {-140.0, 80.0},
	{-140.0,40.0} };
	point2D_t kotak03[4] = { {-100.0,-40.0}, {-100.0, -80.0}, {-140.0, -
	80.0}, {-140.0,-40.0} };
	point2D_t kotak04[4] = { {100.0,-40.0}, {100.0, -80.0}, {140.0, -80.0},
	{140.0,-40.0} };
	color_t col = { 0.0,0.0,1.0 };
	drawPolygon(kotak01, 4, col);
	drawPolygon(kotak02, 4, col);
	drawPolygon(kotak03, 4, col);
	drawPolygon(kotak04, 4, col);
}

// Fungsi untuk membuat bintang
void bintang() {
	point2D_t bintang01[10] = { {0, 90}, {19,30}, {77,30}, {25,-5}, {47,-
	60},{0,-30}, {-47,-60},{-25,-5}, {-77,30}, {-19,30} };
	point2D_t bintang02[10] = { {99, 90}, {118,30}, {176,30}, {124,-5},
	{146,-60},{99,-30}, {52,-60},{74,-5}, {22,30}, {80,30} };
	point2D_t bintang03[10] = { {0, 204}, {19,144}, {77,144}, {25,109},
	{47,54},{0,84}, {-47,54},{-25,109}, {-77,144}, {-19,144} };
	point2D_t bintang04[10] = { {-99, 90}, {-118,30}, {-176,30}, {-124,-5},
	{-146,-60},{-99,-30}, {-52,-60},{-74,-5}, {-22,30}, {-80,30} };
	point2D_t bintang05[10] = { {0, -24}, {19,-84}, {77,-84}, {25,-119},
	{47,-174},{0,-144}, {-47,-174},{-25,-119}, {-77,-84}, {-19,-84} };
	color_t col = { 0.0, 0.0, 1.0 };
	color_t col1 = { 1.0, 0.0, 0.0 };
	drawPolyline(bintang01, 10, col1);
	drawPolyline(bintang02, 10, col);
	drawPolyline(bintang03, 10, col);
	drawPolyline(bintang04, 10, col);
	drawPolyline(bintang05, 10, col);
}

// Fungsi untuk menggambar
void userdraw(void) {
	//sumbu_koordinat();
	//segitiga();
	//kotak();
	bintang();
}

// Fungsi display
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	userdraw();
	glutSwapBuffers();
}

// Program utama (untuk menjalankan)
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("2103181060 Nova Andre Saputra");
	glClearColor(1.0, 1.0, 1.0, 0.0);
	gluOrtho2D(-200, 200, -200, 200);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}