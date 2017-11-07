#include "RayTracer.h"

RayTracer::RayTracer(FrameBuffer & cBuffer, color defaultColor)
	:colorBuffer(cBuffer), defaultColor(defaultColor), recursionDepth(2) {

}

RayTracer::~RayTracer(void) {
}

void RayTracer::setCameraFrame(const glm::vec3 & viewPosition, const glm::vec3 & viewingDirection, glm::vec3 up) {
	// Calculate and set the the w, u, and vdata members and 
	// set the eye data member to the viewing position.
	// TODO

	eye = viewPosition;
	// Squaring up a basis
	w = glm::normalize(-viewingDirection);
	u = glm::normalize(glm::cross(up, w));
	v = glm::normalize(glm::cross(w, u));
} // end setCameraFrame

void RayTracer::calculatePerspectiveViewingParameters(const float verticalFieldOfViewDegrees) {
	// Set the set the topLimit, bottomLimit, rightLimit, 
	// leftLimit, distToPlane, nx, and ny data members
	// TODO
	nx = (float)colorBuffer.getWindowWidth();
	ny = (float)colorBuffer.getWindowHeight();
	topLimit = 1;
	bottomLimit = -topLimit;
	rightLimit = topLimit * nx / ny;
	leftLimit = -rightLimit;
	float rads = glm::radians(verticalFieldOfViewDegrees / 2);
	distToPlane = 1.0f / std::tan(rads);

} // end calculatePerspectiveViewingParameters

void RayTracer::calculateOrthographicViewingParameters(const float viewPlaneHeight) {
	topLimit = fabs(viewPlaneHeight) / 2.0f;

	rightLimit = topLimit * ((float)colorBuffer.getWindowWidth() / colorBuffer.getWindowHeight()); // Set r based on aspect ratio and height of plane

	// Make view plane symetrical about the viewing direction
	leftLimit = -rightLimit;
	bottomLimit = -topLimit;

	// Calculate the distance between pixels in the horizontal and vertical directions
	nx = (float)colorBuffer.getWindowWidth();
	ny = (float)colorBuffer.getWindowHeight();

	distToPlane = 0.0; // Rays start on the view plane

} // end calculateOrthographicViewingParameters

glm::vec2 RayTracer::getProjectionPlaneCoordinates(const int x, const int y) {
	// Page 75
	// Calculate and return the u and v data members as the x an y coordinates
	// of the 
	glm::vec2 uvScalarValues;
	uvScalarValues.x = leftLimit + (rightLimit - leftLimit) * (x + 0.5f) / nx;
	uvScalarValues.y = bottomLimit + (topLimit - bottomLimit) * (y + 0.5f) / ny;
	// TODO
	return uvScalarValues;
}

void RayTracer::setOrthoRayOriginAndDirection(const int x, const int y) {
	glm::vec2 uv = getProjectionPlaneCoordinates(x, y);

	// Page 75
	rayDirection = glm::normalize(-w);
	rayOrigin = eye + uv.x * u + uv.y * v;

} // end setOrthoOriginAndDirection

void RayTracer::setPerspectiveRayOriginAndDirection(const int x, const int y) {
	glm::vec2 uv = getProjectionPlaneCoordinates(x, y);
	rayDirection = glm::normalize(-distToPlane*w + uv.x*u + uv.y*v);
	rayOrigin = eye;
} // end setPerspectiveOriginAndDirection

void RayTracer::raytraceScene(const std::vector<std::shared_ptr<Surface>> & surfaces, const std::vector<std::shared_ptr<PositionalLight>> & lights, const int & antiAlias) {
	this->surfacesInScene = surfaces;
	this->lightsInScene = lights;
	int size = 0;
	for (int x = 0; x < colorBuffer.getWindowWidth(); x++) {
		for (int y = 0; y < colorBuffer.getWindowHeight(); y++) {

			color avgColor(0, 0, 0, 0);

			for (int i = 1; i <= antiAlias; i++) {
				for(int j = 1; j <= antiAlias; j++){
					setPerspectiveRayOriginAndDirection(x + (i/antiAlias) , y + (j/antiAlias) );
					color C = traceIndividualRay(rayOrigin, rayDirection, 1);
					avgColor += C;
				}
			}

			size = glm::pow(antiAlias, 2);

			avgColor.r = avgColor.r / size;
			avgColor.g = avgColor.g / size;
			avgColor.b = avgColor.b / size;
			avgColor.a = avgColor.a / size;

			colorBuffer.setPixel(x, y, avgColor);
			
		}
	}
	// Iterate through each and every pixel in the rendering window
	// TODO

	std::cout << "Rays/Pixel: " << size << std::endl;

} // end raytraceScene

color RayTracer::traceIndividualRay(const glm::vec3 &e, const glm::vec3 &d, int recursionLevel) {
	// Find surface intersection that is closest to 'e' in the direction 'd.'
	// TODO
	float distance = FLT_MAX;
	color result = defaultColor;
	for (unsigned int i = 0; i < surfacesInScene.size(); i++) {
		HitRecord hit = surfacesInScene[i]->findClosestIntersection(e, d);

		if (hit.t != FLT_MAX && hit.t < distance) {
			distance = hit.t;

			if (traceShadowFeeler(hit.interceptPoint + EPSILON*hit.surfaceNormal, lightsInScene[0])) {  //If the shadow feeler hits something between the intersection and the light source
				result = color(0, 0, 0, 1);
			}
			else {  // No object between light source and intersection
				result = hit.material;
			}
			
		}
	}
	return result;

} // end traceRay

// TODO: Put this in the traceIndividualRay(...) function
bool RayTracer::traceShadowFeeler(const glm::vec3 &intersectionPt, const std::shared_ptr<PositionalLight> light) {

	glm::vec3 intersectionToLight = light->lightPosition - intersectionPt;

	float distance = glm::length(intersectionToLight);
	glm::vec3 direction = glm::normalize(intersectionToLight);

	for(int i = 0; i < surfacesInScene.size(); i++) {
		HitRecord hit = surfacesInScene[i]->findClosestIntersection(intersectionPt, direction);

		if (hit.t != FLT_MAX && hit.t < distance) {
			return true;
		}

	}

	return false;
}