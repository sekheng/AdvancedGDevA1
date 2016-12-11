#include "SceneGraph.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
SceneGraph::SceneGraph(void)
	: ID(0)
	, theRoot(NULL)
{
	theRoot = new SceneNode();
	// Assign the first ID to the root. Default is 0
	theRoot->SetID(this->GenerateID());
}
SceneGraph::~SceneGraph()
{
}
void SceneGraph::Destroy()
{
	if (theRoot)
	{
		theRoot->DeleteAllChildren();
		delete theRoot;
	}
	Singleton<SceneGraph>::Destroy();
}
SceneNode* SceneGraph::GetRoot()
{
	return theRoot;
}
// Add a Node to this Scene Graph
SceneNode* SceneGraph::AddNode(EntityBase* theEntity)
{
	SceneNode* aNewSceneNode = theRoot->AddChild(theEntity);
	// aNewSceneNode->SetID(this->GenerateID());
	return aNewSceneNode;
}
// Delete a Node from this Scene Graph using the pointer to the node
bool SceneGraph::DeleteNode(EntityBase* theEntity)
{
	return theRoot->DeleteChild(theEntity);
}
// Delete a Node from this Scene Graph using its ID
bool SceneGraph::DeleteNode(const int ID)
{
	return theRoot->DeleteChild(ID);
}
// Generate an ID for a Scene Node
int SceneGraph::GenerateID(void)
{
	// Return the ID and increment the value by 1
	return ID++;
}
// PrintSelf for debug purposes
void SceneGraph::PrintSelf(void)
{
	theRoot->PrintSelf();
}

// Detach a Node from this Scene Graph using the pointer to the node
SceneNode* SceneGraph::DetachNode(SceneNode* theNode)
{
	return theRoot->DetachChild(theNode);
}
// Detach a Node from this Scene Graph using its ID
SceneNode* SceneGraph::DetachNode(const int ID)
{
	return theRoot->DetachChild(ID);
}
// Get a Node using the pointer to the node
SceneNode* SceneGraph::GetNode(EntityBase* theEntity)
{
	return theRoot->GetEntity(theEntity);
}
// Get a Node using its ID
SceneNode* SceneGraph::GetNode(const int ID)
{
	return theRoot->GetEntity(ID);
}
// Return the number of nodes in this Scene Graph
int SceneGraph::GetNumOfNode(void)
{
	return theRoot->GetNumOfChild();
}
// Update the Scene Graph
void SceneGraph::Update(void)
{
	theRoot->Update();
}
// Render the Scene Graph
void SceneGraph::Render(void)
{
	theRoot->Render();
}