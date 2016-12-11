#pragma once

#include "EntityBase.h"
#include <vector>
#include "Mesh.h"

class QuadTree : public EntityBase
{
public:
    QuadTree();
    virtual ~QuadTree();

    virtual void Update(double dt);
    virtual void Render();

    virtual bool onNotify(EntityBase &zeEvent);
    virtual bool onNotify(const std::string &zeEvent);

    std::vector<QuadTree> otherTrees;
    std::vector<EntityBase*> m_objectList, waitingList;
    QuadTree *previousQuad;

private:
    static Mesh *debuggingModel;
};