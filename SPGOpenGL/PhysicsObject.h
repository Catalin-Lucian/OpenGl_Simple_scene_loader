#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H
#include "includers.h"

class CollisionOBJ {
public:
	Model* model;
	reactphysics3d::CollisionBody* body;
	reactphysics3d::Collider* collider;

	
	void SetModel(Model* model) {
		this->model = model;
	}
	void SetBody(reactphysics3d::PhysicsWorld* world) {
		reactphysics3d::Vector3 position(0.0, 0.0, 0.0);
		reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
		reactphysics3d::Transform transform(position, orientation);
		body = world -> createCollisionBody(transform);
	}

	void SetCollider() {
		reactphysics3d::Transform transform = reactphysics3d::Transform::identity();
		collider = body->addCollider(CreateShape(), transform);
	}

private:
	reactphysics3d::TriangleVertexArray* CreateArray(Mesh& mesh) {
		int nrTriangles = mesh.indices.size()/3;
		int nrVertices = mesh.vertices.size();

		float* vertices=new float[3 * nrVertices];
		int* indices=new int[3 * nrTriangles];
		for (int i = 0; i < mesh.vertices.size(); i+=3) {
			vertices[i] = mesh.vertices[i].Position.x;
			vertices[i+1] = mesh.vertices[i].Position.y;
			vertices[i+2] = mesh.vertices[i].Position.z;
		}
		for (int i = 0; i < mesh.indices.size(); i++) {
			indices[i] = mesh.indices[i];
		}

		return  new reactphysics3d::TriangleVertexArray(nrVertices, vertices, 3 * sizeof(
				float), nrTriangles,
				indices, 3 * sizeof(int),
			reactphysics3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
			reactphysics3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);
	}


	reactphysics3d::ConcaveMeshShape* CreateShape() {
		reactphysics3d::PhysicsCommon physicsCommon;
		reactphysics3d::TriangleMesh* triangleMesh = physicsCommon.createTriangleMesh();
		for (Mesh mesh:model->meshes)
			triangleMesh->addSubpart(CreateArray(mesh));
		return  physicsCommon.createConcaveMeshShape(triangleMesh);
	}

};

#endif // !PHYSICSOBJECT_H
