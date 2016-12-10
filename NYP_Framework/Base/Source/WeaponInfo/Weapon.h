#pragma once

#include "../GenericEntity.h"

class Weapon : public GenericEntity
{
public:
    Weapon();
    Weapon(Mesh *zeMesh);
    virtual ~Weapon();

    virtual void Update(double dt);

protected:

};