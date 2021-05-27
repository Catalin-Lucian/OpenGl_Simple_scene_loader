#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <stack>
#include <cmath>
#include <assimp/texture.h>

#include "objloader.hpp"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "lights.h"


int width = 700;
int height = 700;

// ------------- state variable ----------------------
bool flashOn = false;
bool pointLightOn1 = true;
bool pointLightOn2 = true;
bool directionalLightOn = true;
bool blinn = false;


// ------------ camera and shader -------------------
Shader shader;
Camera camera(glm::vec3(0, 0, 30));

// ------------ models for the scene ------------

Model ourModel;
int nrMeshShown = 1;

// ------------- lights ----------------------

DirLight dirlight(glm::vec3(-0.2f, -1.0f, -0.3f),
	glm::vec3(0.7f, 0.0f, 0.4f),
	glm::vec3(0.4f, 0.4f, 0.4f),
	glm::vec3(0.5f, 0.5f, 0.5f));

PointLight pointlight1(glm::vec3(15.0f, 10.0f, 3.0f),
	glm::vec3(0.2f, 0.2f, 1.0f),
	glm::vec3(0.2f, 0.2f, 0.6f),
	glm::vec3(0.2f, 0.2f, 0.6f),
	200);

PointLight pointlight2(glm::vec3(-15.f, 10.f, 3.0f),
	glm::vec3(1.0f, 0.4f, 0.4f),
	glm::vec3(0.4f, 0.4f, 0.04f),
	glm::vec3(0.4f, 0.4f, 0.4f),
	200);

SpotLight spotLight(camera.Position, 
	camera.Front,
	glm::cos(glm::radians(10.0f)),
	glm::cos(glm::radians(20.0f)),
	vec3(1.0f, 1.0f, 1.0f),
	vec3(1.0f, 1.0f, 1.0f),
	vec3(1.0f, 1.0f, 1.0f),
	200);


void init()
{
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	//std::cout << "Renderer: " << renderer << std::endl;;
	//std::cout << "OpenGL version supported " << version << std::endl;;

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	//stbi_set_flip_vertically_on_load(true);

	glewInit();

	shader.init("vertex.vert", "fragment.frag");
	ourModel.init("D:/Fac_an3_sem2/SPG/Proiect/ProiectPrincipal/SPGOpenGL/obj/book/book.obj");
}


void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.use();
	shader.setVec3("viewPos", camera.Position);
	shader.setBool("flashOn", flashOn);
	shader.setBool("pointLightOn[0]", pointLightOn1);
	shader.setBool("pointLightOn[1]", pointLightOn2);
	shader.setBool("directionalLightOn", directionalLightOn);
	shader.setBool("blinn", blinn);

	if(directionalLightOn)
		dirlight.render(shader);
	if(pointLightOn1)
		pointlight1.render(shader, 0);
	if (pointLightOn2)
		pointlight2.render(shader, 1);

	//Flash light
	if (flashOn)
	{
		spotLight.position = camera.Position;
		spotLight.direction = camera.Front;
		spotLight.render(shader);
	}
		


	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);


	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	
	ourModel.Draw(shader);

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	camera.SetWandH(w, h);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		camera.ProcessKeyboard(FORWARD);
		break;
	case 's':
		camera.ProcessKeyboard(BACKWARD);
		break;
	case 'a':
		camera.ProcessKeyboard(LEFT);
		break;
	case 'd':
		camera.ProcessKeyboard(RIGHT);
		break;
	case 'f':
		if (flashOn)
			flashOn = false;
		else
			flashOn = true;
		break;
	case '1':
		if (pointLightOn1)
			pointLightOn1 = false;
		else
			pointLightOn1 = true;
		break;
	case '2':
		if (pointLightOn2)
			pointLightOn2 = false;
		else
			pointLightOn2 = true;
		break;
	case '0':
		if (directionalLightOn)
			directionalLightOn = false;
		else
			directionalLightOn = true;
		break;
	case '+':
		nrMeshShown += 1;
		break;
	case '-':
		nrMeshShown -= 1;
		break;
	case 'b':
		if (blinn) blinn = false;
		else blinn = true;
		
	};
	glutPostRedisplay(); // cauzeaza redesenarea ferestrei
}


bool visable_mouse=true;
void mouse_press(int button, int state, int x, int y) {
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_UP) 
				if (visable_mouse) {
					glutSetCursor(GLUT_CURSOR_NONE);
					visable_mouse = false;
					glutWarpPointer(width / 2, height / 2);
					
				}
				else {
					glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
					visable_mouse = true;
				}

		default:
			break;
	}
}

void mouse_wheel(int wheel, int direction, int x, int y) {
	camera.ProcessMouseScroll(direction);
	glutPostRedisplay();
}

void mouse_move(int x, int y) {
	if (!visable_mouse) {
		int xoff = x - (width / 2);
		int yoff = y - (height / 2);
		camera.ProcessMouseMovement(xoff, -yoff);
		if (x != width/2 || y != height/2)
			glutWarpPointer(width / 2, height / 2);
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(width, height);
	camera.SetWandH(width, height);
	glutCreateWindow("SPG");
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse_press);
	glutPassiveMotionFunc(mouse_move);
	glutMouseWheelFunc(mouse_wheel);


	glutMainLoop();

	return 0;
}
