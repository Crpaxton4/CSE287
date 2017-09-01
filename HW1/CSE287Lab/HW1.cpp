#include <iostream>
#include <limits>
#include <cmath>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include "FrameBuffer.h"
//#include "HorizontalLine.h"

FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

bool approximatelyEqual(float a, float b) {
	return std::abs(a - b) < EPSILON;
}

bool approximatelyZero(float a) {
	return approximatelyEqual(a, 0);
}

void swap(int &a, int &b) {
	int tmp = a;
	a = b;
	b = tmp;
}

// Take a number of degrees and returns an equivalent value in the range [0, 360).
// Here are some examples:
// normalizeDegrees(40) --> 40
// normalizeDegrees(-350) --> 10
// normalizeDegrees(730) --> 10
float normalizeDegrees(float degrees) {
	return 0;
}

// Write a function (without glm) that calculates the equivalent
// number of degrees that is equivalent to the number of radians.
float rad2deg(float rad) {
	return 0;
}

// Write a function (without glm) that calculates the equivalent
// number of radians that is equivalent to the number of degrees.
float deg2rad(float deg) {
	return 0;
}

// Computes the cosine between two vectors. Do not use any trig
// functions. Check to ensure that the value returned it not
// out of the range [-1, 1] (due to numerical inaccuracies).
float cosBetween(const glm::vec2 &v1, const glm::vec2 &v2) {
	return 0;
}

// Returns the radians between the two vectors. You can use trig here.
float radsBetween(const glm::vec2 &v1, const glm::vec2 &v2) {
	return 0;
}

// Same as above, but uses 3D vectors. You can use trig here.
float radsBetween(const glm::vec3 &v1, const glm::vec3 &v2) {
	return 0;
}

// Computes the angle between the two vectors. You can use
// trig functions for this one.
float degreesBetween(glm::vec2 v1, glm::vec2 v2) {
	return 0;
}

// Returns the dot product of two vectors. Does not use glm
// function calls for this one.
float myDotProduct(const glm::vec3 &v1, const glm::vec3 &v2) {
	return 0;
}

// Suppose that you stand on the position (x1, y1). This function determines
// the direction, in degrees, of the point (x2, y2) relative to (x1, y1).
// The result should be return in the range [0, 360)
// Some examples:
// x1=0, y1=0  x2=10, y2=10     returns 45
// x1=2, y1=2  x2=2, y2=0       returns 270
// x1=1, y1=1  x2=3  y2=0       returns 315
float directionInDegrees(float x1, float y1, float x2, float y2) {
	return 0;
}

// Same as above, but two points are provided as vec2 objects.
float directionInDegrees(const glm::vec2 &pt1, const glm::vec2 &pt2) {
	return 0;
}

// Similar to above but assumes you are standing on the origin.
float directionInDegrees(const glm::vec2 &targetPt) {
	return 0;
}

// Linearly maps a value (x) in one range [xLow, xHigh] to its corresponding
// value in another range [yLow, yHigh]. See page 3 of chapter 2 notes.
// Some examples:
// x = 0  xLow = -1  xHigh = +1  yLow = 2   yHigh = 3    return 2.5 
float map(float x, float xLow, float xHigh, float yLow, float yHigh) {
	return 0;
}

// Same as above but the result is passed out via a reference parameter.
void map(float x, float xLow, float xHigh, float yLow, float yHigh, float &y) {
	y = 0;
}

// Returns the roots (0, 1, or 2 roots) that satisfy the equation:
// Ax^2 + Bx^1 + C = 0
std::vector<float> quadratic(float A, float B, float C) {
	std::vector<float> result;
	return result;
}

// Same as above but roots are passed out as reference parameters.
// The return value of the function is the number of real roots (0, 1, or 2).
int quadratic(float A, float B, float C, float &root1, float &root2) {
	root1 = root2 = std::numeric_limits<float>::quiet_NaN();
	return 0;
}

