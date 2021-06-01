#ifndef INCLUDERS_H
#define INCLUDERS_H


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
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <reactphysics3d/reactphysics3d.h> 


#include "objloader.hpp"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "lights.h"
#include "CollisionCamera.h"
#include "sceneManager.h"
#include "PhysicsObject.h"


using namespace reactphysics3d;
using namespace std;
using namespace glm;


#endif // !INCLUDERS_H

