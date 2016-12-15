#include "RenderGun.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

#ifdef _DEBUG
#include <assert.h>
#endif
#include "SceneManager.h"

static const int ScoreByGeneric = 50;

RenderGun::RenderGun(Mesh* _modelMesh)
: modelMesh(_modelMesh)
{
	
}

RenderGun::~RenderGun()
{
}

void RenderGun::Update(double _dt)
{

}

void RenderGun::Render()
{
	if ((modelMesh))
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

		modelStack.PushMatrix();
		modelStack.Translate(CameraPos.x, CameraPos.y, CameraPos.z);
		//
		modelStack.PushMatrix();
		modelStack.Rotate((CameraRotation.x), right.x, right.y, right.z);
		/*modelStack.Rotate((CameraRotation.y), 0, 1, 0);
		modelStack.Rotate((CameraRotation.x), 1, 0, 0);
		modelStack.Rotate((CameraRotation.z), 0, 0, 1);*/
		//modelStack.Translate(0, 0, -5);
		modelStack.PushMatrix();
		modelStack.Rotate((CameraRotation.y), 0, 1, 0);
		
		modelStack.PushMatrix();
		modelStack.Translate(1, -1, -3);
		modelStack.Scale(scale.x, scale.y, scale.z);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
}

// Set the maxAABB and minAABB
//void RenderGun::SetAABB(Vector3 maxAABB, Vector3 minAABB)
//{
//	this->maxAABB = maxAABB;
//	this->minAABB = minAABB;
//}

RenderGun* Create::Gun(const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	RenderGun* result = new RenderGun(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	//EntityManager::GetInstance()->AddEntity(result);
	return result;
}

bool RenderGun::onNotify(const Vector3 &zeEvent1, const Vector3 &zeEvent2)
{
	CameraPos = zeEvent1;
	CameraRotation = zeEvent2;
	return true;
}

bool RenderGun::onNotify(const Vector3 &zeEvent)
{
	right = zeEvent;
	return true;
}