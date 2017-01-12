#include "Transform.h"
#include <iostream>
#include "GraphicsManager.h"
using namespace std;
// Default Constructor
Transform::Transform(void)
	: theUpdateTransformation(NULL)
	, theUpdateRotation(NULL)
{
	Mtx.SetToIdentity();
	DefaultMtx.SetToIdentity();
}
// Overloaded Constructor
Transform::Transform(const float dx, const float dy, const float dz)
{
	Mtx.SetToTranslation(dx, dy, dz);
}
// Destructor
Transform::~Transform(void)
{
	if (theUpdateTransformation)
	{
		delete theUpdateTransformation;
		theUpdateTransformation = NULL;
	}
	if (theUpdateRotation)
	{
		delete theUpdateRotation;
		theUpdateRotation = NULL;
	}
}

void Transform::deleteUpdateTransform()
{
	if (theUpdateTransformation)
	{
		delete theUpdateTransformation;
		theUpdateTransformation = NULL;
	}
}
// Apply a translation to the Transformation Matrix
void Transform::ApplyTranslate(const float dx, const float dy, const float dz)
{
	Mtx44 TempMtx;
	TempMtx.SetToTranslation(dx, dy, dz);
	Mtx = Mtx*TempMtx;
}
// Get the translation from the Transformation Matrix
void Transform::GetTranslate(float& x, float& y, float& z)
{
	x = Mtx.a[12];
	y = Mtx.a[13];
	z = Mtx.a[14];
}
// Apply a rotation to the Transformation Matrix
void Transform::ApplyRotate(const float angle, const float rx, const float ry, const
	float rz)
{
	Mtx44 TempMtx;
	TempMtx.SetToRotation(angle, rx, ry, rz);
	Mtx = Mtx*TempMtx;
}
// Get the rotation from the Transformation Matrix
float Transform::GetRotate(const AXIS theAxis)
{
	if (theAxis == X_AXIS)
	{
		float y = acos(Mtx.a[5]); // Equals to cos a
		float z = asin(-Mtx.a[9]); // Equals to sin a
		if (abs(y - z) < Math::EPSILON)
			return y;
	}
	else if (theAxis == Y_AXIS)
	{
		float x = acos(Mtx.a[0]); // Equals to cos a
		float z = asin(Mtx.a[8]); // Equals to -sin a
		if (abs(x - z) < Math::EPSILON)
			return x;
	}
	else // Z_AXIS
	{
		float x = acos(Mtx.a[0]); // Equals to cos a
		float y = asin(-Mtx.a[4]); // Equals to -sin a
		if (abs(x - y) < Math::EPSILON)
			return x;
	}
	return 0.0f;
}
// Set the scale of the Transformation Matrix
void Transform::SetScale(const float sx, const float sy, const float sz)
{
	float scaleX = sx, scaleY = sy, scaleZ = sz;
	if (scaleX <= 0.0f)
		scaleX = 1.0f;
	if (scaleY <= 0.0f)
		scaleY = 1.0f;
	if (scaleZ <= 0.0f)
		scaleZ = 1.0f;
	Mtx.SetToScale(scaleX, scaleY, scaleZ);
}
// Get the scale from the Transformation Matrix
void Transform::GetScale(float& x, float& y, float& z)
{
	x = Mtx.a[0];
	y = Mtx.a[5];
	z = Mtx.a[10];
}
// Apply a Transformation Matrix to the Transformation Matrix here
void Transform::ApplyTransform(Mtx44 newMTX)
{
	Mtx = Mtx * newMTX;
}
// Reset the transformation matrix to identity matrix
void Transform::Reset(void)
{
	Mtx.SetToIdentity();
}
// Get the transformation matrix
Mtx44 Transform::GetTransform(void)
{
	return Mtx;
}
// Print Self
void Transform::PrintSelf(void) const
{
	cout << "======================================================================"
		<< endl;
	cout << "Transform::PrintSelf" << endl;
	cout << "----------------------------------------------------------------------"
		<< endl;
	cout << "[\t" << Mtx.a[0] << "\t" << Mtx.a[4] << "\t" << Mtx.a[8] << "\t" <<
		Mtx.a[12] << "\t]" << endl;
	cout << "[\t" << Mtx.a[1] << "\t" << Mtx.a[5] << "\t" << Mtx.a[9] << "\t" <<
		Mtx.a[13] << "\t]" << endl;
	cout << "[\t" << Mtx.a[2] << "\t" << Mtx.a[6] << "\t" << Mtx.a[10] << "\t" <<
		Mtx.a[14] << "\t]" << endl;
	cout << "[\t" << Mtx.a[3] << "\t" << Mtx.a[7] << "\t" << Mtx.a[11] << "\t" <<
		Mtx.a[15] << "\t]" << endl;
	cout << "======================================================================"
		<< endl;
}

void Transform::SetUpdateTransformation(UpdateTransformation *theUpdateTransformation)
{
	this->theUpdateTransformation = theUpdateTransformation;
}
Mtx44 Transform::GetUpdateTransform()
{
	if (theUpdateTransformation == NULL)
	{
		return DefaultMtx;
	}
	theUpdateTransformation->Update();

	return theUpdateTransformation->GetUpdateTransformation();
}

void Transform::SetUpdateRotation(UpdateRotation *theUpdateRotation)
{
	this->theUpdateRotation = theUpdateRotation;
}
Mtx44 Transform::GetUpdateRotation()
{
	if (theUpdateRotation == NULL)
	{
		return DefaultMtx;
	}
	theUpdateRotation->Update();

	return theUpdateRotation->GetUpdateRotation();
}

Vector3 Transform::getPosition()
{
	Vector3 position;

	position.x = (Mtx.a[12]);
	position.y = Mtx.a[13];
	position.z = Mtx.a[14];
	//position = GetUpdateRotation() * position;
	/*MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();

	modelStack.MultMatrix(this->GetTransform());
	MS& modelStack2 = GraphicsManager::GetInstance()->GetModelStack();
	modelStack2.PushMatrix();
	modelStack2.Translate(position.x, position.y, position.z);
	position.x = modelStack2.Top().a[12];
	position.y = modelStack2.Top().a[13];
	position.z = modelStack2.Top().a[14];
	modelStack2.PopMatrix();
	modelStack.PopMatrix();*/
	/*if (theUpdateRotation != NULL)
	{*/
	/*position.x = (Mtx.a[12]);
	position.y = Mtx.a[13];
	position.z = Mtx.a[14];
	position = Mtx * position;*/
		/*position.x = (Mtx.a[12] * theUpdateRotation->GetUpdateRotation().a[0]) + (Mtx.a[13] * theUpdateRotation->GetUpdateRotation().a[4]) + (Mtx.a[14] * theUpdateRotation->GetUpdateRotation().a[8]);
		position.y = (Mtx.a[12] * theUpdateRotation->GetUpdateRotation().a[1]) + (Mtx.a[13] * theUpdateRotation->GetUpdateRotation().a[5]) + (Mtx.a[14] * theUpdateRotation->GetUpdateRotation().a[9]);
		position.z = (Mtx.a[12] * theUpdateRotation->GetUpdateRotation().a[2]) + (Mtx.a[13] * theUpdateRotation->GetUpdateRotation().a[6]) + (Mtx.a[14] * theUpdateRotation->GetUpdateRotation().a[10]);*/
	//}
	/*else
	{
		position.x = (Mtx.a[12]);
		position.y = Mtx.a[13];
		position.z = Mtx.a[14];
	}*/
	//position += 
	return position;
}