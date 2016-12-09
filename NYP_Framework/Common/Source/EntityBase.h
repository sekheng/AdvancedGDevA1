#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"
#include "DetectMemoryLeak.h"

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
	void SetIsDone(const bool _value);

	// Check if this entity has a collider class parent
	virtual bool HasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	virtual void SetCollider(const bool _value);

protected:
	Vector3 position;
	Vector3 scale;

	bool isDone;
	bool m_bCollider;

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