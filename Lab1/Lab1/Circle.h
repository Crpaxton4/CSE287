#pragma once

#include <GLM/glm.hpp>
#include "FrameBuffer.h"
#include "BasicIncludesAndDefines.h"

class Circle {
private:
	float x, y, r;
public:
	Circle(float X, float Y, float R);
	void getPoint(float deg, float &outX, float &outY);
	void display(FrameBuffer &frameBuffer, color C);
};