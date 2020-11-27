#include <GL/glut.h>
#include <math.h>

typedef struct
{
    float x;
    float y;
    float z;
} Point3D_t;

typedef struct
{
    float v[3];
} Vector3D_t;

typedef struct
{
    float m[3][3];
} Matrix3D_t;
typedef struct
{
    float r;
    float g;
    float b;
} Color_t;
typedef struct
{
    int numberOfVertices;
    int pnt[32];
} Face_t;
typedef struct
{
    int numberOfVertices;
    Point3D_t pnt[32];
    int numberOfFaces;
    Face_t face[32];
} Object3D_t;
Color_t yellow = { 1.0, 1.0, 0.0 };
Color_t blue = { 0.0, 0.0, 1.0 };
Matrix3D_t createIdentity()
{
    Matrix3D_t rotate;
    rotate.m[0][0] = 1.0;
    rotate.m[0][1] = 0.0;
    rotate.m[0][2] = 0.0;
    rotate.m[1][0] = 0.0;
    rotate.m[1][1] = 1.0;
    rotate.m[1][2] = 0.0;
    rotate.m[2][0] = 0.0;
    rotate.m[2][1] = 0.0;
    rotate.m[2][2] = 1.0;
    return rotate;
}
void setColor(Color_t col) { glColor3f(col.r, col.g, col.b); }
Matrix3D_t rotationX(float teta)
{
    Matrix3D_t rotate = createIdentity();
    float cs = cos(teta / 57.3);
    float sn = sin(teta / 57.3);
    rotate.m[1][1] = cs;
    rotate.m[1][2] = -sn;
    rotate.m[2][1] = sn;
    rotate.m[2][2] = cs;
    return rotate;
}
Matrix3D_t rotationY(float teta)
{
    Matrix3D_t rotate = createIdentity();
    float cs = cos(teta / 57.3);
    float sn = sin(teta / 57.3);
    rotate.m[0][0] = cs;
    rotate.m[0][2] = sn;
    rotate.m[2][0] = -sn;
    rotate.m[2][2] = cs;
    return rotate;
}
Matrix3D_t rotationZ(float teta)
{
    Matrix3D_t rotate = createIdentity();
    rotate.m[0][0] = cos(teta / 57.3);
    rotate.m[0][1] = -sin(teta / 57.3);
    rotate.m[0][2] = 0.0;
    rotate.m[1][0] = sin(teta / 57.3);
    rotate.m[1][1] = cos(teta / 57.3);
    rotate.m[1][2] = 0.0;
    rotate.m[2][0] = 0.0;
    rotate.m[2][1] = 0.0;
    rotate.m[2][2] = 1.0;
    return rotate;
}
Vector3D_t Point2Vector3D(Point3D_t pnt)
{
    Vector3D_t vec;
    vec.v[0] = pnt.x;
    vec.v[1] = pnt.y;
    vec.v[2] = pnt.z;
    return vec;
}
Point3D_t Vector2Point3D(Vector3D_t vec)
{
    Point3D_t pnt;
    pnt.x = vec.v[0];
    pnt.y = vec.v[1];
    pnt.z = vec.v[2];
    return pnt;
}
Vector3D_t operator *(Matrix3D_t matrix, Vector3D_t vector)
{
    Vector3D_t vectorHasil;
    for (int i = 0; i < 3; i++)
    {
        vectorHasil.v[i] = 0;
        for (int j = 0; j < 3; j++)
        {
            vectorHasil.v[i] += matrix.m[i][j] * vector.v[j];
        }
    }
    return vectorHasil;
}
void drawPolyline3D(Point3D_t pnt[], int n, Color_t color)
{
    int i;
    setColor(color);
    glBegin(GL_LINE_LOOP);
    for (i = 0; i < n; i++)
    {
        glVertex2f(pnt[i].x, pnt[i].y);
    }
    glEnd();
}
void drawObject3D(Object3D_t object, Color_t color)
{
    static int sudut = 10;
    static Matrix3D_t matrix3DX;
    static Matrix3D_t matrix3DY;
    static Matrix3D_t matrix3DZ;
    static Vector3D_t vec3D;
    matrix3DX = rotationX(sudut);
    matrix3DY = rotationY(sudut);
    matrix3DZ = rotationZ(sudut);
    for (size_t i = 0; i < object.numberOfVertices; i++)
    {
        vec3D = Point2Vector3D(object.pnt[i]);
        vec3D = operator*(matrix3DZ, vec3D);
        vec3D = operator*(matrix3DY, vec3D);
        vec3D = operator*(matrix3DX, vec3D);
        object.pnt[i] = Vector2Point3D(vec3D);
    }
    Point3D_t pnt3D[4];
    sudut += 1;
    for (size_t i = 0; i < object.numberOfFaces; i++)
    {
        for (size_t j = 0; j < object.face[i].numberOfVertices; j++)
        {
            pnt3D[j] = object.pnt[object.face[i].pnt[j]];
        }
        drawPolyline3D(pnt3D, object.face[i].numberOfVertices, color);
    }
}
void selimut()
{
    setColor(yellow);
    int radius = 50;
    int height = 200;
    Point3D_t circle[32];
    for (size_t i = 0; i < 16; i++)
    {
        circle[i].x = (float)0 + (radius * cos(i * 3.14 / 8));
        circle[i].y = (float)0 + (radius * sin(i * 3.14 / 8));
        circle[i].z = 20;
    }
    for (size_t i = 16; i < 32; i++)
    {
        circle[i].x = (float)0 + (radius * cos(i * 3.14 / 8));
        circle[i].y = (float)0 + (radius * sin(i * 3.14 / 8));
        circle[i].z = -height;
    }
    Face_t face[16];
    for (int i = 0; i < 16; i++)
    {
        face[i] = { 4, {i, 16 + i, 17 + i, i + 1} };
        if (i == 15)
            face[i] = { 4, {15, 31, 16, 0} };
    }
    Object3D_t tube;
    tube.numberOfVertices = 32;
    for (size_t i = 0; i < 32; i++)
    {
        tube.pnt[i] = circle[i];
    }
    tube.numberOfFaces = 16;
    for (size_t i = 0; i < 16; i++)
    {
        tube.face[i] = face[i];
    }
    drawObject3D(tube, yellow);
}
void tutup()
{
    setColor(blue);
    int radius = 50;
    int height = 200;
    Point3D_t circle[16];
    Point3D_t circle2[16];
    for (size_t i = 0; i < 16; i++)
    {
        circle[i].x = (float)0 + (radius * cos(i * 3.14 / 8));
        circle[i].y = (float)0 + (radius * sin(i * 3.14 / 8));
        circle[i].z = 20;
        circle2[i].x = (float)0 + (radius * cos(i * 3.14 / 8));
        circle2[i].y = (float)0 + (radius * sin(i * 3.14 / 8));
        circle2[i].z = -height;
    }
    Face_t face[24];
    Face_t face2[24];
    for (int i = 0; i < 24; i++)
    {
        if (i == 23)
        {
            face[i] = { 2, {i - 8, 0} };
        }
        else if (i < 8) { face[i] = { 2, {i, 8 + i} }; }
        else { face[i] = { 2, {i - 8, i - 7} }; }
        if (i == 23)
        {
            face2[i] = { 2, {i - 8, 0} };
        }
        else if (i < 8) { face2[i] = { 2, {i, 8 + i} }; }
        else { face2[i] = { 2, {i - 8, i - 7} }; }
    }
    Object3D_t tube;
    Object3D_t tube2;
    tube.numberOfVertices = 16;
    tube2.numberOfVertices = 16;
    for (size_t i = 0; i < 16; i++)
    {
        tube.pnt[i] = circle[i];
        tube2.pnt[i] = circle2[i];
    }
    tube.numberOfFaces = 24;
    tube2.numberOfFaces = 24;
    for (size_t i = 0; i < 24; i++)
    {
        tube.face[i] = face[i];
        tube2.face[i] = face2[i];
    }
    drawObject3D(tube, blue);
    drawObject3D(tube2, blue);
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    selimut();
    tutup();
    glFlush();
}
void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(20, timer, 0);
}
void Initialize(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-300, 300, -300, 300);
}
int main(int argc, char** argv)
{
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