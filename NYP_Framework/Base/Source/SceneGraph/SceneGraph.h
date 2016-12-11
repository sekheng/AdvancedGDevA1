#pragma once
#include "SingletonTemplate.h"
#include "EntityBase.h"
#include "Vector3.h"
#include <string>
#include "SceneNode.h"
class Mesh;
class SceneGraph : public Singleton<SceneGraph>
{
	friend Singleton<SceneGraph>;
public:
	SceneGraph(void);
	virtual ~SceneGraph();
	void Destroy();
	// Get the root of this Scene Graph
	SceneNode* GetRoot();
	// Add a Node to this Scene Graph
	SceneNode* AddNode(EntityBase* theEntity = NULL);
	// Delete a Node from this Scene Graph using the pointer to the node
	bool DeleteNode(EntityBase* theEntity);
	// Delete a Node from this Scene Graph using its ID
	bool DeleteNode(const int ID);
	// Detach a Node from this Scene Graph using the pointer to the node
	SceneNode* DetachNode(SceneNode* theNode);
	// Detach a Node from this Scene Graph using its ID
	SceneNode* DetachNode(const int ID);
	// Get a Node using the pointer to the node
	SceneNode* GetNode(EntityBase* theEntity);
	// Get a Node using its ID
	SceneNode* GetNode(const int ID);
	// Return the number of nodes in this Scene Graph
	int GetNumOfNode(void);
	// Generate an ID for a Scene Node
	int GenerateID(void);
	// Update the Scene Graph
	void Update(void);
	// Render the Scene Graph
	void Render(void);
	// PrintSelf for debug purposes
	void PrintSelf(void);
	// The root of the scene graph.
	// It usually does not have a mesh, and is the starting point for all scene graph operations
	SceneNode* theRoot;
protected:
	// The next ID to be assigned to a scene node.
	int ID;
};