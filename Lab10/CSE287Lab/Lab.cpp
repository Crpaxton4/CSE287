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

const GLint FRAMES_PER_SECOND = 60; // Desired maximum number of frames per second
const GLint FRAME_INTERVAL = 1000 / FRAMES_PER_SECOND; // Interval in milliseconds between frames

// Frame buffer that contains both the color and depth buffers
FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

// Objects in the scene

std::vector<VertexData> pyramidVerts;
std::vector<VertexData> diskVertices;
std::vector<VertexData> planeVertices;
std::vector<VertexData> coneVertices;

// Global to hold the rotation angle of objects in the scene
float angle = 0;

/********************** END GLOBALS ******************************/
glm::mat4 T(float dx, float dy, float dz) {
	return glm::translate(glm::vec3(dx, dy, dz));
}

glm::mat4 S(float sx, float sy, float sz) {
	return glm::scale(glm::vec3(sx, sy, sz));
}

glm::mat4 S(float scale) {
	// Fix this.
	return glm::mat4();
}
glm::mat4 Rx(float rads) {
	// Fix this.
	return glm::mat4();
}

glm::mat4 Ry(float rads) {
	// Fix this.
	return glm::mat4();
}

glm::mat4 Rz(float rads) {
	// Fix this.
	return glm::mat4();
}

void renderObjects() {
	glm::mat4 I;

	static glm::mat4 planeTM = T(0.0f, -3.0f, 0.0f);
	PerVertex::processTriangleVertices(planeTM, planeVertices);

	// Will need to make changes below
	glm::mat4 pyramidTM = glm::mat4();
	PerVertex::processTriangleVertices(pyramidTM, pyramidVerts);
	
	glm::mat4 pyramidTM2 = glm::mat4();
	PerVertex::processTriangleVertices(pyramidTM2, pyramidVerts);

	glm::mat4 diskTM = glm::mat4();
	PerVertex::processTriangleVertices(diskTM, diskVertices);

	glm::mat4 coneTM = glm::mat4();
	PerVertex::processTriangleVertices(coneTM, coneVertices);

	glm::mat4 coneTM2 = glm::mat4();
	PerVertex::processTriangleVertices(coneTM2, coneVertices);
}

/**
* Acts as the display function for the window.
*/
static void RenderSceneCB() {
	// time in milliseconds of last frame render
	static GLint lastRenderTime = 0;

	int currentTime = glutGet(GLUT_ELAPSED_TIME); // Get current time
	int elapsedTime = currentTime - lastRenderTime; // Calc time since last frame

	// Check if enough time has elapsed since the last render.
	if (elapsedTime >= FRAME_INTERVAL) {
		// Save time for this frame render
		lastRenderTime = currentTime;

		// Clear the color buffer
		frameBuffer.clearColorAndDepthBuffers();

		angle += glm::radians(5.0f);		// increment by 5 degrees

		// Draw the objects in the scene
		renderObjects();

		// Display the color buffer
		frameBuffer.showColorBuffer();
	}
} // end RenderSceneCB

