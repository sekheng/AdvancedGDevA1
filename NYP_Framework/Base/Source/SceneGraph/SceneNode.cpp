#include "SceneNode.h"
#include "../EntityManager.h"
#include <algorithm>

#include "SceneGraph.h"
#include "GraphicsManager.h"

SceneNode::SceneNode()
	: ID(-1)
	, theEntity(NULL)
	, theParent(NULL)
{

}
SceneNode::~SceneNode()
{
}

void SceneNode::Destroy()
{
	std::vector<SceneNode*>::iterator it;
	for (it = theChildren.begin(); it != theChildren.end(); ++it)
	{
		(*it)->Destroy();
		delete *it;
		theChildren.erase(it);
	}
	theEntity = NULL;
	theParent = NULL;
}
void SceneNode::SetID(const int ID)
{
	this->ID = ID;
}
int SceneNode::GetID()const
{
	return ID;
}

bool SceneNode::SetEntity(EntityBase* theEntity)
{
	if (theEntity)
	{
		this->theEntity = theEntity;
		return this;
	}
	return false;
}

EntityBase* SceneNode::GetEntity() const
{
	return theEntity;
}

void SceneNode::SetParent(SceneNode* theParent)
{
	this->theParent = theParent;
}

SceneNode* SceneNode::GetParent() const
{
	return theParent;
}

void SceneNode::PrintSelf(const int numTabs)
{
	if (numTabs == 0)
	{
		cout << "===========================" << endl << "Start of theRoot::PrintSelf()" << endl;
		cout << "SceneNode::PrintSelf: ID = " << ID << "/Childeren = " << theChildren.size() << endl;
		cout << "Printing out the children: " << endl;
		std::vector<SceneNode*>::iterator it = theChildren.begin();
		while (it != theChildren.end())
		{
			(*it)->PrintSelf((numTabs + 1));
			it++;
		}
		cout << "End of theRoot::PrintSelf()" << endl << "===========================" << endl;

	}
	else
	{
		for (int i = 0; i < numTabs; i++)
		{
			cout << "\it";
		}
		cout << "SceneNode::PrintSelf: ID = " << ID << "/theEntity" << theEntity << "/Children = " << theChildren.size() << endl;
		for (int i = 0; i < numTabs; i++)
			cout << "\it";
		cout << "Printing out the children: " << endl;
		std::vector<SceneNode*>::iterator it = theChildren.begin();
		while (it != theChildren.end())
		{
			(*it)->PrintSelf((numTabs + 1));
			it++;
		}
	}
}

SceneNode* SceneNode::AddChild(EntityBase* theEntity)
{
	if (theEntity)
	{
		SceneNode* aNewNode = new SceneNode();

		aNewNode->SetEntity(theEntity);

		aNewNode->SetParent(this);

		aNewNode->SetID(SceneGraph::GetInstance()->GenerateID());

		this->theChildren.push_back(aNewNode);

		return aNewNode;
	}
	return NULL;
}

bool SceneNode::DeleteChild(EntityBase* theEntity )
{
	// if this node contains theEntity, then we proceed to delete all its children
	if (this->theEntity == theEntity)
	{
		// If this node has children, then we proceed to delete them.
		if (theChildren.size() != 0)
		{
			// Iterate through all the children and delete all of their children and grand children etc
			vector <SceneNode*>::iterator it = theChildren.begin();
			while (it != theChildren.end())
			{
				if ((*it)->DeleteAllChildren())
				{
					cout << "SceneNode::DeleteChild: Deleted child nodes for theEntity." << endl;
				}
				(*it)->GetEntity()->SetIsDone(true);
				delete *it;
				it = theChildren.erase(it);
			}
		}
		return true; // return true to say that this Node contains theEntity
	}
	else
	{
		// Search the children for this particular theEntity
		if (theChildren.size() != 0)
		{
			std::vector<SceneNode*>::iterator it;
			for (it = theChildren.begin(); it != theChildren.end(); ++it)
			{
				// Check if this child is the one containing theEntity
				if ((*it)->DeleteChild(theEntity))
				{
					// If DeleteChild method call above DID remove theEntity
					// Then we should proceed to removed this child from our vector of children
					(*it)->GetEntity()->SetIsDone(true);
					delete *it;
					theChildren.erase(it);
					break; // Stop deleting since we have already found and deleted theEntity
				}
			}
		}
	}
	return false;

}

bool SceneNode::DeleteChild(const int ID)
{
	// if this node contains theEntity, then we proceed to delete all its children
	if (this->ID == ID)
	{
		// If this node has children, then we proceed to delete them.
		if (theChildren.size() != 0)
		{
			// Iterate through all the children and delete all of their children and grand children etc
			vector <SceneNode*>::iterator it = theChildren.begin();
			while (it != theChildren.end())
			{
				if ((*it)->DeleteAllChildren())
				{
					cout << "SceneNode::DeleteChild: Deleted child nodes for ID=" << ID << endl;
				}
				(*it)->GetEntity()->SetIsDone(true);
				delete *it;
				it = theChildren.erase(it);
			}
		}
		return true; // return true to say that this Node contains theEntity
	}
	else
	{
		// Search the children for this particular theEntity
		if (theChildren.size() != 0)
		{
			std::vector<SceneNode*>::iterator it;
			for (it = theChildren.begin(); it != theChildren.end(); ++it)
			{
				// Check if this child is the one containing theEntity
				if ((*it)->DeleteChild(theEntity))
				{
					// If DeleteChild method call above DID remove theEntity
					// Then we should proceed to removed this child from our vector of children
					(*it)->GetEntity()->SetIsDone(true);
					delete *it;
					theChildren.erase(it);
					break; // Stop deleting since we have already found and deleted theEntity
				}
			}
		}
	}
	return false;

}

