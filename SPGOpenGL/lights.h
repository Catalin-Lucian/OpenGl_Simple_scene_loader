#ifndef LIGHTS_H
#define LIGHTS_H

#include "Includers.h"

using namespace glm;

//light distance:7 / 13 / 20 / 32 / 50 / 65 / 100 / 160 / 200 / 324 / 600 / 3250 

class DirLight
{
public:
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;


	DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
		this->direction = direction;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	void setDirection(glm::vec3 direction) {
		this->direction = direction;
	}

	void render(Shader shader) {
		shader.setVec3("dirLight.direction", direction);
		shader.setVec3("dirLight.ambient", ambient);
		shader.setVec3("dirLight.diffuse", diffuse);
		shader.setVec3("dirLight.specular", specular);
	}
};

class PointLight
{
public:
	glm::vec3 position;


	float linear;
	float quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, int distance = 50) {
		this->position = position;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;

		setLightDistance(distance);
	}

	void setPosition(glm::vec3 pos) {
		position = pos;
	}

	void render(Shader shader,int index) {
		
		shader.setVec3("pointLights[" + std::to_string(index) + "].position", position);
		shader.setVec3("pointLights[" + std::to_string(index) + "].ambient", ambient);
		shader.setVec3("pointLights[" + std::to_string(index) + "].diffuse", diffuse);
		shader.setVec3("pointLights[" + std::to_string(index) + "].specular", specular);
		shader.setFloat("pointLights[" + std::to_string(index) + "].linear", linear);
		shader.setFloat("pointLights[" + std::to_string(index) + "].quadratic", quadratic);
	}

	void setLightDistance(int distance) {
		switch (distance)
		{
		case 3250:
			linear = 0.0014f;
			quadratic = 0.000007f;
			break;
		case 600:
			linear = 0.007f;
			quadratic = 0.0002f;
			break;
		case 325:
			linear = 0.014f;
			quadratic = 0.0007f;
			break;
		case 200:
			linear = 0.022f;
			quadratic = 0.0019f;
			break;
		case 160:
			linear = 0.027f;
			quadratic = 0.0028f;
			break;
		case 100:
			linear = 0.045f;
			quadratic = 0.0075f;
			break;
		case 65:
			linear = 0.07f;
			quadratic = 0.017f;
			break;
		case 50:
			linear = 0.09f;
			quadratic = 0.032f;
			break;
		case 32:
			linear = 0.14f;
			quadratic = 0.07f;
			break;
		case 20:
			linear = 0.22f;
			quadratic = 0.20f;
			break;
		case 13:
			linear = 0.35f;
			quadratic = 0.44f;
			break;
		case 7:
			linear = 0.7f;
			quadratic = 1.8f;
			break;
		default:
			linear = 0.7f;
			quadratic = 1.8f;
		}
	}
	

};


class SpotLight
{
public:
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	SpotLight(vec3 position, vec3 direction, float cutOff, float outerCutOff, vec3 ambient, vec3 diffuse, vec3 specular, int distance = 50) {
		this->position = position;
		this->direction = direction;

		this->cutOff = cutOff;
		this->outerCutOff = outerCutOff;

		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;

		setLightDistance(distance);
	}
	void render(Shader shader) {

		shader.setVec3("spotLight.position", position);
		shader.setVec3("spotLight.direction", direction);
		shader.setVec3("spotLight.ambient", ambient);
		shader.setVec3("spotLight.diffuse", diffuse);
		shader.setVec3("spotLight.specular", specular);
		shader.setFloat("spotLight.linear", linear);
		shader.setFloat("spotLight.quadratic", quadratic);
		shader.setFloat("spotLight.cutOff", cutOff);
		shader.setFloat("spotLight.outerCutOff", outerCutOff);
	}

	void setLightDistance(int distance) {
		switch (distance)
		{
		case 3250:
			linear = 0.0014f;
			quadratic = 0.000007f;
			break;
		case 600:
			linear = 0.007f;
			quadratic = 0.0002f;
			break;
		case 325:
			linear = 0.014f;
			quadratic = 0.0007f;
			break;
		case 200:
			linear = 0.022f;
			quadratic = 0.0019f;
			break;
		case 160:
			linear = 0.027f;
			quadratic = 0.0028f;
			break;
		case 100:
			linear = 0.045f;
			quadratic = 0.0075f;
			break;
		case 65:
			linear = 0.07f;
			quadratic = 0.017f;
			break;
		case 50:
			linear = 0.09f;
			quadratic = 0.032f;
			break;
		case 32:
			linear = 0.14f;
			quadratic = 0.07f;
			break;
		case 20:
			linear = 0.22f;
			quadratic = 0.20f;
			break;
		case 13:
			linear = 0.35f;
			quadratic = 0.44f;
			break;
		case 7:
			linear = 0.7f;
			quadratic = 1.8f;
			break;
		default:
			linear = 0.7f;
			quadratic = 1.8f;
		}
	}
};



#endif 

