#include "QuadTree.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#ifdef _DEBUG
#include <assert.h>
#endif

#define MAX_NUM_OBJ 4U

Mesh* QuadTree::debuggingModel = nullptr;

QuadTree::QuadTree()
{
    previousQuad = nullptr;
    if (!debuggingModel)
        debuggingModel = MeshBuilder::GetInstance()->GetMesh("GRIDMESH");
    m_bCollider = false;
    static size_t zeQuadID = 0;
    name_ = "QuadTree";
    name_.append(std::to_string(zeQuadID++));
}

QuadTree::~QuadTree()
{
    otherTrees.clear();
    m_objectList.clear();
    previousQuad = nullptr;
}

void QuadTree::Update(double dt)
{
    while (!waitingList.empty())
    {
        waitingList.back()->onNotify(*this);
        m_objectList.push_back(waitingList.back());
        waitingList.pop_back();
    }
    switch (otherTrees.empty())
    {
    case true:
        if (m_objectList.size() > MAX_NUM_OBJ)
        {
            //Just the following sentences gives me a huge headache. So just hardcode 4 quad for each quad
            QuadTree zeTree;
            zeTree.SetScale(scale * 0.5f);
            zeTree.SetPosition(position + Vector3(-scale.x * 0.25f, 10, scale.z * 0.25f));
            otherTrees.push_back(zeTree);
            zeTree.SetPosition(position + Vector3(-scale.x * 0.25f, 10, -scale.z * 0.25f));
            otherTrees.push_back(zeTree);
            zeTree.SetPosition(position + Vector3(scale.x * 0.25f, 10, -scale.z * 0.25f));
            otherTrees.push_back(zeTree);
            zeTree.SetPosition(position + Vector3(scale.x * 0.25f, 10, scale.z * 0.25f));
            otherTrees.push_back(zeTree);

            std::vector<size_t> whatObjectToRemove;
            for (std::vector<EntityBase*>::iterator it = m_objectList.begin(), end = m_objectList.end(); it != end; ++it)
            {
                for (std::vector<QuadTree>::iterator quadIt = otherTrees.begin(), quadEND = otherTrees.end(); quadIt != quadEND; ++quadIt)
                {
                    if (quadIt->CheckAABBCollision(&(*quadIt), *it))
                    {
                        //quadIt->m_objectList.push_back(*it);
                        quadIt->onNotify(**it);
                        whatObjectToRemove.push_back(it - m_objectList.begin());
                        break;
                    }
                }
            }
            //m_objectList.clear();
            for (std::vector<size_t>::reverse_iterator rit = whatObjectToRemove.rbegin(), rend = whatObjectToRemove.rend(); rit != rend; ++rit)
                m_objectList.erase(m_objectList.begin() + (*rit));
            for (std::vector<EntityBase*>::iterator it = m_objectList.begin(), end = m_objectList.end(); it != end; ++it)
                previousQuad->onNotify(**it);
            m_objectList.clear();

            for (std::vector<QuadTree>::iterator quadIt = otherTrees.begin(), quadEND = otherTrees.end(); quadIt != quadEND; ++quadIt)
                quadIt->previousQuad = this;
        }
        //else
        //{
        //    std::vector<size_t> removeStuffInObjectList;
        //    for (std::vector<EntityBase*>::iterator it = m_objectList.begin(), end = m_objectList.end(); it != end; ++it)
        //    {
        //        (*it)->Update(dt);
        //        if (CheckAABBCollision(this, *it) == false)
        //        {
        //            previousQuad->waitingList.push_back(*it);
        //            removeStuffInObjectList.push_back(it - m_objectList.begin());
        //        }
        //    }
        //    for (std::vector<size_t>::iterator it = removeStuffInObjectList.begin(), end = removeStuffInObjectList.end(); it != end; ++it)
        //    {
        //        m_objectList.erase(m_objectList.begin() + (*it));
        //    }
        //}
        break;
    default:
    {
        bool checkWholeTreeIsEmpty = true;
        for (std::vector<QuadTree>::iterator it = otherTrees.begin(), end = otherTrees.end(); it != end; ++it)
        {
            it->Update(dt);
            if (!it->onNotify("IS_EMPTY"))
                checkWholeTreeIsEmpty = false;
        }
        if (checkWholeTreeIsEmpty)
        {
            otherTrees.clear();
        }

        // This 1st part is to check whether the entity fit into it's quads
        if (!m_objectList.empty())
        {
            std::vector<size_t> removeStuffInObjectList;
            for (std::vector<EntityBase*>::iterator it = m_objectList.begin(), end = m_objectList.end(); it != end; ++it)
            {
                for (std::vector<QuadTree>::iterator quadIt = otherTrees.begin(), quadEND = otherTrees.end(); quadIt != quadEND; ++quadIt)
                {
                    if (quadIt->CheckAABBCollision(&(*quadIt), *it))
                    {
                        removeStuffInObjectList.push_back(it - m_objectList.begin());
                        quadIt->m_objectList.push_back((*it));
                        break;
                    }
                }
            }
            for (std::vector<size_t>::reverse_iterator rit = removeStuffInObjectList.rbegin(), rend = removeStuffInObjectList.rend(); rit != rend; ++rit)
                m_objectList.erase(m_objectList.begin() + (*rit));

            // This part comes into effect when the entities doesn't fit into either of it's quads
            if (!m_objectList.empty())
            {
                for (std::vector<EntityBase*>::iterator it = m_objectList.begin(), end = m_objectList.end(); it != end; ++it)
                {
                    previousQuad->m_objectList.push_back(*it);
                }
            }
        }
    }
    }
}

void QuadTree::Render()
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    modelStack.PushMatrix();
    //modelStack.Rotate(90, 1, 0, 0);
    modelStack.Translate(position.x, position.y, position.z);
    modelStack.Scale(scale.x, scale.y, scale.z);
    RenderHelper::RenderMesh(debuggingModel);
    modelStack.PopMatrix();

    for (std::vector<QuadTree>::iterator it = otherTrees.begin(), end = otherTrees.end(); it != end; ++it)
    {
        it->Render();
    }
}

bool QuadTree::onNotify(EntityBase &zeEvent)
{
    if (!otherTrees.empty())
    {
        for (std::vector<QuadTree>::iterator quadIt = otherTrees.begin(), quadEND = otherTrees.end(); quadIt != quadEND; ++quadIt)
        {
            if (quadIt->CheckAABBCollision(&(*quadIt), &zeEvent))
            {
                return quadIt->onNotify(zeEvent);
            }
        }
        previousQuad->onNotify(zeEvent);
    }
    else
    {
        waitingList.push_back(&zeEvent);
    }
    return true;
}

bool QuadTree::onNotify(const std::string &zeEvent)
{
    if (zeEvent == "IS_EMPTY" && otherTrees.empty() && m_objectList.empty())
        return true;
    return false;
}