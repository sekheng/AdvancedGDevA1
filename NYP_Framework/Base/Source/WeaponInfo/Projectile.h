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
    virtual bool onNotify(const float &zeEvent);
    virtual bool onNotify(const Vector3 &zeEvent1, const Vector3 &zeEvent2);

protected:
    Vector3 vel_;
    float speed_, timespan_;
private:
    static size_t zeID;
};
