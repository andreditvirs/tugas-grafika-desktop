#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
using namespace std;
typedef struct {
    float x;
    float y;
    float z;
} Point3D_t;
typedef struct {
    float v[3];
} Vector3D_t;
typedef struct {
    float m[3][3];
} Matrix3D_t;
typedef struct {
    float r;
    float g;
    float b;
} Color_t;
typedef struct {
    int numberOfVertices;
    int pnt[32];
} Face_t;
typedef struct {
    int numberOfVertices;
    Point3D_t pnt[32];
    int numberOfFaces;
    Face_t face[32];
} Object3D_t;
Matrix3D_t createIdentity() {
    Matrix3D_t rotate;
    rotate.m[0][0] = 1.0; rotate.m[0][1] = 0.0; rotate.m[0][2] = 0.0;
    rotate.m[1][0] = 0.0; rotate.m[1][1] = 1.0; rotate.m[1][2] = 0.0;
    rotate.m[2][0] = 0.0; rotate.m[2][1] = 0.0; rotate.m[2][2] = 1.0;
    return rotate;
}
void setColor(Color_t col) {
    glColor3f(col.r, col.g, col.b);
}
Matrix3D_t rotationX(float teta) {
    Matrix3D_t rotate = createIdentity();
    float cs = cos(teta / 57.3);
    float sn = sin(teta / 57.3);
    rotate.m[1][1] = cs; rotate.m[1][2] = -sn;
    rotate.m[2][1] = sn; rotate.m[2][2] = cs;
    return rotate;
}
Matrix3D_t rotationY(float teta) {
    Matrix3D_t rotate = createIdentity();
    float cs = cos(teta / 57.3);
    float sn = sin(teta / 57.3);
    rotate.m[0][0] = cs; rotate.m[0][2] = sn;
    rotate.m[2][0] = -sn; rotate.m[2][2] = cs;
    return rotate;
}
Matrix3D_t rotationZ(float teta) {
    Matrix3D_t rotate = createIdentity();
    rotate.m[0][0] = cos(teta / 57.3); rotate.m[0][1] = -sin(teta / 57.3);
    rotate.m[0][2] = 0.0;
    rotate.m[1][0] = sin(teta / 57.3); rotate.m[1][1] = cos(teta / 57.3);
    rotate.m[1][2] = 0.0;
    rotate.m[2][0] = 0.0; rotate.m[2][1] = 0.0; rotate.m[2][2] = 1.0;
    return rotate;
}
Vector3D_t Point2Vector3D(Point3D_t pnt) {
    Vector3D_t vec;
    vec.v[0] = pnt.x;
    vec.v[1] = pnt.y;
    vec.v[2] = pnt.z;
    return vec;
}
Point3D_t Vector2Point3D(Vector3D_t vec) {
    Point3D_t pnt;
    pnt.x = vec.v[0];
    pnt.y = vec.v[1];
    pnt.z = vec.v[2];
    return pnt;
}
Vector3D_t operator *(Matrix3D_t matrix, Vector3D_t vector) {
    Vector3D_t vectorHasil;
    for (int i = 0; i < 3; i++) {
        vectorHasil.v[i] = 0;
        for (int j = 0; j < 3; j++) {
            vectorHasil.v[i] += matrix.m[i][j] * vector.v[j];
        }
    }
    return vectorHasil;
}
Vector3D_t operator^(Vector3D_t a, Vector3D_t b) {
    Vector3D_t c;
    c.v[0] = a.v[1] * b.v[2] - a.v[2] * b.v[1];
    c.v[1] = a.v[2] * b.v[0] - a.v[0] * b.v[2];
    c.v[2] = a.v[0] * b.v[1] - a.v[1] * b.v[0];
    return c;
}
Vector3D_t operator-(Vector3D_t a, Vector3D_t b) {
    Vector3D_t c;
    c.v[0] = a.v[0] - b.v[0];
    c.v[1] = a.v[1] - b.v[1];
    c.v[2] = a.v[2] - b.v[2];
    return c;
}
void drawPolygon3D(Point3D_t pnt[], int n, Color_t color) {
    int i;
    setColor(color);
    glBegin(GL_POLYGON);
    for (i = 0; i < n; i++) {
        glVertex2f(pnt[i].x, pnt[i].y);
    }
    glEnd();
}
void drawObject3DVisible(Object3D_t object) {
    Color_t col[5] = {
    { 100 / 255.0, 125 / 255.0, 225 / 255.0 },
    { 200 / 255.0, 135 / 255.0, 210 / 255.0 },
    { 300 / 255.0, 145 / 255.0, 205 / 255.0 },
    { 400 / 255.0, 185 / 255.0, 285 / 255.0 },
    { 500 / 255.0, 225 / 255.0, 250 / 255.0 }
    };
    static int sudut = 10;
    static Matrix3D_t matrix3DX;
    static Matrix3D_t matrix3DY;
    static Matrix3D_t matrix3DZ;
    static Vector3D_t vec3D;
    matrix3DX = rotationX(sudut);
    matrix3DY = rotationY(sudut);
    matrix3DZ = rotationZ(sudut);
    for (size_t i = 0; i < object.numberOfVertices; i++) {
        vec3D = Point2Vector3D(object.pnt[i]);
        vec3D = operator*(matrix3DZ, vec3D);
        vec3D = operator*(matrix3DY, vec3D);
        vec3D = operator*(matrix3DX, vec3D);
        object.pnt[i] = Vector2Point3D(vec3D);
    }
    Point3D_t pnt3D[4];
    sudut += 1;
    static Vector3D_t vecbuff[3];
    static Vector3D_t normalVector;
    for (size_t i = 0; i < object.numberOfFaces; i++) {
        for (size_t j = 0; j < object.face[i].numberOfVertices; j++) {
            int temp = object.face[i].pnt[j];
            vecbuff[j] = Point2Vector3D(object.pnt[temp]);
        }
        normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
        cout << "Face ke-" << i << " Normalisasi = " << normalVector.v[2] << endl;
        if (normalVector.v[2] > 0) {
            for (size_t j = 0; j < object.face[i].numberOfVertices; j++) {
                pnt3D[j] = object.pnt[object.face[i].pnt[j]];
            }
            drawPolygon3D(pnt3D, object.face[i].numberOfVertices, col[i]);
        }
    }
}
void makePyramid() {
    static Object3D_t pyramid = {
    5,
    {
    {0,150,0},
    {100,0,0},
    {0,0,100},
    {-100,0,0},
    {0,0,-100}
    },
    5,
    {
    {3, {0,2,1}},
    {3, {0,3,2}},
    {3, {0,4,3}},
    {3, {0,1,4}},
    {4, {1,2,3,4}}
    }
    };
    drawObject3DVisible(pyramid);
}
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    makePyramid();
    glFlush();
}
void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(20, timer, 0);
}
void Initialize(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-300, 300, -300, 300);
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(300, 25);
    glutInitWindowSize(600, 600);
    glutCreateWindow("2103181060 - Nova Andre Saputra");
    Initialize();
    glutTimerFunc(1, timer, 0);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}