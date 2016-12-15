#pragma once

#include "../GenericEntity.h"

class Projectile : public GenericEntity
{
public:
    Projectile();
    Projectile(Mesh *zeMesh);
    virtual ~Projectile();

    virtual void Update(double dt);
    virtual void Render();

    virtual bool onNotify(const std::string &zeEvent);
    virtual bool onNotify(const float &zeEvent1, const float &zeEvent2);
    virtual bool onNotify(const float &zeEvent);
    virtual bool onNotify(const Vector3 &zeEvent1, const Vector3 &zeEvent2);
    virtual bool onNotify(std::vector<GenericEntity*> &zeList);

protected:
    Vector3 vel_;
    float speed_, timespan_;

    bool GetIntersection(const float &fDst1, const float &fDst2, const Vector3 &P1, const Vector3 &P2, Vector3 &Hit);
    bool CheckLineSegmentPlane(const Vector3 &linestart, const Vector3 &line_end, const Vector3 &minAABB, const Vector3 &maxAABB, Vector3 &Hit);
    bool InBox(const Vector3 &Hit, const Vector3 &B1, const Vector3 &B2, const int &Axis);
    std::vector<GenericEntity*> *sceneObjectList;
    float angleX, angleY, angleZ;

private:
    static size_t zeID;
    
};
