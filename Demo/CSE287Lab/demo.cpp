#include <iostream>
#include <gl/freeglut.h>
const int W = 600;
const int H = 400;

const int TIME_INTERVAL = 20;
int x = -W;

static void RenderSceneCB() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (float)W / H, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1, 0, 0);
	gluLookAt(x, 100, 100, 0, 0, 0, 0, 1, 0);
	float gray[] = { 0.5,0.5,0.5,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gray);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gray);

	glPushMatrix();
	glTranslatef(50, 0, 0);
	glutSolidTeapot(50);

	glPopMatrix();
	glTranslatef(-150, 0, 0);
	glRotatef(90, 0, 1, 0);
	glutSolidTeapot(100);

	glFlush();
	glutSwapBuffers();
}

static void timerFunc(int id) {
	x++;
	glutPostRedisplay();
	glutTimerFunc(TIME_INTERVAL, timerFunc, id);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);
	glutInitWindowSize(W, H);
	glutCreateWindow("2D Shapes");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutDisplayFunc(RenderSceneCB);
	glClearColor(0, 0, 0, 1);
	glutTimerFunc(TIME_INTERVAL, timerFunc, 0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float white[] = { 1,1,1,1 };
	float darkgray[] = { 0.2,0.2,0.2,1 };
	float overhead[] = { 0,400,0,1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, darkgray);
	glLightfv(GL_LIGHT0, GL_POSITION, overhead);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}