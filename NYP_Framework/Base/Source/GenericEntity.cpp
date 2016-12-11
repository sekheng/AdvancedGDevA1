#include "GenericEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "SceneGraph\SceneGraph.h"
#ifdef _DEBUG
#include <assert.h>
#endif

GenericEntity::GenericEntity(Mesh* _modelMesh)
	: modelMesh(_modelMesh)
{
    whichQuadIsIn = nullptr;
}

GenericEntity::~GenericEntity()
{
}

void GenericEntity::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
	//SceneGraph::GetInstance()->Update();
    if (!CheckAABBCollision(this, whichQuadIsIn))
    {
        for (std::vector<EntityBase*>::iterator it = whichQuadIsIn->m_objectList.begin(), end = whichQuadIsIn->m_objectList.end(); it != end; ++it)
        {
            if ((**it) == *this)
            {
                whichQuadIsIn->m_objectList.erase(it);
                break;
            }
        }
        whichQuadIsIn->previousQuad->onNotify(*this);
        whichQuadIsIn = whichQuadIsIn->previousQuad;
        return;
    }

}

void GenericEntity::Render()
{
    if (modelMesh)
    {
        MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
        modelStack.PushMatrix();
        modelStack.Translate(position.x, position.y, position.z);
        modelStack.Scale(scale.x, scale.y, scale.z);
        RenderHelper::RenderMesh(modelMesh);
        modelStack.PopMatrix();
    }
}

// Set the maxAABB and minAABB
void GenericEntity::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}

GenericEntity* Create::Entity(	const std::string& _meshName, 
								const Vector3& _position,
								const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	//EntityManager::GetInstance()->AddEntity(result);
	return result;
}

bool GenericEntity::onNotify(EntityBase &zeEvent)
{
    if (zeEvent.getName().find("QuadTree") != std::string::npos)
    {
        whichQuadIsIn = dynamic_cast<QuadTree*>(&zeEvent);
        return true;
    }
    return false;
}