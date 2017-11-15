#include <algorithm>
#include <cmath>
#include "Lab.h"

/********************** GLOBALS ******************************/
const color red(1.0f, 0.0f, 0.0f, 1.0f);
const color green(0.0f, 1.0f, 0.0f, 1.0f);
const color blue(0.0f, 0.0f, 1.0f, 1.0f);
const color cyan(0.0f, 1.0f, 1.0f, 1.0f);
const color magenta(1.0f, 0.0f, 1.0f, 1.0f);
const color yellow(1.0f, 1.0f, 0.0f, 1.0f);
const color black(0.0f, 0.0f, 0.0f, 1.0f);
const color gray(0.6f, 0.6f, 0.6f, 1.0f);
const color white(1.0f, 1.0f, 1.0f, 1.0f);

std::vector<LightSource*> lights;

const int WIDTH = 10;
const float WIDTH2 = WIDTH / 2.0f;
const int DIV = 20;

// Frame buffer that contains both the color and depth buffers
FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

// Objects in the scene

std::vector<VertexData> planeVertices;
std::vector<VertexData> extrusion1Vertices;
std::vector<VertexData> extrusion2Vertices;

/********************** END GLOBALS ******************************/
glm::mat4 T(float dx, float dy, float dz) {
	return glm::translate(glm::vec3(dx, dy, dz));
}

glm::mat4 S(float sx, float sy, float sz) {
	return glm::scale(glm::vec3(sx, sy, sz));
}

glm::mat4 S(float scale) {
	return S(scale, scale, scale);
}
glm::mat4 Rx(float rads) {
	return glm::rotate(rads, glm::vec3(1, 0, 0));
}

glm::mat4 Ry(float rads) {
	return glm::rotate(rads, glm::vec3(0, 1, 0));
}

glm::mat4 Rz(float rads) {
	return glm::rotate(rads, glm::vec3(0, 0, 1));
}

void renderObjects() {
	static glm::mat4 planeTM = T(0.0f, 0.0f, 0.0f);
	PerVertex::modelingTransformation = planeTM;
	PerVertex::processTriangleVertices(planeVertices);

	glm::mat4 extrusion1TM = T(-WIDTH/2.0f, 1, 0)*Ry(glm::radians(90.0f))*S(0.25f);
	PerVertex::modelingTransformation = extrusion1TM;
	PerVertex::processTriangleVertices(extrusion1Vertices);

	glm::mat4 extrusion2TM = T(WIDTH / 2.0f, 1, 0)*Ry(glm::radians(-90.0f))*S(0.25f);
	PerVertex::modelingTransformation = extrusion2TM;
	PerVertex::processTriangleVertices(extrusion2Vertices);
}

/**
* Acts as the display function for the window.
*/
static void RenderSceneCB() {
	glm::vec3 eyePosition(0, 1, 0);
	glm::vec3 focusPt = eyePosition + glm::vec3(1, 0, 0);
	glm::vec3 up(0, 1, 0);
	PerVertex::viewingTransformation = glm::lookAt(eyePosition, focusPt, up);
	frameBuffer.clearColorAndDepthBuffers();
	renderObjects();
	frameBuffer.showColorBuffer();
}

static void ResizeCB(int width, int height) {
	frameBuffer.setFrameBufferSize(width, height);

	PerVertex::xViewportMin = 0;
	PerVertex::yViewportMin = 0;
	PerVertex::xViewportMax = (float)width;
	PerVertex::yViewportMax = (float)height;

	PerVertex::projectionTransformation =
		glm::perspective(glm::radians(90.0f), ((float)PerVertex::xViewportMax - PerVertex::xViewportMin) /
		((float)PerVertex::yViewportMax - PerVertex::yViewportMin), 0.1f, 50.0f);

	PerVertex::viewportTransformation =
		glm::translate(glm::vec3(PerVertex::xViewportMin, PerVertex::yViewportMin, 0.0f)) *
		glm::scale(glm::vec3((float)(PerVertex::xViewportMax - PerVertex::xViewportMin) /
		(PerVertex::xNdcMax - PerVertex::xNdcMin),
			(float)(PerVertex::yViewportMax - PerVertex::yViewportMin) /
			(PerVertex::yNdcMax - PerVertex::yNdcMin), 1.0f)) *
		glm::translate(glm::vec3(-PerVertex::xNdcMin, -PerVertex::yNdcMin, 0.0f));

	glutPostRedisplay();
}

