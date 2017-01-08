#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H

#include "EntityBase.h"
#include <string>
//#include "Collider/Collider.h"
#include "SpatialPartition\QuadTree.h"
#include "LevelOfDetail\LevelOfDetails.h"

class Mesh;

class GenericEntity : public EntityBase, public CLevelOfDetails/*, public CCollider*/
{
public:
	GenericEntity(Mesh* _modelMesh);
	virtual ~GenericEntity();

	virtual void Update(double _dt);
	virtual void Render();

	// Set the maxAABB and minAABB
	//void SetAABB(Vector3 maxAABB, Vector3 minAABB);

    virtual bool onNotify(EntityBase &zeEvent);
    virtual bool onNotify(const std::string &zeEvent);
	virtual bool onNotify(const Vector3 &zeEvent1, const Vector3 &zeEvent2);
	virtual bool onNotify(const Vector3 &zeEvent);
	virtual bool onNotify(const float &zeEvent1, const float &zeEvent2);
	//bool isVisable()
	bool removeItselfFromQuad();
protected:
	Mesh* modelMesh;
    QuadTree *whichQuadIsIn;
    EntityBase *boundary_;
    unsigned char howManyLives;

    
	bool isVisible;
	bool isKilledByBullet;

	float levelOfDetail_Distances[2];
	Vector3 posOfPlayer, targetOfPlayer;
};

namespace Create
{
	GenericEntity* Entity(	const std::string& _meshName, 
							const Vector3& _position, 
							const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};

#endif // GENERIC_ENTITY_H