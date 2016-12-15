#include "Projectile.h"
#include "MeshBuilder.h"
#ifdef _DEBUG
#include <iostream>
#endif
#define MAX_LIFESPAN 10.f

size_t Projectile::zeID = 0;

Projectile::Projectile()
    : GenericEntity(MeshBuilder::GetInstance()->GetMesh("sphere"))
{
    vel_.SetZero();
    speed_ = 400;
    timespan_ = MAX_LIFESPAN;
    isDone = true;
    name_ = "Projectile"; 
    name_.append(std::to_string(zeID++));
    sceneObjectList = nullptr;
}

Projectile::Projectile(Mesh *zeMesh)
    : GenericEntity(zeMesh)
{
    vel_.SetZero();
    speed_ = 10;
    timespan_ = MAX_LIFESPAN;
    isDone = true;
    name_ = "Projectile";
    name_.append(std::to_string(zeID++));
    sceneObjectList = nullptr;
}

Projectile::~Projectile()
{

}

void Projectile::Update(double dt)
{
    if (!isDone)
    {
        timespan_ = Math::Clamp(timespan_ - (float)(dt), 0.f, MAX_LIFESPAN);
        if (timespan_ < Math::EPSILON)
        {
            isDone = true;
            removeItselfFromQuad();
            return;
        }
        if (!vel_.IsZero())
        {
            position += vel_ * (float)(dt)* speed_;

            if (!CheckAABBCollision(this, boundary_))
            {
                removeItselfFromQuad();
            }
            else if (whichQuadIsIn)
            {

                if (!CheckAABBCollision(this, whichQuadIsIn))
                {
                    for (std::vector<EntityBase*>::iterator it = whichQuadIsIn->m_objectList.begin(), end = whichQuadIsIn->m_objectList.end(); it != end; ++it)
                    {
                        if ((**it) == *this)
                        {
                            whichQuadIsIn->m_objectList.erase(it);
                            break;
                        }
                    }
                    whichQuadIsIn->previousQuad->onNotify(*this);
                    whichQuadIsIn = whichQuadIsIn->previousQuad;
                }
                else
                {
                    bool shouldRemoveItself = false;
                    if (!whichQuadIsIn->otherTrees.empty())
                    {
                        for (std::vector<GenericEntity*>::iterator it = sceneObjectList->begin(), end = sceneObjectList->end(); it != end; ++it)
                        {
                            if ((**it) != *this && (*it)->getName().find("Projectile") == std::string::npos)
                            {
#ifdef _DEBUG
                                std::cout << "Object is " << (*it)->getName() << std::endl;
#endif
                                Vector3 thatMinAABB = (*it)->GetPosition() - (*it)->GetScale() /*- scale*/;
                                Vector3 thatMaxAABB = (*it)->GetPosition() + (*it)->GetScale() /*+ scale*/;
                                Vector3 HitPosition(0, 0, 0);
                                if (CheckLineSegmentPlane(position, position - (position + vel_ * 200.f), thatMinAABB, thatMaxAABB, HitPosition))
                                {
#ifdef _DEBUG
                                    std::cout << "Hit  " << (*it)->getName() << std::endl;
#endif
                                    isDone = true;
                                    shouldRemoveItself = true;
                                    (*it)->onNotify("DIED");
                                    break;
                                }
                            }
                        }
                    }
                   else
                            {
                                for (std::vector<EntityBase*>::iterator it = whichQuadIsIn->m_objectList.begin(), end = whichQuadIsIn->m_objectList.end(); it != end; ++it)
                                {
                                    if ((**it) != *this && (*it)->getName().find("Projectile") == std::string::npos)
                                    {
#ifdef _DEBUG
                                        std::cout << "Object is " << (*it)->getName() << std::endl;
#endif
                                        Vector3 thatMinAABB = (*it)->GetPosition() - (*it)->GetScale() - scale;
                                        Vector3 thatMaxAABB = (*it)->GetPosition() + (*it)->GetScale() + scale;
                                        Vector3 HitPosition(0, 0, 0);
                                        if (CheckLineSegmentPlane(position, position - (position + vel_ * 200.f), thatMinAABB, thatMaxAABB, HitPosition))
                                        {
#ifdef _DEBUG
                                            std::cout << "Hit  " << (*it)->getName() << std::endl;
#endif
                                            isDone = true;
                                            shouldRemoveItself = true;
                                            (*it)->onNotify("DIED");
                                            break;
                                        }
                                    }
                                }
                            }
                            if (shouldRemoveItself)
                                removeItselfFromQuad();
                        }

                }
            }
        }
    }


bool Projectile::onNotify(const std::string &zeEvent)
{
    if (zeEvent.find("RESET") != std::string::npos)
    {
        return true;
    }
    /*else if (zeEvent.find("EXPIRED") )*/
    return false;
}

bool Projectile::onNotify(const float &zeEvent)
{
    if (zeEvent > Math::EPSILON)
    {
        timespan_ = zeEvent;
        return true;
    }
    else if (zeEvent < -Math::EPSILON)
    {
        speed_ = zeEvent;
        return true;
    }
    return false;
}

bool Projectile::onNotify(const Vector3 &zeEvent1, const Vector3 &zeEvent2)
{
    if (isDone)
    {
        isDone = false;
        vel_ = (zeEvent2 - zeEvent1).Normalize();
        position = zeEvent1;
        timespan_ = MAX_LIFESPAN;
        return true;
    }
    return false;
}

bool Projectile::GetIntersection(const float &fDst1, const float &fDst2, const Vector3 &P1, const Vector3 &P2, Vector3 &Hit)
{
    if ((fDst1 * fDst2) >= Math::EPSILON)
        return false;
    if (fDst1 == fDst2)
        return false;
    Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
    return true;
}

bool Projectile::InBox(const Vector3 &Hit, const Vector3 &B1, const Vector3 &B2, const int &Axis)
{
    if (Axis == 1 && Hit.x > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y)
        return true;
    if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x)
        return true;
    if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y)
        return true;
    return false;
}

bool Projectile::CheckLineSegmentPlane(const Vector3 &line_start, const Vector3 &line_end, const Vector3 &minAABB, const Vector3 &maxAABB, Vector3 &Hit)
{
    if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 1))
        || (GetIntersection(line_start.y - minAABB.y, line_end.y - minAABB.y, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 2))
        || (GetIntersection(line_start.z - minAABB.z, line_end.z - minAABB.z, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 3))
        || (GetIntersection(line_start.x - maxAABB.x, line_end.x - maxAABB.x, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 1))
        || (GetIntersection(line_start.y - maxAABB.y, line_end.y - maxAABB.y, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 2))
        || (GetIntersection(line_start.z - maxAABB.z, line_end.z - maxAABB.z, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 3))
        )
        return true;
    return false;
}

bool Projectile::onNotify(std::vector<GenericEntity*> &zeList)
{
    sceneObjectList = &zeList;
    return true;
}