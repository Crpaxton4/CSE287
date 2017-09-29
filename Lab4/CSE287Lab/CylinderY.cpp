#include "CylinderY.h"
#include "Quadric.h"

CylinderY::CylinderY(const glm::vec3 &position, float rad, float H,
							const color &material)
	: Surface(material), center(position), radius(rad), height(H) {
}

HitRecord CylinderY::findClosestIntersection(const glm::vec3 &rayOrigin,
										const glm::vec3 &rayDirection) {
	HitRecord theHit;
	theHit.t = FLT_MAX;
	return theHit;
}
