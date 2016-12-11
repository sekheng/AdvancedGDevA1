#pragma once

#include "../GenericEntity.h"

class Projectile : public GenericEntity
{
public:
    Projectile();
    Projectile(Mesh *zeMesh);
    virtual ~Projectile();
};