#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include <vector>
#include <string>
#include "model.h"
#define PI glm::pi<float>()

using namespace glm;

// posibile miscari 
enum MOVE {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	RotateLX,
	RotateRX,
	RotateLY,
	RotateRY,
	RotateLZ,
	RotateRZ
};


// unstruct care sa retina ce obiecte sau pus in scena
struct modelInstace {
	int type;
	mat4 move;
	mat4 rotate;
	modelInstace(int t, mat4 m,mat4 r) {
		type = t;
		move = m;
		rotate=r;
	}
};


class EditorManager {
public:

	// vectorul cu modele folosit pt desena un tip de obiect incarcat 
	vector<Model> models;

	// vector cu struct care retine ce model a fost pus ,si matricile de transformare
	vector<modelInstace> displayObjs;

	float speed = 0.5f;
	int selected;

	EditorManager() {
	}

	// folosit pt a adauga  modele (clasa care se ocupa de citirea .obj) in vector
	void initModel(string path){
		Model model;
		model.init(path);
		models.push_back(model);
	}


	// cand se apasa 1 ,2,3 ... se adauga in vector structul de instanta 
	void addObject(int type) {
		if (models.size() > type-1) {
			mat4 mvp(1.0f);
			mat4 mvpRot(1.0f);
			displayObjs.push_back(modelInstace(type-1, mvp,mvpRot));
			selected = displayObjs.size()-1;
		}
	}


	// se seteaza matricea de rasformare din vertex numita "mvp", si se deseneaza modelele
	void Draw(Shader& shader) {
		for (int i = 0; i < displayObjs.size(); i++) {
			shader.setMat4("mvp", displayObjs[i].move *displayObjs[i].rotate);
			models[displayObjs[i].type].Draw(shader);
		}
	}


	// schimbare selected pt a muta obiectele 
	void ChangeSelected(MOVE dir) {
		if (dir == LEFT && selected > 0) {
			selected--;
		}
		else if (dir == RIGHT && selected < (displayObjs.size() - 1)) {
			selected++;
		}

	}
		

	// primeste o directie si seteaza matricea de trasnformare a obiectului selectat
	void Moveobject(MOVE direction) {
		vec3 pos(0.0f, 0.0f, 0.0f);
		pos = vec3(displayObjs[selected].move * vec4(pos, 1.0f));
		cout << "x:" << pos.x << " y:" << pos.y << " z:" << pos.z << endl;

		if (direction == FORWARD) {
			displayObjs[selected].move =translate(displayObjs[selected].move,vec3(0.0f, 0.0f, -speed));
		}
		else if (direction == BACKWARD) {
			displayObjs[selected].move = translate(displayObjs[selected].move,vec3(0.0f, 0.0f, speed));
		}
		else if (direction == LEFT) {
			displayObjs[selected].move = translate(displayObjs[selected].move,vec3(-speed, 0.0f, 0.0f));
		}
		else if (direction == RIGHT) {
			displayObjs[selected].move = translate(displayObjs[selected].move,vec3(speed, 0.0f, 0.0f));
		}
		else if (direction == UP) {
			displayObjs[selected].move = translate(displayObjs[selected].move,vec3(0.0f, speed, 0.0f));
		}
		else if (direction == DOWN) {
			displayObjs[selected].move = translate(displayObjs[selected].move,vec3(0.0f, -speed, 0.0f));
		}
		else if (direction == RotateLY) {
			displayObjs[selected].rotate =rotate(displayObjs[selected].rotate,radians(-5.0f), vec3(0.0f, 1.0f, 0.0f));
		}
		else if (direction == RotateRY) {
			displayObjs[selected].rotate =rotate(displayObjs[selected].rotate, radians(5.0f), vec3(0.0f, 1.0f, 0.0f));
		}
		else if (direction == RotateLX) {
			displayObjs[selected].rotate =rotate(displayObjs[selected].rotate , radians(-5.0f), vec3(1.0f, 0.0f, 0.0f));
		}
		else if (direction == RotateRX) {
			displayObjs[selected].rotate =rotate(displayObjs[selected].rotate, radians(5.0f), vec3(1.0f, 0.0f, 0.0f));
		}
		else if (direction == RotateLZ) {
			displayObjs[selected].rotate = rotate(displayObjs[selected].rotate, radians(-5.0f), vec3(0.0f, 0.0f, 1.0f));
		}
		else if (direction == RotateRZ) {
			displayObjs[selected].rotate =rotate(displayObjs[selected].rotate, radians(5.0f), vec3(0.0f, 0.0f, 1.0f));
		}
	}

};



#endif