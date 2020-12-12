#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <fstream>
#include <limits>
#include <string>

using namespace std;

typedef struct {
    float x;
    float y;
    float z;
} Point3D_t;

typedef struct {
    float x;
    float y;
} Point2D_t;

typedef struct {
    float m[2][2];
} Matrix2D_t;

typedef struct {
    float m[4][4];
} Matrix3D_t;

typedef struct {
    float v[2];
} Vector2D_t;

typedef struct {
    float v[3];
} Vector3D_t;

typedef struct {
    float r;
    float g;
    float b;
} Color_t;

typedef struct {
    int NumberOfVertices;
    int point[32];
} Face_t;

typedef struct {
    int NumberOfVertices;
    Point3D_t point[100];
    int NumberOfFaces;
    Face_t face[32];
} Object3D_t;

float angle;
float angles[10];
Matrix3D_t matrix3DX;
Matrix3D_t matrix3DY;
Matrix3D_t matrix3DZ;
Vector3D_t vector3D;

fstream& GotoLine(fstream& file, unsigned int num) {
    file.seekg(ios::beg);
    for (int i = 0; i < num - 1; ++i) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return file;
}

void setColor(Color_t col) {
    glColor3f(col.r, col.g, col.b);
}

Matrix3D_t createIdentity() {
    Matrix3D_t rotate;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            rotate.m[i][j] = 0.;
        rotate.m[i][i] = 1.;
    }
    return rotate;
}

Matrix3D_t rotationX(float theta) {
    Matrix3D_t rotate = createIdentity();
    rotate.m[0][0] = 1.0;
    rotate.m[0][1] = 0.0;
    rotate.m[0][2] = 0.0;
    rotate.m[1][0] = 0.0;
    rotate.m[1][1] = cos(theta / 57.3);
    rotate.m[1][2] = -sin(theta / 57.3);
    rotate.m[2][0] = 0.0;
    rotate.m[2][1] = sin(theta / 57.3);
    rotate.m[2][2] = cos(theta / 57.3);
    return rotate;
}

Matrix3D_t rotationY(float theta) {
    Matrix3D_t rotate = createIdentity();
    rotate.m[0][0] = cos(theta / 57.3); rotate.m[0][1] = 0.0; rotate.m[0][2] = sin(theta / 57.3);
    rotate.m[1][0] = 0.0; rotate.m[1][1] = 1.0; rotate.m[1][2] = 0.0;
    rotate.m[2][0] = -sin(theta / 57.3); rotate.m[2][1] = 0.0; rotate.m[2][2] = cos(theta / 57.3);
    return rotate;
}

Matrix3D_t rotationZ(float theta) {
    Matrix3D_t rotate = createIdentity();
    rotate.m[0][0] = cos(theta / 57.3); rotate.m[0][1] = -sin(theta / 57.3);
    rotate.m[0][2] = 0.0;
    rotate.m[1][0] = sin(theta / 57.3); rotate.m[1][1] = cos(theta / 57.3);
    rotate.m[1][2] = 0.0;
    rotate.m[2][0] = 0.0; rotate.m[2][1] = 0.0; rotate.m[2][2] = 1.0;
    return rotate;
}

Vector3D_t Point2Vector2D(Point2D_t point) {
    Vector3D_t vector;
    vector.v[0] = point.x;
    vector.v[1] = point.y;
    vector.v[2] = 1;
    return vector;
}

Vector3D_t Point2Vector3D(Point3D_t point) {
    Vector3D_t vector;
    vector.v[0] = point.x;
    vector.v[1] = point.y;
    vector.v[2] = point.z;
    return vector;
}

Point2D_t Point3D2Point2D(Point3D_t iPoint) {
    Point2D_t point;
    point.x = iPoint.x;
    point.y = iPoint.y;
    return point;
}

Point2D_t Vector2Point2D(Vector2D_t vector) {
    Point2D_t point;
    point.x = vector.v[0];
    point.y = vector.v[1];
    return point;
}

Point2D_t Vector3D2Point2D(Vector3D_t vector) {
    Point2D_t point;
    point.x = vector.v[0];
    point.y = vector.v[1];
    return point;
}

Point3D_t Vector2Point3D(Vector3D_t vector) {
    Point3D_t point;
    point.x = vector.v[0];
    point.y = vector.v[1];
    point.z = vector.v[2];
    return point;
}

Vector3D_t operator^(Vector3D_t a, Vector3D_t b) {
    Vector3D_t c;
    c.v[0] = a.v[1] * b.v[2] - a.v[2] * b.v[1];
    c.v[1] = a.v[2] * b.v[0] - a.v[0] * b.v[2];
    c.v[2] = a.v[0] * b.v[1] - a.v[1] * b.v[0];
    return c;
}

