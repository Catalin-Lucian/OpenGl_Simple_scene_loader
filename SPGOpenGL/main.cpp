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

#include "objloader.hpp"
#include "shader.h"
#include "camera.h"

#define PI glm::pi<float>()

int width = 700;
int height = 700;


Shader shader;
Camera camera(glm::vec3(0, 12, 30));
float deltaTime=1;


glm::mat4  modelMatrix;
std::stack<glm::mat4> modelStack;

std::vector< glm::vec3 > verticesNormals;
GLuint vaoObj, vboObj;

std::vector< glm::vec3 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec3 > normals;


glm::vec3 lightPos(0, 20000, 0);
glm::vec3 viewPos(10, 12, 30);

float axisRotAngle = PI / 16.0; // unghiul de rotatie in jurul propriei axe
float radius = 2;
float scaleFactor = 0.1;

void printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char*)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

void printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char*)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}


void init()
{
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	bool res = loadOBJ("obj/CenterCitySci-Fi.obj", vertices, uvs, normals);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glewInit();

	verticesNormals = vertices;
	verticesNormals.insert(verticesNormals.end(), normals.begin(), normals.end());

	glGenBuffers(1, &vboObj);
	glBindBuffer(GL_ARRAY_BUFFER, vboObj);
	glBufferData(GL_ARRAY_BUFFER, 3 * verticesNormals.size() * sizeof(float), &verticesNormals[0].x, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoObj);
	glBindVertexArray(vaoObj);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * vertices.size() * sizeof(float)));
	glEnableVertexAttribArray(1);

	shader.init("vertex.vert", "fragment.frag");
}


void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.use();

	glBindVertexArray(vaoObj);

	shader.set3Float("lightPos", viewPos);
	shader.set3Float("viewPos", viewPos);
	
	modelMatrix = glm::mat4(); // matricea de modelare este matricea identitate
	modelMatrix *= glm::rotate(axisRotAngle, glm::vec3(0, 1, 0));
	modelMatrix *= glm::scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	shader.setMat4("modelViewProjectionMatrix", camera.GetProjectionMatrix() * camera.GetViewMatrix() * modelMatrix);

	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
	shader.setMat4("normalMatrix", normalMatrix);
	
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

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
