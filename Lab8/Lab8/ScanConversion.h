#pragma once

#include "BasicIncludesAndDefines.h"
#include "FrameBuffer.h"

void drawAxis();
void drawLine(const glm::vec3 &A, const glm::vec3 &B, const color &rgba);
void drawWirePolygon(const std::vector<glm::vec3> &vertices, const color &rgba);
void drawFilledPolygon(const std::vector<glm::vec3> &vertices, const color &rgba);
