#ifndef COLLISIONCAMERA_H
#define COLLISIONCAMERA_H
#include "includers.h"


class CollisionCamera :public Camera{
public:
	reactphysics3d::CollisionBody* body;
	reactphysics3d::Collider* collider;
	vec3 last_position;

	class CameraEventListener : public reactphysics3d::OverlapCallback {
	public:
		CollisionCamera* camera;

		CameraEventListener(CollisionCamera* camera) {
			this->camera = camera;
		}


		void onOverlap(CallbackData& callbackData) {

			cout << "detected colision on camera " << endl;
		
		}
	};
	CameraEventListener cameraEvent = CameraEventListener(this);


	CollisionCamera(vec3 position = vec3(0.0f, 0.0f, 0.0f)):Camera(position)
	{
		last_position = position;
	}

	void SetBody(reactphysics3d::PhysicsWorld* world) {
		reactphysics3d::Vector3 position(Position.x, Position.y,Position.z);
		reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
		reactphysics3d::Transform transform(position, orientation);
		body = world -> createCollisionBody(transform);
	}

	void SetCollider(reactphysics3d::SphereShape* shape) {
		reactphysics3d::Transform transform = reactphysics3d::Transform::identity();
		collider = body->addCollider(shape, transform);
	}

	void ProcesssMove(MOVEMENT dirrection) {

		this->ProcessKeyboard(dirrection);
		reactphysics3d::Vector3 position(Position.x, Position.y, Position.z);
		reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
		reactphysics3d::Transform newTransform(position, orientation);
		// Move the collision body
		body->setTransform(newTransform);
	}



};





#endif // !COLLISIONCAMERA_H
