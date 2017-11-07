#pragma once

#include <iostream>
#include "BasicIncludesAndDefines.h"
#include "HitRecord.h"
#include "Surface.h"

HitRecord findIntersection(const glm::vec3 &e, const glm::vec3 &d, const std::vector<std::shared_ptr<Surface>> & surfaces);

struct PositionalLight {
	bool isOn;
	glm::vec3 lightPosition;
	Material lightProperties;
	virtual color illuminate(const HitRecord &hit, const glm::vec3 &eyeVector, bool inShadow, const glm::vec3 attenuationParam) const {
		if (!isOn || inShadow) {
			return color(0, 0, 0, 1);

		}else {

			float attenuation = 1 / (attenuationParam.x + attenuationParam.y * hit.t + attenuationParam.z * glm::pow(hit.t, 2));

			glm::vec3 v = -eyeVector;
			v = glm::normalize(v);
			
			glm::vec3 l = lightPosition - hit.interceptPoint;
			l = glm::normalize(l);
			
			glm::vec3 r = ( -l - (2 * glm::dot(-l, hit.surfaceNormal) * hit.surfaceNormal ));
			r = glm::normalize(r);

			float NdotL = glm::max(0.0f, glm::dot(hit.surfaceNormal, l));
			float RdotV = glm::max(0.0f, glm::dot(r, v));
			
			glm::vec4 diffuseComponent((hit.material.materialDiffuse*NdotL) * lightProperties.materialDiffuse);
			diffuseComponent.a = 1.0f; // Fixing alpha component

			glm::vec4 specularComponent(hit.material.materialSpecular*glm::pow(RdotV, hit.material.shininess));
			specularComponent.a = 1.0f; // Fixing alpha component
			
			
			return attenuation * (diffuseComponent + specularComponent);

		}
	}
	
	PositionalLight(const glm::vec3 &pos)
		: lightPosition(pos) {
	}

	PositionalLight(const glm::vec3 &pos, const Material& lightProp)
		: lightPosition(pos), lightProperties(lightProp){
	}

	friend std::ostream &operator << (std::ostream &os, const PositionalLight &pl);
};


struct SpotLight : public PositionalLight {
	float FOV;
	SpotLight(const glm::vec3 &position)
		: PositionalLight(position) {
	}

	SpotLight(const glm::vec3& position, const Material& lightProp) :
	PositionalLight(position, lightProp){

	}

	SpotLight(const glm::vec3& position, const float& FOV, const Material& lightProp)
		: PositionalLight(position), FOV(FOV) {
	}

	virtual color illuminate(const HitRecord &hit, const glm::vec3 &eyeVector, bool inShadow, const glm::vec3 attenuationParam) const {
		
		glm::vec3 lightToIntercept = glm::normalize(hit.interceptPoint - this->lightPosition);
		glm::vec3 unitDown(0, -1, 0); // Spotlight points straight down

		float cosBetween = glm::dot(lightToIntercept, unitDown) / (glm::length(lightToIntercept) * glm::length(unitDown));
		float theta = glm::degrees(glm::acos(cosBetween));

		//std::cout << "Theta: " << theta << std::endl << "FOV: " << FOV << std::endl;

		if (theta <= FOV / 2) {
			return PositionalLight::illuminate(hit, eyeVector, inShadow, attenuationParam);
		}
		else {
			return color(0.0f, 0.0f, 0.0f, 1.0f);
		}

	}

	friend std::ostream &operator << (std::ostream &os, const SpotLight &pl);
};