Matrix3D_t operator*(Matrix3D_t a, Matrix3D_t b) {
    Matrix3D_t c;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            c.m[i][j] = 0;
            for (int k = 0; k < 3; k++)
                c.m[i][j] += a.m[i][k] * b.m[k][j];
        }
    }
    return c;
}

Vector3D_t operator*(Matrix3D_t a, Vector3D_t b) {
    Vector3D_t c;
    for (int i = 0; i < 3; i++) {
        c.v[i] = 0;
        for (int j = 0; j < 3; j++)
            c.v[i] += a.m[i][j] * b.v[j];
    }
    return c;
}

Vector3D_t operator-(Vector3D_t a, Vector3D_t b) {
    Vector3D_t c;
    for (int i = 0; i < 3; i++) {
        c.v[i] = a.v[i] - b.v[i];
    }
    return c;
}

void drawDot(float x, float y, Color_t color) {
    setColor(color);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawPolyline(Point3D_t point[], int n, Color_t color) {
    setColor(color);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < n; i++) {
        glVertex3f(point[i].x, point[i].y, point[i].z);
    }
    glEnd();
}

void drawPolygon(Point3D_t point[], int n, Color_t color) {
    setColor(color);
    glBegin(GL_POLYGON);
    for (int i = 0; i < n; i++) {
        glVertex3f(point[i].x, point[i].y, point[i].z);
    }
    glEnd();
}

void drawLine(Point2D_t pnt[], int n, Color_t color) {
    setColor(color);
    glBegin(GL_LINES);
    for (int i = 0; i < n; i++) {
        glVertex2f(pnt[i].x, pnt[i].y);
    }
    glEnd();
}

void drawCircle(Point3D_t point, Color_t color, float r, int fill) {
    setColor(color);
    Point3D_t circle[360];
    for (int i = 0; i < 360; i++) {
        circle[i].x = (float)(r * cos(i * 3.14 / 180)) + point.x;
        circle[i].y = 0.0;
        circle[i].z = (float)(r * sin(i * 3.14 / 180)) + point.z;
    }
    if (fill == 0)
        drawPolyline(circle, 360, color);
    else
        drawPolygon(circle, 360, color);
}

void drawCoordinatAxis() {
    Point2D_t xAxis[2] = { {-200.0,0.0}, {200.0,0.0} };
    Point2D_t yAxis[2] = { {0.0,-200.0}, {0.0,200.0} };
    Point2D_t zAxis[2] = { {-200.0,-200.0}, {200.0,200.0} };
    Color_t col = { 0.0, 0.0, 0.0 };
    drawLine(xAxis, 2, col);
    drawLine(yAxis, 2, col);
    drawLine(zAxis, 2, col);
}

void drawObject3D(Object3D_t object) {
    matrix3DX = rotationX(angle);
    matrix3DY = rotationY(angle);
    matrix3DZ = rotationZ(angle);
    int numFacePoint;
    Vector3D_t buffVector[4];
    Color_t color;
    for (int i = 0; i < object.NumberOfVertices; i++) {
        vector3D = Point2Vector3D(object.point[i]);
        vector3D = operator*(matrix3DZ, vector3D);
        vector3D = operator*(matrix3DY, vector3D);
        vector3D = operator*(matrix3DX, vector3D);
        object.point[i] = Vector2Point3D(vector3D);
    }
    angle += 1;
    Point3D_t point3D[4];
    Point2D_t point2D[4];
    Vector3D_t normalVector;
    Vector3D_t buffVector1[4];
    for (int i = 0; i < object.NumberOfFaces; i++) {
        for (int j = 0; j < object.face[i].NumberOfVertices; j++) {
            point3D[j] = object.point[object.face[i].point[j]];
            point3D[j].z = 0.0;
            buffVector[j] = Point2Vector3D(point3D[j]);
        }
        normalVector = (buffVector[1] - buffVector[0]) ^ (buffVector[2] - buffVector[0]);
        if (normalVector.v[2] > 0) {
            color = { 0.0,1.0,0.0 };
            drawPolyline(point3D, object.face[i].NumberOfVertices, color);
        }
        else if (normalVector.v[2] <= 0) {
            color = { 0.0,0.0,1.0 };
            drawPolyline(point3D, object.face[i].NumberOfVertices, color);
        }
        cout << "Face ke-" << i << " : " << normalVector.v[2] << "\n";
    }
    cout << "\n";
}

