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

#define PI glm::pi<float>()

int width = 700;
int height = 700;

// ------------ camera and shader -------------------
Shader shader;
Camera camera(glm::vec3(0, 12, 30));

// ------------ models for the scene ------------

Model ourModel;


float axisRotAngle = PI / 16.0; 
float radius = 2;
float scaleFactor = 0.1;


void init()
{
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	//std::cout << "Renderer: " << renderer << std::endl;;
	//std::cout << "OpenGL version supported " << version << std::endl;;

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	stbi_set_flip_vertically_on_load(true);

	glewInit();

	shader.init("vertex.vert", "fragment.frag");
	ourModel.init("D:/Fac_an3_sem2/SPG/Proiect/SPGOpenGL/SPGOpenGL/obj/low_poly_room/Room.obj");
}


void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.use();

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);


	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
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
