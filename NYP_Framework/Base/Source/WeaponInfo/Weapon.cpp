#include "Weapon.h"

Weapon::Weapon()
    : GenericEntity(nullptr)
{
    maxBullets = maxClips = currBullets = currClips = 0;
}

Weapon::Weapon(Mesh *zeMesh)
    : GenericEntity(zeMesh)
{
    maxBullets = maxClips = currBullets = currClips = 0;
}

Weapon::~Weapon()
{

}

void Weapon::Update(double dt)
{

}

bool Weapon::onNotify(const std::string &zeEvent)
{
    return false;
}