static void KeyboardCB(unsigned char key, int x, int y) {
	switch (key) {

	case('p'): case('P'):
		break;
	case('r'): case('R'):
		break;
	case('c'): case('C'):
		break;
	case(27):
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();

} // end KeyboardCB

static void SpecialKeysCB(int key, int x, int y) {
	static const float rotateInc = glm::radians(10.0f);
	switch (key) {
	case(GLUT_KEY_LEFT):
		break;
	case(GLUT_KEY_RIGHT):
		break;
	case(GLUT_KEY_UP):
		break;
	case(GLUT_KEY_DOWN):
		break;
	}

	glutPostRedisplay();
}

static void timer(int id) {
	glutTimerFunc(100, timer, 0);
	glutPostRedisplay();
}

std::vector<VertexData> makePlaneVertices() {	// for checkerboard
	const float INC = (float)WIDTH / DIV;
	std::vector<VertexData> verts;
	for (int X = 0; X < DIV; X++) {
		bool isRed = X % 2 == 0;
		for (float Z = 0; Z < DIV; Z++) {
			glm::vec4 V1(-WIDTH / 2.0f + X*INC, 0.0f, -WIDTH / 2 + Z*INC, 1.0f);
			glm::vec4 V2 = V1 + glm::vec4(0.0f, 0.0f, INC, 0.0f);
			glm::vec4 V3 = V1 + glm::vec4(INC, 0.0f, INC, 0.0f);
			glm::vec4 V4 = V1 + glm::vec4(INC, 0.0f, 0.0f, 0.0f);
			color C = isRed ? color(1, 0, 0, 1) : color(1, 1, 1, 1);
			// Square is made up of two triangles
			verts.push_back(VertexData(V1, C));
			verts.push_back(VertexData(V2, C));
			verts.push_back(VertexData(V3, C));

			verts.push_back(VertexData(V3, C));
			verts.push_back(VertexData(V4, C));
			verts.push_back(VertexData(V1, C));

			isRed = !isRed;
		}
	}
	return verts;
}

std::vector<VertexData> makeExtrusionVertices(const std::vector<glm::vec2> &V, const color &C) {
	// FIX THIS
	std::vector<VertexData> verts;
	verts.push_back(VertexData(glm::vec4(0, 0, 0, 1), C));
	verts.push_back(VertexData(glm::vec4(1, 1, 0, 1), C));
	verts.push_back(VertexData(glm::vec4(0, 1, 0, 1), C));
	return verts;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	GLuint world_Window = glutCreateWindow("HW5");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);
	glutTimerFunc(100, timer, 0);

	color clearColor(0.784, 0.784, 1.0, 1.0);
	frameBuffer.setClearColor(clearColor);

	planeVertices = makePlaneVertices();
	std::vector<glm::vec2> face1 = {glm::vec2(-1,-1), glm::vec2(1,-1), 
									glm::vec2(0.5f,2), glm::vec2(-0.5f, 2)};
	extrusion1Vertices = makeExtrusionVertices(face1, blue);

	std::vector<glm::vec2> face2 = { glm::vec2(0,-3), glm::vec2(3,-1), 
									glm::vec2(2, 3), glm::vec2(-0.5f,5),
									glm::vec2(-2,3), glm::vec2(-3, -1) };
	extrusion2Vertices = makeExtrusionVertices(face2, magenta);

	glutMainLoop();

	return 0;
}
