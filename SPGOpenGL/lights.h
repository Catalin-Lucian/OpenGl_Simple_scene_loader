#ifndef LIGHTS_H
#define LIGHTS_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "shader.h"

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
		shader.set3Float("dirLight.direction", direction);
		shader.set3Float("dirLight.ambient", ambient);
		shader.set3Float("dirLight.diffuse", diffuse);
		shader.set3Float("dirLight.specular", specular);
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

	PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, int distace = 50) {
		this->position = position;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;


		switch (distace)
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

	void setPosition(glm::vec3 pos) {
		position = pos;
	}

	void render(Shader shader,int index) {
		
		shader.set3Float("pointLights[" + std::to_string(index) + "].position", position);
		shader.set3Float("pointLights[" + std::to_string(index) + "].ambient", ambient);
		shader.set3Float("pointLights[" + std::to_string(index) + "].diffuse", diffuse);
		shader.set3Float("pointLights[" + std::to_string(index) + "].specular", specular);
		shader.setFloat("pointLights[" + std::to_string(index) + "].linear", linear);
		shader.setFloat("pointLights[" + std::to_string(index) + "].quadratic", quadratic);
	}
	

};



#endif 