bool SceneNode::DeleteAllChildren()
{
	bool bResult = false;
	vector <SceneNode*>::iterator it = theChildren.begin();
	while (it != theChildren.end()-1)
	{
		if ((*it)->DeleteAllChildren())
		{
			cout << "SceneNode::DeleteChild: Delete child nodes." << endl;
		}
		(*it)->GetEntity()->SetIsDone(true);
		delete *it;
		it = theChildren.erase(it);
		bResult = true;
	}
	return bResult;

}

SceneNode* SceneNode::DetachChild(EntityBase* theEntity)
{
	// if it is inside this node, then return this node
	if (this->theEntity == theEntity)
		return this;
	if (theChildren.size() != 0)
	{
		vector <SceneNode*>::iterator it = theChildren.begin();
		while (it != theChildren.end())
		{
			SceneNode* theNode = (*it)->DetachChild(theEntity);
			if (theNode)
			{
				// Remove this node from the children
				theChildren.erase(it);
				return theNode;
			}
			it++;
		}
	}
	return NULL;
}
SceneNode* SceneNode::DetachChild(const int ID)
{
	// if it is inside this node, then return this node
	if (this->ID == ID)
		return this;
	if (theChildren.size() != 0)
	{
		vector <SceneNode*>::iterator it = theChildren.begin();
		while (it != theChildren.end())
		{
			SceneNode* theNode = (*it)->DetachChild(ID);
			if (theNode)
			{
				// Remove this node from the children
				theChildren.erase(it);
				return theNode;
			}
			it++;
		}
	}
	return NULL;

}

SceneNode* SceneNode::DetachChild(SceneNode* theNode)
{
	// if it is inside this node, then return this node
	if (this == theNode)
		return this;
	if (theChildren.size() != 0)
	{
		vector <SceneNode*>::iterator it = theChildren.begin();
		while (it != theChildren.end())
		{
			SceneNode* theTempNode = theNode;
			if (theTempNode)
			{
				// Remove this node from the children
				theChildren.erase(it);
				return theTempNode;
			}
			it++;
		}
	}
	return NULL;
}

SceneNode* SceneNode::GetEntity(EntityBase* theEntity)
{
	// if it is inside this node, then return this node
	if (this->theEntity == theEntity)
		return this;
	if (theChildren.size() != 0)
	{
		std::vector<SceneNode*>::iterator it;
		for (it = theChildren.begin(); it != theChildren.end(); ++it)
		{
			SceneNode* theNode = (*it)->GetEntity(theEntity);
			if (theNode)
			{
				return theNode;
			}
		}
	}
	return NULL;

}
SceneNode* SceneNode::GetEntity(const int ID)
{
	// if it is inside this node, then return this node
	if (this->ID == ID)
		return this;
	// Check the children
	if (theChildren.size() != 0)
	{
		std::vector<SceneNode*>::iterator it;
		for (it = theChildren.begin(); it != theChildren.end(); ++it)
		{
			SceneNode* theNode = (*it)->GetEntity(ID);
			if (theNode)
			{
				return theNode;
			}
		}
	}
	return NULL;

}

int SceneNode::GetNumOfChild()
{
	// Start with this node's children
	int NumOfChild = theChildren.size();
	// Ask the children to feedback how many children they have
	std::vector<SceneNode*>::iterator it;
	for (it = theChildren.begin(); it != theChildren.end(); ++it)
	{
		NumOfChild += (*it)->GetNumOfChild();
	}
	return NumOfChild;

}

// Update the Scene Graph
void SceneNode::Update(void)
{
	if (theUpdateRotation)
	{
		ApplyTransform(GetUpdateRotation());
	}
	if (theUpdateTransformation)
	{
		ApplyTransform(GetUpdateTransform());
	}
	
	if (theEntity != NULL)
	{

		if (theEntity->IsDone() == true)
		{
			std::vector<SceneNode*>::iterator it;
			for (it = theChildren.begin(); it != theChildren.end(); ++it)
			{
				(*it)->theEntity->onNotify("DIED");
				(*it)->theEntity->onNotify("DIED");
			}
		}
	}
	// Update the children
	std::vector<SceneNode*>::iterator it;
	for (it = theChildren.begin(); it != theChildren.end(); ++it)
	{
		(*it)->Update();
	}
}
// Render the Scene Graph
void SceneNode::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();

	if (theEntity)
	{
		//modelStack.LoadMatrix(this->GetTransform());
		modelStack.MultMatrix(this->GetTransform());

		// Render the entity
		theEntity->Render();
	}

	// Render the children
	std::vector<SceneNode*>::iterator it;
	for (it = theChildren.begin(); it != theChildren.end(); ++it)
	{
		(*it)->Render();
	}

	modelStack.PopMatrix();
}

