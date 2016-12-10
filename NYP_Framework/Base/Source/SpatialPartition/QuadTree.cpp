#include "QuadTree.h"

QuadTree::QuadTree()
{

}

QuadTree::~QuadTree()
{
    otherTrees.clear();
    m_objectList.clear();
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

}