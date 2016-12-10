#include "EntityBase.h"

EntityBase::EntityBase() 
	: position(0.0f, 0.0f, 0.0f)
	, scale(1.0f, 1.0f, 1.0f)
	, isDone(false)
	, m_bCollider(false)
{
    name_ = "";
}

EntityBase::~EntityBase()
{
}

void EntityBase::Update(double _dt)
{
}

void EntityBase::Render()
{
}

void EntityBase::RenderUI()
{
}

bool EntityBase::IsDone()
{
	return isDone;
}

void EntityBase::SetIsDone(bool _value)
{
	isDone = _value;
}

// Check if this entity has a collider class parent
bool EntityBase::HasCollider(void) const
{
	return m_bCollider;
}

// Set the flag to indicate if this entity has a collider class parent
void EntityBase::SetCollider(const bool &_value)
{
	m_bCollider = _value;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityBase::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
    // Get the colliders for the 2 entities
    // Get the minAABB and maxAABB for each entity
    Vector3 thisMinAABB = ThisEntity->GetPosition() - ThisEntity->GetScale();
    Vector3 thisMaxAABB = ThisEntity->GetPosition() + ThisEntity->GetScale();
    Vector3 thatMinAABB = ThatEntity->GetPosition() - ThatEntity->GetScale();
    Vector3 thatMaxAABB = ThatEntity->GetPosition() + ThatEntity->GetScale();

    // Check for overlap
    if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
        return true;

    // if AABB collision check fails, then we need to check the other corners of the bounding boxes to 
    // Do more collision checks with other points on each bounding box
    Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
    Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);
    //	Vector3 altThatMinAABB = Vector3(thatMinAABB.x, thatMinAABB.y, thatMaxAABB.z);
    //	Vector3 altThatMaxAABB = Vector3(thatMaxAABB.x, thatMaxAABB.y, thatMinAABB.z);

    // Check for overlap
    if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
        return true;

    return false;
}

bool EntityBase::CheckOverlap(const Vector3 &thisMinAABB, const Vector3 &thisMaxAABB, const Vector3 &thatMinAABB, const Vector3 &thatMaxAABB)
{
    // Check if this object is overlapping that object
    if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
        ||
        ((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
    {
        return true;
    }

    // Check if that object is overlapping this object
    if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
        ||
        ((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
    {
        return true;
    }

    // Check if this object is within that object
    if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
        &&
        ((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
        return true;

    // Check if that object is within this object
    if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
        &&
        ((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
        return true;

    return false;
}

bool EntityBase::onNotify(EntityBase &zeEvent)
{
    return false;
}

const std::string &EntityBase::getName()
{
    return name_;
}

void EntityBase::setName(const std::string &zeStr)
{
    name_ = zeStr;
}