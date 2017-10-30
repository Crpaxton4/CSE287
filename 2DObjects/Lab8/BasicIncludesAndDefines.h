#pragma once

#include <iostream>
#include <vector>
#include <time.h>
#include <memory>

#define GLM_SWIZZLE

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 700;

typedef glm::vec4 color;
const float EPSILON = 1.0E-4f;
const float PI = std::acos(-1.0f);

color getRandomColor();

void print(const glm::vec2 & v0);
void print(const glm::vec3 & v0);
void print(const glm::vec4 & v0);
void print(const glm::mat2 & m);
void print(const glm::mat3 & m);
void print(const glm::mat4 & m);

const color red(1, 0, 0, 1);
const color green(0, 1, 0, 1);
const color blue(0, 0, 1, 1);
const color magenta(1, 0, 1, 1);
const color cyan(0, 1, 1, 1);
const color yellow(1, 1, 0, 1);
const color white(1, 1, 1, 1);
const color black(0, 0, 0, 1);