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

int quadrant(const glm::vec2 &pt) {
	return quadrant(pt.x, pt.y);
}

bool approximatelyEqual(float a, float b) {
	return std::abs(a - b) < EPSILON;
}

bool approximatelyZero(float a) {
	// Fill in
	return false;
}

void horizontalLine(int xLeft, int xRight, int y, color C) {
	for (int x = xLeft; x <= xRight; x++) {
		frameBuffer.setPixel(x, y, C);
	}
}

void closed5x5Square(int x, int y, color C) {
	for (int r = y - 2; r <= y + 2; r++) {
		horizontalLine(x - 2, x + 2, r, C);
	}
}

void open5x5Square(int x, int y, color C) {
	// fill in
}

enum Direction { NE, SE, SW, NW };

void diagonalLine(const glm::vec2 &startPt, int numPixels, Direction dir, color C) {
	// Fill in
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
	horizontalLine(150, 300, 50, green);
	closed5x5Square(450, 50, magenta);
	
	// top row of figures
	Circle C(50, 150, 25);
	C.display(frameBuffer, cyan);

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

static void animate() {
	glutPostRedisplay();
} // end animate

static void timerFunc(int id) {
	glutPostRedisplay();
	glutTimerFunc(TIME_INTERVAL, timerFunc, id);
} // end timerFunc

int main(int argc, char** argv) {
	int a = 4;
	int b = -123;
	std::cout << "Before swap: " << a << " " << b << std::endl;
	swap(a, b);
	std::cout << "After swap: " << a << " " << b << std::endl;

	std::cout << "Quadrant: " << quadrant(glm::vec2(-3, 2)) << std::endl;

	std::cout << "Approximately equal: " << approximatelyEqual(3.10f, 3.100001f) << std::endl;
	std::cout << "Approximately equal: " << approximatelyEqual(3.10f, 3.100010f) << std::endl;
	std::cout << "Approximately equal: " << approximatelyEqual(3.10f, 3.100100f) << std::endl;
	std::cout << "Approximately equal: " << approximatelyEqual(3.10f, 3.101000f) << std::endl;

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
	//glutIdleFunc(animate);

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