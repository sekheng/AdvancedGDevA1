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
    virtual bool onNotify(const int &zeEvent1, const int &zeEvent2);

    size_t getNumCurrBullets();
    size_t getNumCurrMagazine();

protected:
    unsigned char maxBullets, currBullets, maxClips, currClips;
    float timeCounter, fireRate;

private:
    static size_t zeID;
};