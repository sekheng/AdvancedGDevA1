#include "Projectile.h"

Projectile::Projectile()
    : GenericEntity(nullptr)
{
    vel_.SetZero();
}

Projectile::Projectile(Mesh *zeMesh)
    : GenericEntity(zeMesh)
{
    vel_.SetZero();
}

Projectile::~Projectile()
{

}

void Projectile::Update(double dt)
{
    if (!vel_.IsZero())
    {

    }
}

bool Projectile::onNotify(const std::string &zeEvent)
{
    if (zeEvent.find("RESET") != std::string::npos)
    {
        return true;
    }
    return false;
}

bool Projectile::onNotify(EntityBase &zeEvent)
{
    return false;
}