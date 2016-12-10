#pragma once

#include "EntityBase.h"
#include <vector>

class QuadTree : public EntityBase
{
public:
    QuadTree();
    virtual ~QuadTree();

    virtual void Update(double dt);
    virtual void Render();

    std::vector<QuadTree> otherTrees;
    std::vector<EntityBase*> m_objectList;
};