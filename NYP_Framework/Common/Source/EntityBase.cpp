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
    Vector3 thisMinAABB = ThisEntity->position - (ThisEntity->scale * 0.5f);
    Vector3 thisMaxAABB = ThisEntity->position + (ThisEntity->scale*0.5f);
    Vector3 thatMinAABB = ThatEntity->position - (ThatEntity->scale*0.5f);
    Vector3 thatMaxAABB = ThatEntity->position + (ThatEntity->scale*0.5f);

    // Check for overlap
    //if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
    //    return true;
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

    // if AABB collision check fails, then we need to check the other corners of the bounding boxes to 
    // Do more collision checks with other points on each bounding box
    Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
    Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);

    // Check for overlap
    //if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
    //    return true;
    // Check if this object is overlapping that object
    if (((thatMinAABB >= altThisMinAABB) && (thatMinAABB <= altThisMaxAABB))
        ||
        ((thatMaxAABB >= altThisMinAABB) && (thatMaxAABB <= altThisMaxAABB)))
    {
        return true;
    }

    // Check if that object is overlapping this object
    if (((altThisMinAABB >= thatMinAABB) && (altThisMinAABB <= thatMaxAABB))
        ||
        ((altThisMaxAABB >= thatMinAABB) && (altThisMaxAABB <= thatMaxAABB)))
    {
        return true;
    }

    // Check if this object is within that object
    if (((altThisMinAABB >= thatMinAABB) && (altThisMaxAABB <= thatMaxAABB))
        &&
        ((altThisMaxAABB >= thatMinAABB) && (altThisMaxAABB <= thatMaxAABB)))
        return true;

    // Check if that object is within this object
    if (((thatMinAABB >= altThisMinAABB) && (thatMinAABB <= altThisMaxAABB))
        &&
        ((thatMaxAABB >= altThisMinAABB) && (thatMaxAABB <= altThisMaxAABB)))
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

bool EntityBase::onNotify(const float &zeEvent)
{
    return false;
}

bool EntityBase::onNotify(const float &zeEvent1, const float &zeEvent2)
{
    return false;
}

bool EntityBase::onNotify(const std::string &zeEvent)
{
    return false;
}

bool EntityBase::onNotify(const int &zeEvent)
{
    return false;
}

bool EntityBase::onNotify(const int &zeEvent1, const int &zeEvent2)
{
    return false;
}

bool EntityBase::onNotify(const Vector3 &zeEvent)
{
    return false;
}

bool EntityBase::onNotify(const Vector3 &zeEvent1, const Vector3 &zeEvent2)
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

bool EntityBase::operator==(const EntityBase &rhs)
{
    if (name_ == rhs.name_ && position == rhs.position && scale == rhs.scale)
        return true;
    return false;
}

bool EntityBase::operator!=(const EntityBase &rhs)
{
    return !(*this == rhs);
}