// Reset viewport limits for full window rendering each time the window is resized.
// This function is called when the program starts up and each time the window is 
// resized.
static void ResizeCB(int width, int height) {
	// Size the color buffer to match the window size.
	frameBuffer.setFrameBufferSize(width, height);

	// Set rendering window parameters for viewport transfomation
	PerVertex::xViewportMin = 0;
	PerVertex::yViewportMin = 0;
	PerVertex::xViewportMax = (float)width;
	PerVertex::yViewportMax = (float)height;

	// Create a perspective projection matrix. glm::perspective vertical field of view is specifed in degrees.
	PerVertex::projectionTransformation =
		glm::perspective(45.0f, ((float)PerVertex::xViewportMax - PerVertex::xViewportMin) /
		((float)PerVertex::yViewportMax - PerVertex::yViewportMin), 1.0f, 50.0f);

	// Set viewport transformation
	PerVertex::viewportTransformation =
		glm::translate(glm::vec3(PerVertex::xViewportMin, PerVertex::yViewportMin, 0.0f)) *
		glm::scale(glm::vec3((float)(PerVertex::xViewportMax - PerVertex::xViewportMin) /
		(PerVertex::xNdcMax - PerVertex::xNdcMin),
			(float)(PerVertex::yViewportMax - PerVertex::yViewportMin) /
			(PerVertex::yNdcMax - PerVertex::yNdcMin), 1.0f)) *
		glm::translate(glm::vec3(-PerVertex::xNdcMin, -PerVertex::yNdcMin, 0.0f));

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end ResizeCB

// Responds to 'f' and escape keys. 'f' key allows 
// toggling full screen viewing. Escape key ends the
// program. Allows lights to be individually turned on and off.
static void KeyboardCB(unsigned char key, int x, int y) {
	switch (key) {

	case('f'): case('F'): // 'f' key to toggle full screen
		glutFullScreenToggle();
		break;
	case(27): // Escape key
		glutLeaveMainLoop();
		break;
	default:
		std::cout << key << " key pressed." << std::endl;
	}

	glutPostRedisplay();

} // end KeyboardCB
// Responds to presses of the arrow keys
static void SpecialKeysCB(int key, int x, int y) {
	static const float rotateInc = glm::radians(20.0f);
	switch (key) {
	case(GLUT_KEY_RIGHT):
		break;
	case(GLUT_KEY_LEFT):
		break;
	case(GLUT_KEY_UP):
		break;
	case(GLUT_KEY_DOWN):
		break;
	default:
		std::cout << key << " key pressed." << std::endl;
	}

	glutPostRedisplay();

} // end SpecialKeysCB
// Register as the "idle" function to have the screen continously
// repainted. Due to software rendering, the frame rate will not
// be fast enough to support motion simulation
static void animate() {
	glutPostRedisplay();
} // end animate

std::vector<VertexData> makePyramidVertices(color pyramidColor, float width = 1.0f, float height = 1.0f) {
	std::vector<VertexData> pyramidVertices;

	// Set vertex locations for a pyramid

	// Positive Z face
	float H2 = height / 2;
	float W2 = width / 2;

	pyramidVertices.push_back(VertexData(glm::vec4(0, H2, 0, 1), red));
	pyramidVertices.push_back(VertexData(glm::vec4(-W2, -H2, W2, 1), red));
	pyramidVertices.push_back(VertexData(glm::vec4(W2, -H2, W2, 1), red));

	// Need to add more triangles below.

	return pyramidVertices;

} // end makePyramidVertices

std::vector<VertexData> makeDiskVertices(color C, int DIV) {
	std::vector<VertexData> verts;
	// Finish this.
	return verts;
}
std::vector<VertexData> makeConeVertices(color C, int DIV) {
	std::vector<VertexData> verts;
	// Finish this.
	return verts;
}
std::vector<VertexData> makePlaneVertices() {
	// No changes needed here.
	const float W = 8.0f;
	std::vector<VertexData> verts;
	// Red Triangles
	verts.push_back(VertexData(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), red));
	verts.push_back(VertexData(glm::vec4(-W / 2.0f, 0.0f, -W / 2.0f, 1.0f), red));
	verts.push_back(VertexData(glm::vec4(-W / 2.0f, 0.0f, W / 2.0f, 1.0f), red));
	verts.push_back(VertexData(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), red));
	verts.push_back(VertexData(glm::vec4(W / 2.0f, 0.0f, W / 2.0f, 1.0f), red));
	verts.push_back(VertexData(glm::vec4(W / 2.0f, 0.0f, -W / 2.0f, 1.0f), red));
	// Green Triangles
	verts.push_back(VertexData(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), green));
	verts.push_back(VertexData(glm::vec4(W / 2.0f, 0.0f, -W / 2.0f, 1.0f), green));
	verts.push_back(VertexData(glm::vec4(-W / 2.0f, 0.0f, -W / 2.0f, 1.0f), green));
	verts.push_back(VertexData(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), green));
	verts.push_back(VertexData(glm::vec4(-W / 2.0f, 0.0f, W / 2.0, 1.0f), green));
	verts.push_back(VertexData(glm::vec4(W / 2.0f, 0.0f, W / 2.0f, 1.0f), green));
	return verts;
}

// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.
int main(int argc, char** argv) {
	// freeGlut and Window initialization ***********************

	// initialize random seed
	srand((unsigned int)time(NULL));

	// Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
	glutInit(&argc, argv);

	// Set the initial display mode.
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

	// Set the initial window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create a window using a string and make it the current window.
	GLuint world_Window = glutCreateWindow("CSE 287 Lab 8 Modeling Tranformations");

	// Indicate to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Callback for window redisplay
	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);
	glutIdleFunc(animate);

	// Attach menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Request that the window be made full screen
	//glutFullScreenToggle();

	color clearColor(0.784, 0.784, 1.0, 1.0);

	// Set red, green, blue, and alpha to which the color buffer is cleared.
	frameBuffer.setClearColor(clearColor);

	// ************* Object vertex coordinate initialization ***********************

	// Make a red pyramid
	pyramidVerts = makePyramidVertices(red);
	diskVertices = makeDiskVertices(green, 10);
	planeVertices = makePlaneVertices();
	coneVertices = makeConeVertices(blue, 10);

	// Set vertex locations for a plane that is composed o four triangles.

	// Set the initial viewing tranformation for the scene
	PerVertex::viewingTransformation = glm::translate(glm::vec3(0.0f, 0.0f, -12.0));

	// Enter the GLUT main loop. Control will not return until the window is closed.
	glutMainLoop();

	return 0;
} // end main
