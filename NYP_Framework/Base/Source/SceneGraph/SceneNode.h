#pragma once

#include "Vector3.h"
#include "Transform.h"
#include <vector>

using namespace std;

#include "EntityBase.h"

class SceneNode : public Transform
{
public:
	SceneNode();
	virtual ~SceneNode();

	void Destroy();
	void SetID(const int ID);
	int GetID()const;

	bool SetEntity(EntityBase* theEntity);

	EntityBase* GetEntity() const;

	void SetParent(SceneNode* theParent);

	SceneNode* GetParent() const;

	void PrintSelf(const int numTabs = 0);

	SceneNode* AddChild(EntityBase* theEntity = NULL);

	bool DeleteChild(EntityBase* theEntity = NULL);

	bool DeleteChild(const int ID);

	bool DeleteAllChildren();

	SceneNode* DetachChild(EntityBase* theEntity = NULL);
	SceneNode* DetachChild(const int ID);
	SceneNode* DetachChild(SceneNode* theNode = NULL);
	SceneNode* GetEntity(EntityBase* theEntity);
	SceneNode* GetEntity(const int ID);

	int GetNumOfChild();

	// Update the Scene Graph
	void Update(void);
	// Render the Scene Graph
	void Render(void);

	

protected:
	int ID;
	EntityBase* theEntity;
	SceneNode* theParent;

	vector<SceneNode*> theChildren;
};