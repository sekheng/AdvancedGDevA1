#pragma once

#include "../GenericEntity.h"

class Weapon : public GenericEntity
{
public:
    Weapon();
    Weapon(Mesh *zeMesh);
    virtual ~Weapon();

    virtual void Update(double dt);

    virtual bool onNotify(const std::string &zeEvent);
    virtual bool onNotify(const float &zeEvent);

protected:
    unsigned char maxBullets, currBullets, maxClips, currClips;
    double timeCounter, fireRate;
};