void saveFile(Object3D_t object) {
    FILE* fp;
    int a1 = 5;
    int a2 = 6;
    double a3 = 20.123456789;
    int x, y, z, r, g, b, c;
    r = 255.0;
    g = 0.0;
    b = 0.0;
    c = 255.0;
    fp = fopen("D:/object.off", "w+");
    fprintf(fp, "COFF \n");
    int n1 = object.NumberOfVertices;
    int n2 = object.NumberOfFaces;
    fprintf(fp, "%d %d 0\n", n1, n2);
    for (int i = 0; i < object.NumberOfVertices; i++) {
        x = object.point[i].x;
        y = object.point[i].y;
        z = object.point[i].z;
        fprintf(fp, "%d %d %d %d %d %d %d \n", x, y, z, r, g, b, c);
    }
    int n, p1, p2, p3;
    for (int i = 0; i < object.NumberOfFaces; i++) {
        n = object.face[i].NumberOfVertices;
        p1 = object.face[i].point[0];
        p2 = object.face[i].point[1];
        p3 = object.face[i].point[2];
        fprintf(fp, "%d %d %d %d %d %d %d \n", n, p1, p2, p3, r, g, b);
    }
    fclose(fp);
}

Object3D_t readFile() {
    string line;
    fstream file;
    file.open("D:/object.off");
    string delimiter = " ";

    Object3D_t object;
    if (file.is_open()) {
        GotoLine(file, 2);
        getline(file, line);
        object.NumberOfVertices = stoi(line.substr(0, line.find(delimiter)));
        line.erase(0, line.find(delimiter) + delimiter.length());
        object.NumberOfFaces = stoi(line.substr(0, line.find(delimiter)));
        for (size_t i = 0; i < object.NumberOfVertices; i++) {
            GotoLine(file, 3 + i);
            getline(file, line);
            object.point[i].x = stof(line.substr(0, line.find(delimiter)));
            line.erase(0, line.find(delimiter) + delimiter.length());
            object.point[i].y = stof(line.substr(0, line.find(delimiter)));
            line.erase(0, line.find(delimiter) + delimiter.length());
            object.point[i].z = stof(line.substr(0, line.find(delimiter)));
        }

        int nuv;
        int a;
        int b;
        int c;
        int d;
        for (size_t i = 0; i < object.NumberOfFaces; i++) {
            GotoLine(file, 3 + object.NumberOfVertices + i);
            getline(file, line);
            nuv = stoi(line.substr(0, line.find(delimiter)));
            line.erase(0, line.find(delimiter) + delimiter.length());
            a = stoi(line.substr(0, line.find(delimiter)));
            line.erase(0, line.find(delimiter) + delimiter.length());
            b = stoi(line.substr(0, line.find(delimiter)));
            line.erase(0, line.find(delimiter) + delimiter.length());
            c = stoi(line.substr(0, line.find(delimiter)));
            line.erase(0, line.find(delimiter) + delimiter.length());
            d = stoi(line.substr(0, line.find(delimiter)));
            object.face[i] = { nuv, {a , b ,c,d} };
        }
        file.close();
    }
    else {
        cout << "File tidak bisa dibuka";
    }
    return object;
}

void drawCylinder() {
    int radius = 50;
    int height = 250;
    Point3D_t circle[32];
    for (size_t i = 0; i < 16; i++) {
        circle[i].x = (float)0 + (radius * cos(i * 3.14 / 8));
        circle[i].y = (float)0 + (radius * sin(i * 3.14 / 8));
        circle[i].z = 20;
    }
    for (size_t i = 16; i < 32; i++) {
        circle[i].x = (float)0 + (radius * cos(i * 3.14 / 8));
        circle[i].y = (float)0 + (radius * sin(i * 3.14 / 8));
        circle[i].z = -height;
    }
    Face_t face[16];
    for (int i = 0; i < 16; i++) {
        face[i] = { 4, {i , 16 + i , 16 + i + 1, i + 1} };
        if (i == 15)
            face[i] = { 4, {15 , 31 , 16 , 0} };
    }
    Object3D_t tube;
    tube.NumberOfVertices = 32;
    for (size_t i = 0; i < 32; i++) {
        tube.point[i] = circle[i];
    }
    tube.NumberOfFaces = 16;
    for (size_t i = 0; i < 16; i++) {
        tube.face[i] = face[i];
    }
    drawObject3D(tube);
    saveFile(tube);
}

void userDraw() {
    drawCoordinatAxis();
    drawCylinder();
    //drawObject3D(readFile());
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(50, timer, 0);
}

void initialize() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200., 200., -200.0, 200.0);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    userDraw();
    glFlush();
}

int main(int iArgc, char** cppargv) {
    glutInit(&iArgc, cppargv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(400, 400);
    glutCreateWindow("2103181060 - Nova Andre Saputra");
    initialize();
    glutDisplayFunc(display);
    glutTimerFunc(1, timer, 0);
    glutMainLoop();
    return 0;
}
