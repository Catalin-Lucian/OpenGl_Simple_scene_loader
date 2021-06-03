#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "includers.h"
#include <reactphysics3d/engine/Timer.h>
#include "PhysicsObject.h"
#define PI glm::pi<float>()
class SceneManager
{
public:

	// ------------------------ state variables --------------------
	reactphysics3d::PhysicsCommon physicsCommon;
	reactphysics3d::PhysicsWorld * world;
	CollisionCamera camera = CollisionCamera(vec3(0.0f, 30.0f, 50.0f));


	// ----------------------- models ------------------------------

	vector<Model> models;
	CollisionOBJ mainobj;
	vector<CollisionOBJ*> objects;

	SceneManager() {
		world = physicsCommon.createPhysicsWorld();
	};

	void SetMainObject(string path) {
		AddModel(path);
		mainobj.SetModel(&models[0]);
		mainobj.SetBody(world);
		mainobj.SetCollider();

	}

	void AddObject(string path) {
		AddModel(path);
		CollisionOBJ* temp = new CollisionOBJ;
		temp->SetModel(&models[models.size() - 1]);
		temp->SetBody(world);
		temp->SetCollider();

		objects.push_back(temp);
	}

	void InitScene() {
		camera.SetBody(world);
		camera.SetCollider(physicsCommon.createSphereShape(0.5));
		//world->testOverlap(camera.body,camera.cameraEvent);
	}

	void AddModel(string path) {
		Model model;
		model.init(path);
		models.push_back(model);
	}

	void Draw(Shader& shader) {

		int cnt = 0;
		for (Model model : models) {
			if( cnt != 0)
				shader.setMat4("model", objects[cnt-1]->mvp);
			model.Draw(shader);
			cnt++;
		}
	}
};








#endif // !SCENEMANAGER_H
