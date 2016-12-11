#pragma once

#include "../GenericEntity.h"

class Projectile : public GenericEntity
{
public:
    Projectile();
    Projectile(Mesh *zeMesh);
    virtual ~Projectile();

    virtual void Update(double dt);

    virtual bool onNotify(const std::string &zeEvent);
    virtual bool onNotify(EntityBase &zeEvent);

protected:
    Vector3 vel_;
};
