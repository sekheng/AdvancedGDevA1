#ifndef RENDER_GUN_H
#define RENDER_GUN_H

#include "EntityBase.h"
#include <string>
//#include "Collider/Collider.h"


class Mesh;

class RenderGun : public EntityBase
{
public:
	RenderGun(Mesh* _modelMesh);
	virtual ~RenderGun();

	virtual void Update(double _dt);
	virtual void Render();

	// Set the maxAABB and minAABB
	//void SetAABB(Vector3 maxAABB, Vector3 minAABB);

	virtual bool onNotify(const Vector3 &zeEvent1, const Vector3 &zeEvent2);
	virtual bool onNotify(const Vector3 &zeEvent);
	//bool isVisable()

protected:
	Mesh* modelMesh;
	
	Vector3 CameraPos, CameraRotation, right;

};

namespace Create
{
	RenderGun* Gun(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};

#endif // GENERIC_ENTITY_H