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

const float SPEED_FACTOR = 0.1f;

float view_azimuth = 0;
float view_elevation = 0;

bool pause = false;

glm::vec3 eye_pos(0, 1, 0);
glm::vec3 eye_movement_vector(1, 0, 0);
glm::vec3 eye_view_vector = sphereical_to_cartesian(view_azimuth, view_elevation);

// Frame buffer that contains both the color and depth buffers
FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

// Objects in the scene

std::vector<VertexData> planeVertices;
std::vector<VertexData> extrusion1Vertices;
std::vector<VertexData> extrusion2Vertices;

/********************** END GLOBALS ******************************/

void update_azimuth(const float radians) {
	/*
	if (view_azimuth + radians >= glm::radians(90.0f) && radians > 0) {
		return;
	}

	if (view_azimuth + radians <= glm::radians(-90.0f) && radians < 0) {
		return;
	}

	*/
	view_azimuth += radians;

	std::cout <<  "Azimuth : " << view_azimuth << std::endl;

}

void update_elevation(const float radians) {

	/*

	if (view_elevation + radians >= glm::radians(90.0f) && radians > 0) {
		return;
	}

	if (view_elevation + radians <= glm::radians(-60.0f) && radians < 0) {
		return;
	}

	*/

	view_elevation += radians;
	
	std::cout << "Elevation : " << view_elevation << std::endl;
}

glm::vec3 sphereical_to_cartesian(float az, float el) {
	glm::vec3 cart(0, 0, 0);

	cart.x = glm::cos(el)*glm::sin(az);
	cart.y = glm::sin(el);
	cart.z = glm::cos(el)*glm::cos(az);

	return glm::normalize(cart);
}


// returns (az, el)
glm::vec2 cartesian_to_sphreical(glm::vec3 cart) {
		
	float az = std::atan2(cart.x, cart.z);
	float el = std::atan2(cart.y, 1);

	return glm::vec2(az, el);
}

void move_eye(glm::vec3 movement_vector) {
	if (pause) {
		return; // Don't move when paused
	}

	glm::vec3 new_eye_pos = eye_pos + eye_movement_vector * SPEED_FACTOR;
	
//	if (new_eye_pos.x <= -WIDTH / 2 || new_eye_pos.x >= WIDTH / 2 || new_eye_pos.z <= -WIDTH / 2 || new_eye_pos.z >= WIDTH / 2) {
//		return; // Don't move if moving will put eye off of the board
//	}
//	else {
		eye_pos = new_eye_pos; // Eye moves along the movement vector
		eye_pos.y = 1; // Keep the eye height off the ground constant
//	}
	

}



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

	glm::vec3 focusPt = eye_pos + eye_view_vector;
	glm::vec3 up(0, 1, 0); // TODO: Update up vector based on eye point and focus point
	PerVertex::viewingTransformation = glm::lookAt(eye_pos, focusPt, up);
	frameBuffer.clearColorAndDepthBuffers();
	renderObjects();
	frameBuffer.showColorBuffer();

	move_eye(eye_movement_vector);
	 
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
		pause = !pause;
		break;

	case('r'): case('R'):
		eye_view_vector = glm::normalize(eye_movement_vector); // Align the focus with the current movement
		glm::vec2 sph = cartesian_to_sphreical(eye_view_vector);
		
		view_azimuth = sph.x;
		view_elevation = sph.y;
		break;

	case('c'): case('C'):
		eye_movement_vector = glm::normalize(eye_view_vector); // Aligns themovement with the view
		break;

	case(27): //ESC
		glutLeaveMainLoop();
		break;

	}

	glutPostRedisplay();

} // end KeyboardCB

static void SpecialKeysCB(int key, int x, int y) {
	static const float rotateInc = glm::radians(10.0f);

	switch (key) {
	case(GLUT_KEY_LEFT):
		update_azimuth(glm::radians(10.0f)); // decreace azimuth by 10 deg
		break;

	case(GLUT_KEY_RIGHT):
		update_azimuth(glm::radians(-10.0f)); // increace azimuth by 10 deg
		break;

	case(GLUT_KEY_UP):
		update_elevation(glm::radians(10.0f)); // increace elevation by 10 deg
		break;

	case(GLUT_KEY_DOWN):
		update_elevation(glm::radians(-10.0f)); // decreace elevation by 10 deg
		break;

	}

	eye_view_vector = sphereical_to_cartesian(view_azimuth, view_elevation);

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
