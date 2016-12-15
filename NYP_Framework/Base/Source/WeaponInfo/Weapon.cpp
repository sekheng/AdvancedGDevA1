#include "Weapon.h"
#include "SceneManager.h"
#include "../MusicsStuff/MusicSystem.h"

size_t Weapon::zeID = 0;

Weapon::Weapon()
    : GenericEntity(nullptr)
{
    maxBullets = maxClips = currBullets = currClips = 0;
    fireRate = timeCounter = 0;
    name_ = "Weapon";
    name_.append(std::to_string(zeID++));
}

Weapon::Weapon(Mesh *zeMesh)
    : GenericEntity(zeMesh)
{
    maxBullets = maxClips = currBullets = currClips = 0;
    fireRate = timeCounter = 0;
    name_ = "Weapon";
    name_.append(std::to_string(zeID++));
}

Weapon::~Weapon()
{

}

void Weapon::Update(double dt)
{
    timeCounter += (float)dt;
}

bool Weapon::onNotify(const std::string &zeEvent)
{
    if (zeEvent.find("FIRE") != std::string::npos)
    {
        if (timeCounter >= fireRate && currBullets > 0)
        {
            --currBullets;
            timeCounter = 0;
            MusicSystem::accessing().playMusic("Fire");
            return SceneManager::GetInstance()->GetCurrScene()->onNotify("FIRE_BULLET");
        }
        else if (currBullets == 0)
            MusicSystem::accessing().playMusic("NoAmmo");
    }
    else if (zeEvent.find("RELOAD") != std::string::npos && currBullets < maxBullets && currClips > 0)
    {
        currBullets = maxBullets;
        --currClips;
        return true;
    }
    return false;
}

bool Weapon::onNotify(const float &zeEvent)
{
    if (zeEvent > Math::EPSILON)
    {
        timeCounter = fireRate = zeEvent;
        return true;
    }
    return false;
}

bool Weapon::onNotify(const int &zeEvent1, const int &zeEvent2)
{
    if (zeEvent1 > 0 && zeEvent2 > 0)
    {
        currBullets = maxBullets = zeEvent1;
        currClips = maxClips = zeEvent2;
        return true;
    }
    return false;
}

size_t Weapon::getNumCurrBullets()
{
    return (size_t)currBullets;
}

size_t Weapon::getNumCurrMagazine()
{
    return (size_t)(currClips);
}