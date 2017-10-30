#include <string>
#include "BasicIncludesAndDefines.h"
#include "FrameBuffer.h"
#include "ScanConversion.h"

FrameBuffer colorBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
const int N = 50;
const int TIME_INTERVAL = 25;
int windowWidth, windowHeight;

glm::mat3 T(float dx, float dy) {
	return glm::mat3(1, 0, 0, 0, 1, 0, dx, dy, 1);
}

glm::mat3 S(float sx, float sy) {
	return glm::mat3(sx, 0, 0, 0, sy, 0, 0, 0, 1);
}

glm::mat3 R(float deg) {
	float rads = glm::radians(deg);
	float C = std::cos(rads);
	float S = std::sin(rads);
	return glm::mat3(C, S, 0, -S, C, 0, 0, 0, 1);
}

std::vector<glm::vec3> transformVertices(const glm::mat3 &transMatrix, const std::vector<glm::vec3> &vertices) {
	std::vector<glm::vec3> transformedVertices;

	for (unsigned int i = 0; i < vertices.size(); i++) {
		glm::vec3 vt(transMatrix * vertices[i]);
		transformedVertices.push_back(vt);
	}

	return transformedVertices;
}

void renderObject(const glm::mat3 &TM, const std::vector<glm::vec3> &verts, const color &C) {
	std::vector<glm::vec3> transformedVerts = transformVertices(TM, verts);
	drawWirePolygon(transformedVerts, C);
}

std::vector<glm::vec3> createTriangle() {
	std::vector<glm::vec3> result;
	result.push_back(glm::vec3(0, 1, 1));
	result.push_back(glm::vec3(-1, -1, 1));
	result.push_back(glm::vec3(1, -1, 1));
	return result;
}

std::vector<glm::vec3> createSquareCenterOrigin() {
	std::vector<glm::vec3> result;
	result.push_back(glm::vec3(0.5f, 0.5f, 1));
	result.push_back(glm::vec3(-0.5f, 0.5f, 1));
	result.push_back(glm::vec3(-0.5f, -0.5f, 1));
	result.push_back(glm::vec3(0.5f, -0.5f, 1));
	return result;
}

std::vector<glm::vec3> createSquareLowerLeftOrigin() {
	std::vector<glm::vec3> result;
	result.push_back(glm::vec3(0.0f, 0.0f, 1));
	result.push_back(glm::vec3(1.0f, 0.0f, 1));
	result.push_back(glm::vec3(1.0f, 1.0f, 1));
	result.push_back(glm::vec3(0.0f, 1.0f, 1));
	return result;
}

std::vector<glm::vec3> createHexagon() {
	float R = 10;
	float rads = glm::radians(60.0f);
	float S = R * std::sin(rads);
	float C = R * std::cos(rads);

	std::vector<glm::vec3> result;
	result.push_back(glm::vec3(R, 0.0f, 1));
	result.push_back(glm::vec3(C, S, 1));
	result.push_back(glm::vec3(-C, S, 1));
	result.push_back(glm::vec3(-R, 0.0f, 1));
	result.push_back(glm::vec3(-C, -S, 1));
	result.push_back(glm::vec3(C, -S, 1));
	return result;
}

const static float M_PI = std::acos(-1.0f);

std::vector<glm::vec3> createCircle(float R, int DIV) {
	std::vector<glm::vec3> result;
	const float angInc = 2 * M_PI / DIV;
	for (int i = 0; i < DIV; i++) {
		float ang = i * angInc;
		glm::vec3 vertex(R*std::cos(ang), R*std::sin(ang), 1);
		result.push_back(vertex);
	}
	return result;
}

std::vector<glm::vec3> triangle, squareCenterOrigin, squareLowerLeftOrigin, hexagon, circle;

void createObjects() {
	triangle = createTriangle();
	squareCenterOrigin = createSquareCenterOrigin();
	squareLowerLeftOrigin = createSquareLowerLeftOrigin();
	hexagon = createHexagon();
	circle = createCircle(1, 30);
}

static void RenderSceneCB() {
	colorBuffer.clearColorAndDepthBuffers();

	drawAxis();

	renderObject(T(200, 200)*S(100, 100), squareCenterOrigin, blue);
	renderObject(T(0.5f, 0.5f)*T(195, 195)*S(100, 100)*T(-0.5f, -0.5f), squareLowerLeftOrigin, green);
	renderObject(T(0, 100)*S(100, -100), triangle, black);
	renderObject(T(50, 50)*S(50, 20), circle, red);
	renderObject(T(-200, -200)*S(5, 5), hexagon, blue);

	colorBuffer.showColorBuffer();
}

static void ResizeCB(int width, int height) {
	colorBuffer.setFrameBufferSize(width, height);
	windowWidth = width;
	windowHeight = height;
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	createObjects();
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	GLuint world_Window = glutCreateWindow("2D Transformations");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(ResizeCB);

	colorBuffer.setClearColor(white);

	glutMainLoop();

	return 0;
}