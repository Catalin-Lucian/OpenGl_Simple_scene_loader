#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Includers.h"
#include <reactphysics3d/engine/Timer.h>

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
	CollisionOject mainObject();
	

	// ----------------------- lights ------------------------------


	SceneManager() {
		runSim = true;
		world = physicsCommon.createPhysicsWorld();
	};

	void SetMainObject(string path) {
		AddModel(path);
		mainObject.SetModel(&models[0]);
		mainObject.SetBody(world);
		mainObject.SetCollider();

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

	void RunSIMWorld() {
		long double previousFrameTime = reactphysics3d::Timer::getCurrentSystemTime();
		const float timeStep = 1.0 / 60.0;
		long double accumulator = 0;
		while (runSim)
		{
		
			
			long double currentFrameTime = reactphysics3d::Timer::getCurrentSystemTime();
			long double deltaTime = currentFrameTime - previousFrameTime;
			
			previousFrameTime = currentFrameTime;
			
			accumulator += deltaTime;
			
			while (accumulator >= timeStep) {
				world -> update(timeStep);
				accumulator -= timeStep;
			}
			
		}
	}


};







#endif // !SCENEMANAGER_H
