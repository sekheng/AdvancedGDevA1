#pragma once
#include "Mtx44.h"
#include "UpdateTransformation.h"
#include "UpdateRotation.h"
class Transform
{
protected:
	Mtx44 Mtx, DefaultMtx;
	UpdateTransformation* theUpdateTransformation;
	UpdateRotation* theUpdateRotation;
public:
	// Default Constructor
	Transform(void);
	// Overloaded Constructor
	Transform(const float dx, const float dy, const float dz);
	// Destructor
	~Transform(void);
	enum AXIS
	{
		X_AXIS = 0,
		Y_AXIS,
		Z_AXIS,
		NUM_AXIS
	};
	// Apply a translation to the Transformation Matrix
	void ApplyTranslate(const float dx, const float dy, const float dz);
	// Get the translation from the Transformation Matrix
	void GetTranslate(float& x, float& y, float& z);
	// Apply a rotation to the Transformation Matrix
	void ApplyRotate(const float angle, const float rx, const float ry, const float
		rz);
	// Get the rotation from the Transformation Matrix
	float GetRotate(const AXIS theAxis);
	// Set the scale of the Transformation Matrix
	void SetScale(const float sx, const float sy, const float sz);
	// Get the scale from the Transformation Matrix
	void GetScale(float& x, float& y, float& z);
	// Apply a Transformation Matrix to the Transformation Matrix here
	void ApplyTransform(Mtx44 newMTX);
	// Reset the transformation matrix to identity matrix
	void Reset(void); //reset to identity
	// Get the transformation matrix
	Mtx44 GetTransform(void);
	void SetUpdateTransformation(UpdateTransformation *theUpdateTransformation = NULL);
	Mtx44 GetUpdateTransform();

	//Mtx44 GetRotation(void);
	void SetUpdateRotation(UpdateRotation *theUpdateRotation = NULL);
	Mtx44 GetUpdateRotation();
	// Print Self
	void PrintSelf(void) const;

	Vector3 getPosition();

};