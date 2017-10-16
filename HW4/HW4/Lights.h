#pragma once

#include <iostream>
#include "BasicIncludesAndDefines.h"
#include "HitRecord.h"
#include "Surface.h"

HitRecord findIntersection(const glm::vec3 &e, const glm::vec3 &d, const std::vector<std::shared_ptr<Surface>> & surfaces);

struct PositionalLight {
	bool isOn;
	glm::vec3 lightPosition;
	virtual color illuminate(const HitRecord &hit, const glm::vec3 &eyeVector, bool inShadow) const {
		return !isOn || inShadow ? color(0,0,0,1) : hit.material.materialColor;
	}
	PositionalLight(const glm::vec3 &pos)
		: lightPosition(pos) {
	}
	friend std::ostream &operator << (std::ostream &os, const PositionalLight &pl);
};


struct SpotLight : public PositionalLight {
	SpotLight(const glm::vec3 &position)
		: PositionalLight(position) {
	}
	virtual color illuminate(const HitRecord &hit, const glm::vec3 &eyeVector, bool inShadow) const {
		return !isOn || inShadow ? color(0, 0, 0, 1) : hit.material.materialColor;
	}
	friend std::ostream &operator << (std::ostream &os, const SpotLight &pl);
};


