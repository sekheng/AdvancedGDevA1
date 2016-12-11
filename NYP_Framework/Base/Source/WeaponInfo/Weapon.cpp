#include "Weapon.h"

Weapon::Weapon()
    : GenericEntity(nullptr)
{
    maxBullets = maxClips = currBullets = currClips = 0;
    fireRate = timeCounter = 0;
}

Weapon::Weapon(Mesh *zeMesh)
    : GenericEntity(zeMesh)
{
    maxBullets = maxClips = currBullets = currClips = 0;
    fireRate = timeCounter = 0;
}

Weapon::~Weapon()
{

}

void Weapon::Update(double dt)
{
    timeCounter += dt;
}

bool Weapon::onNotify(const std::string &zeEvent)
{
    if (zeEvent.find("FIRE") != std::string::npos && timeCounter >= fireRate)
    {
        return true;
    }
    return false;
}

bool Weapon::onNotify(const float &zeEvent)
{
    if (zeEvent > Math::EPSILON)
    {
        timeCounter = fireRate = (double)zeEvent;
        return true;
    }
    return false;
}