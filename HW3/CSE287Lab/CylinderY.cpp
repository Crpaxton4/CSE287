#include "CylinderY.h"
#include "Quadric.h"
#include "Disk.h"

CylinderY::CylinderY(const glm::vec3 &position, float rad, float H, const color &material)
	: Surface(material), center(position), radius(rad), height(H) {
}

HitRecord CylinderY::findClosestIntersection(const glm::vec3 &rayOrigin,
										const glm::vec3 &rayDirection) {
	HitRecord hitRecord;
	QuadricParameters params = { 1 / glm::pow(radius, 2), 0, 1 / glm::pow(radius, 2), 0, 0, 0, 0, 0, 0, -1 };
	QuadricSurface quadric(params, center, material);
	hitRecord = quadric.findClosestIntersection(rayOrigin, rayDirection);

	if (glm::abs(hitRecord.interceptPoint.y - center.y) > height / 2) {
		hitRecord.t = FLT_MAX;
	}

	Disk d0(glm::vec3(center.x, center.y + (height / 2), center.z), glm::vec3(0, 1, 0), radius, material);
	Disk d1(glm::vec3(center.x, center.y - (height / 2), center.z), glm::vec3(0, 1, 0), radius, material);

	HitRecord d0Hit = d0.findClosestIntersection(rayOrigin, rayDirection);
	HitRecord d1Hit = d1.findClosestIntersection(rayOrigin, rayDirection);

	hitRecord.t = glm::min(hitRecord.t, glm::min(d0Hit.t, d1Hit.t));

	return hitRecord;
}
