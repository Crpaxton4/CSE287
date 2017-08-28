#include "Circle.h"
#include "BasicIncludesAndDefines.h"

Circle::Circle(float X, float Y, float R) {
	x = X;
	y = Y;
	r = R;
}

void Circle::getPoint(float deg, float &outX, float &outY) {
	float radians = glm::radians(deg);
	outX = x + r * glm::cos(deg);
	outY = y + r * glm::sin(deg);
}

void Circle::display(FrameBuffer &frameBuffer, color C) {
	const float DEG_STEP = 2.0;
	float x, y;
	for (float deg = 0; deg < 360; deg += DEG_STEP) {
		getPoint(deg, x, y);
		frameBuffer.setPixel((int)x, (int)y, C);
	}
}