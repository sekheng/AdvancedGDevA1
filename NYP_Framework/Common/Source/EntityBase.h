#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"
#include "DetectMemoryLeak.h"
#include <string>

class EntityBase
{
public:
	EntityBase();
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI();

	inline void SetPosition(const Vector3& _value){ position = _value; };
	inline Vector3 GetPosition(){ return position; };

	inline void SetScale(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScale(){ return scale; };

	bool IsDone();
	void SetIsDone(const bool &_value);

	bool IsShot();
	void SetIsShot(const bool &_value);

	// Check if this entity has a collider class parent

    virtual bool onNotify(EntityBase &zeEvent);
    virtual bool onNotify(const float &zeEvent);
    virtual bool onNotify(const float &zeEvent1, const float &zeEvent2);
    virtual bool onNotify(const std::string &zeEvent);
    virtual bool onNotify(const int &zeEvent);
    virtual bool onNotify(const int &zeEvent1, const int &zeEvent2);
    virtual bool onNotify(const Vector3 &zeEvent);
    virtual bool onNotify(const Vector3 &zeEvent1, const Vector3 &zeEvent2);

    bool CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
    bool CheckOverlap(const Vector3 &thisMinAABB, const Vector3 &thisMaxAABB, const Vector3 &thatMinAABB, const Vector3 &thatMaxAABB);

	bool HasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	void SetCollider(const bool &_value);

    void setName(const std::string &zeStr);
    const std::string &getName();

    bool operator==(const EntityBase &rhs);
    bool operator!=(const EntityBase &rhs);



protected:
	Vector3 position;
	Vector3 scale;

	bool isDone,isShot;
	bool m_bCollider;
    std::string name_;
    //union Vector3Union
    //{
    //    int ActualValue;                        // ^^^^^^^^ ^^^^^^^^ ^^^^^^^^ ^^^^^^^^ This is all Actual Value
    //    short highBytes, lowBytes;     //  ^^^^^^^^ ^^^^^^^^ // This is highBytes
    //                                                    //                                ^^^^^^^^ ^^^^^^^^ Low bytes
    //    char data[4];                           // ^^^^^^^^ // This is data[0]

    //};
    //Vector3Union TryingIt;
};

#endif // ENTITY_BASE_H