#include <GL/glut.h>

// Fungsi untuk menggambar titik
void drawDot(int x, int y) {
	glColor3f(0.0, 0.0, 1.0);
	glPointSize(10);
	glBegin(GL_POINTS);
		glVertex2i(x, y);
	glEnd();
}

// Fungsi untuk menggambar garis
void drawLine(float x1, float y1, float x2, float y2) {
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0.0);
	glVertex3f(x2, y2, 0.0);
	glEnd();
}

// Fungsi untuk membuat efek tv rusak
void tv_rusak() {
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5);
	glBegin(GL_POINTS);
	for (int i = 0; i < 220; i++) {
		float a = (float)(rand() % 100);
		float b = (float)(rand() % 100);
		glVertex2i(a, b);
		glVertex2i(-a, b);
		glVertex2i(a, -b);
		glVertex2i(-a, -b);
	}
	glEnd();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	// Menggambar sebuah titik
	//drawDot(-150, 150);
	// Menggambar sebuah garis
	//drawLine(25, 25, 175, 175);
	// Menggambar satu bidang tv rusak
	tv_rusak();
	glutSwapBuffers();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("2103181060 Nova Andre Saputra");
	gluOrtho2D(-320., 320., -240.0, 240.0);
	//glutIdleFunc(display);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}