// Write a function that determines if two vectors are exactly equal.
// Do not use other glm functions for this one.
bool exactVectorEquality(const glm::vec3 &v1, const glm::vec3 &v2) {
	return false;
}

// Returns true if th two vectors are basically the same, where the
// definition of equality is: the angle between the two vectors differ
// by at most 0.1 rads AND the length of the vectors differ by at most EPSILON.
bool approximateVectorEquality(const glm::vec3 &v1, const glm::vec3 &v2) {
	return false;
}

// Same as above but with vec2 inputs
bool approximateVectorEquality(const glm::vec2 &v1, const glm::vec2 &v2) {
	return radsBetween(v1, v2) < 0.1 && approximatelyEqual(glm::length(v1), glm::length(v2));
}

// This function's parameters are three corners of a parallelogram in
// counter-clockwise order. The function computes the area of the parallelogram.
float areaOfParallelogram(const glm::vec3 &v1, const glm::vec3 &v2) {
	return 0;
}

// The parameters are the three corners of a triangle in counter-clockwise order. 
// Computes the area of the triangle formed by the three vertices.
float areaOfTriangle(const glm::vec3 &pt1, const glm::vec3 &pt2, const glm::vec3 &pt3) {
	return 0;
}

// Computes the unit length vector that points from the position described by 
// the first vec3 to the position described by the second vec3.
glm::vec3 pointingVector(const glm::vec3 &pt1, const glm::vec3 &pt2) {
	return glm::vec3(0, 0, 0);
}

// Determines if the basis vectors - u, v, w - form a right handed orthonormal
// basis.
bool isRightHandedOrthoNormalBasis(const glm::vec3 &u, const glm::vec3 &v, const glm::vec3 &w) {
	return false;
}

// Suppose you start at the origin and move in the direction 45 degrees for 10 units.
// Then you move in the heading 270 degrees for 5 units. What is the resting coordinate?
// This function receives two vectors having same length and returns the resulting location.
glm::vec2 endPosition(const std::vector<float> &headingInDegrees, const std::vector<float> &dist) {
	return glm::vec2(0, 0);
}

// Return a color similar to C but darker. The R,G, and B
// components are halved. For example,
// C = (1, 0, 0, 1)    returns (0.5,0,0,1)
// C = (0.8, 0.4, 1.0, 1)    returns (0.4,0.2,0.5,1)
color darken(const color &C) {
	return color(1, 1, 1, 1);
}

/**
* Acts as the display function for the window.
*/
static void RenderSceneCB() {
	// Clear the color buffer
	frameBuffer.clearColorAndDepthBuffers(); // Not necessary for ray tracing
	color red(1, 0, 0, 1);

	frameBuffer.setPixel(200, 200, darken(red));
//	HorizontalLine HL(100, 200, 300);
//	HL.draw(frameBuffer, red);
	
	// Display the color buffer
	frameBuffer.showColorBuffer();
} // end RenderSceneCB

  // Reset viewport limits for full window rendering each time the window is resized.
  // This function is called when the program starts up and each time the window is 
  // resized.
static void ResizeCB(int width, int height) {
	// Size the color buffer to match the window size.
	frameBuffer.setFrameBufferSize(width, height);

	// Signal the operating system to re-render the window
	glutPostRedisplay();
} // end ResizeCB

int main(int argc, char** argv) {
	// Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
	glutInit(&argc, argv);

	// Set the initial display mode.
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);

	// Set the initial window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create a window using a string and make it the current window.
	GLuint world_Window = glutCreateWindow("2D Shapes");

	// Indicate to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Callback for window redisplay
	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(ResizeCB);

	// ColorBuffer initialization ***********************

	// Create a clear color for the window
	color blue(0, 0, 0, 1.0);

	// Set red, green, blue, and alpha to which the color buffer is cleared.
	frameBuffer.setClearColor(blue);

	// Enter the GLUT main loop. Control will not return until the window is closed.
	glutMainLoop();

	// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.
	return 0;

} // end main