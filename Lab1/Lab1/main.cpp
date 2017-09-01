#include "main.h"
#include "Circle.h"
#include <limits>
#include <cmath>
const int TIME_INTERVAL = 100;
/**
* Frame buffer that maintains the individual pixel colors in memory
* before they are displayed on the screen.
* This object is a global variable.
*/
FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

void swap(int &x, int &y) {
	int tmp = x;
	x = y;
	y = tmp;
}

/*
* Takes the passed color and converts it to a similar color, but
* darker. For example, the color (1.0f, 0.8f, 0.5f) would be transformed
* to (0.5f, 0.4f, 0.25f).
* The original color is left unchanged.
*/
color darken(color C) {
	return color(1, 1, 1, 1);	// FIX
}

/*
* Returns the quadrant a particular point resides. -1 is returned if
* the point lies on an axis.
*/
int quadrant(float x, float y) {
	int result = -1;
	if (x > 0 && y > 0)
		result = 1;
	else if (x < 0 && y > 0)
		result = 2;
	else if (x < 0 && y < 0)
		result = 3;
	else if (x > 0 && y < 0)
		result = 4;
	return result;
}

/*
* Same as above, but the input is a glm::vec2.
*/
int quadrant(const glm::vec2 &pt) {
	return -100;	// FIX
}

/*
* Returns true if the two numbers have values that are close to
* each other, where close is defined by the constant EPSILON.
*/
bool approximatelyEqual(float a, float b) {
	return std::abs(a - b) < EPSILON;
}

/*
* Returns true if the number is close to zero, where close is
* defined by the constant EPSILON.
*/
bool approximatelyZero(float a) {
	return false;	// FIX
}

/*
* Draws a horizontal line at a height of y from the bottom.
* The line will span, left-to-right, from xLeft to xRight.
*/
void horizontalLine(int xLeft, int xRight, int y, color C) {
	for (int x = xLeft; x <= xRight; x++) {
		frameBuffer.setPixel(x, y, C);
	}
}

/*
* Draws a vertical line.
*/
void verticalLine(int yBotton, int yTop, int x, color C) {
	// FILL IN
}

/*
* Renders a solid 5x5 square, centered at (x, y). The color of the
* square will be C.
*/
void closed5x5Square(int x, int y, color C) {
	for (int r = y - 2; r <= y + 2; r++) {
		horizontalLine(x - 2, x + 2, r, C);
	}
}

/*
* Same as above, but the center point is passes as a glm::vec2.
*/
void closed5x5Square(glm::vec2 centerPt, color C) {
	// FILL IN
}

/*
* Same as above, but the square is a wireframe (not filled in).
* Do this drawing 4 lines.
*/
void open5x5Square(glm::vec2 centerPt, color C) {
	closed5x5Square(10, 10, C);	// FIX
}

enum Direction { NE, SE, SW, NW };	// Northeast, southeast, etc.

/*
* Renders a 45 degree line, starting at the given point. The direction
* of the line is given by "dir".
*/
void diagonalLine(glm::vec2 startPt, int numPixels, Direction dir, color C) {
	closed5x5Square(startPt.x, startPt.y, C);	// FIX
}

/*
* Solid rectangle, having width W and height H.
*/
void closedRectangle(glm::vec2 center, int W, int H, color C) {
	closed5x5Square(center.x, center.y, C);	// FIX
}

/*
* Similar to above, but a wireframe. Do this drawing four lines.
*/
void openRectangle(glm::vec2 center, int W, int H, color C) {
	closed5x5Square(center.x, center.y, C);	// FIX
}

// (x, y) is the corner corresponding to the right triangle. "dir" corresponds
// to the corner in which the right angle is located. For example, NW would look
// like:
// * * * *
// *   *
// * *
// *
void closed45DegreeRightTriangle(int x, int y, int SZ, Direction dir, color C) {
	closed5x5Square(x, y, C);	// FIX
}

/*
* Do this drawing three lines.
*/
void open45DegreeRightTriangle(int x, int y, int SZ, Direction dir, color C) {
	closed5x5Square(x, y, C);	// FIX
}

/**
* Acts as the display function for the window.
*/
static void RenderSceneCB() {
	// Clear the color buffer
	frameBuffer.clearColorAndDepthBuffers(); // Not necessary for ray tracing
	color red(1, 0, 0, 1);
	color green(0, 1, 0, 1);
	color blue(0, 0, 1, 1);
	color cyan(0, 1, 1, 1);
	color magenta(1, 0, 1, 1);
	color yellow(1, 1, 0, 1);
	color black(0, 0, 0, 1);
	color white(1, 1, 1, 1);
	color lightGray(0.7, 0.7, 0.7, 1);
	color darkGray(0.3, 0.3, 0.3, 1);

	// bottom row of figures
	horizontalLine(100, 300, 100, red);
	verticalLine(0, 200, 200, red);
	diagonalLine(glm::vec2(300, 50), 50, NE, cyan);
	diagonalLine(glm::vec2(350, 100), 50, SE, magenta);
	diagonalLine(glm::vec2(450, 100), 50, SW, yellow);
	diagonalLine(glm::vec2(500, 50), 50, NW, white);

	// middle row of figures
	closed5x5Square(glm::vec2(100, 250), yellow);
	closed5x5Square(100, 350, darken(yellow));
	open5x5Square(glm::vec2(100, 300), lightGray);
	closedRectangle(glm::vec2(300, 300), 50, 100, red);
	closedRectangle(glm::vec2(400, 300), 100, 20, green);
	openRectangle(glm::vec2(500, 300), 50, 25, blue);

	// top row of figures
	for (int i = 0; i < 4; i++) {
		open45DegreeRightTriangle(i * 100 + 100, 500, 50, (Direction)i, green);
		closed45DegreeRightTriangle(i * 100 + 100, 500, 50, (Direction)((i+2)%4), blue);
	}

	// Animated objects
	static int i = 0;
	i++;
	if (i % 50 == 0)
		std::cout << ++i << std::endl;
	closed5x5Square(i, i, red);
	closed5x5Square(2 * i, 2 * i, green);

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

  // Register as the "idle" function to have the screen continously
  // repainted. Due to software rendering, the frame rate will not
  // be fast enough to support motion simulation

static void timerFunc(int id) {
	glutPostRedisplay();
	glutTimerFunc(TIME_INTERVAL, timerFunc, id);
} // end timerFunc

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
	glutTimerFunc(TIME_INTERVAL, timerFunc, 0);
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