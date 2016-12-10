#include "QuadTree.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

Mesh* QuadTree::debuggingModel = nullptr;

QuadTree::QuadTree()
{
    previousQuad = nullptr;
    if (!debuggingModel)
        debuggingModel = MeshBuilder::GetInstance()->GetMesh("GRIDMESH");
}

QuadTree::~QuadTree()
{
    otherTrees.clear();
    m_objectList.clear();
    previousQuad = nullptr;
}

void QuadTree::Update(double dt)
{
    switch (otherTrees.empty())
    {
    case true:
        for (std::vector<EntityBase*>::iterator it = m_objectList.begin(), end = m_objectList.end(); it != end; ++it)
        {
            (*it)->Update(dt);
        }
        break;
    default:
        for (std::vector<QuadTree>::iterator it = otherTrees.begin(), end = otherTrees.end(); it != end; ++it)
        {
            it->Update(dt);
        }
    }
}

void QuadTree::Render()
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y, position.z);
    modelStack.Rotate(90, 1, 0, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);

    RenderHelper::RenderMesh(debuggingModel);

    modelStack.PopMatrix();
}