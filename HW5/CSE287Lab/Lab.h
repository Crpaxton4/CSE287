#pragma once

#include "BasicIncludesAndDefines.h"
#include "FrameBuffer.h"
#include "PerVertexOperations.h"
#include "Lights.h"


/**
* Declarations for functions that acte as "call backs" for various
* events. FreeGLUT commands are used to register these functions
* as event handlers.
*/

void update_azimuth(const float radians);
void update_elevation(const float radians);
glm::vec3 sphereical_to_cartesian(float az, float el);

/**
* Acts as the display function for the window. It is called every
* the window needs to be repainted.
*/
static void RenderSceneCB();

/**
* Resets the graphics viewport limits for full window rendering 
* each time the window is resized.
* 
* @param width of the window in pixels
* @param height of the window in pixels
*/
static void ResizeCB(int width, int height);

/**
* If registered as the "idle" function, this method will continously as the
* OS to repaint the window.
*/
static void animate();
