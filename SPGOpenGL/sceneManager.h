#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "includers.h"
#include <reactphysics3d/engine/Timer.h>
#include "PhysicsObject.h"

class SceneManager
{
public:

	bool runSim;
	// ------------------------ state variables --------------------
	reactphysics3d::PhysicsCommon physicsCommon;
	reactphysics3d::PhysicsWorld * world;
	CollisionCamera camera = CollisionCamera(vec3(0.0f, 30.0f, 50.0f));


	// ----------------------- models ------------------------------

	vector<Model> models;
	CollisionOBJ mainobj;
	

	// ----------------------- lights ------------------------------


	SceneManager() {
		runSim = true;
		world = physicsCommon.createPhysicsWorld();
	};

	void SetMainObject(string path) {
		AddModel(path);
		mainobj.SetModel(&models[0]);
		mainobj.SetBody(world);
		mainobj.SetCollider();

	}

	void InitScene() {
		camera.SetBody(world);
		camera.SetCollider(physicsCommon.createSphereShape(5.0));
		world->testOverlap(camera.body,camera.cameraEvent);
	}

	void AddModel(string path) {
		Model model;
		model.init(path);
		models.push_back(model);
	}

	void Draw(Shader& shader) {
		for (Model model : models) {
			model.Draw(shader);
		}
	}

	


};







#endif // !SCENEMANAGER_H
