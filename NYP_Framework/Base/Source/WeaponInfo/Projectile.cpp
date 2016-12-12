#include "Projectile.h"
#include "MeshBuilder.h"

#define MAX_LIFESPAN 10.f

size_t Projectile::zeID = 0;

Projectile::Projectile()
    : GenericEntity(MeshBuilder::GetInstance()->GetMesh("sphere"))
{
    vel_.SetZero();
    speed_ = 50;
    timespan_ = MAX_LIFESPAN;
    isDone = true;
    name_ = "Projectile"; 
    name_.append(std::to_string(zeID++));
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
            for (std::vector<EntityBase*>::iterator it = whichQuadIsIn->m_objectList.begin(), end = whichQuadIsIn->m_objectList.end(); it != end; ++it)
            {
                if ((**it) == *this)
                {
                    whichQuadIsIn->m_objectList.erase(it);
                    break;
                }
            }
            return;
        }
        if (!vel_.IsZero())
        {
            position += vel_ * (float)(dt)* speed_;
            if (whichQuadIsIn)
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