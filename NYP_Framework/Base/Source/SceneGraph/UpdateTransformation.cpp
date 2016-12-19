#include "UpdateTransformation.h"
UpdateTransformation::UpdateTransformation()
	: curSteps(0)
	, deltaSteps(1)
	, minSteps(0)
	, maxSteps(0)
	, toContinue(false)
{
	Update_Mtx.SetToIdentity();
	Update_Mtx_REVERSED.SetToIdentity();
}
UpdateTransformation::~UpdateTransformation()
{
}

// Reset the transformation matrix to identity matrix
void UpdateTransformation::Reset(void)
{
	Update_Mtx.SetToIdentity();
	Update_Mtx_REVERSED.SetToIdentity();
}
// Update the steps
void UpdateTransformation::Update(void)
{
	if (minSteps != 0)
	{
	curSteps += deltaSteps;
		if ((curSteps >= maxSteps) || (curSteps <= minSteps))
		{
			deltaSteps *= -1;
		}
	}
	else if (minSteps == 0 && toContinue == false)
	{
		curSteps += deltaSteps;
		if ((curSteps >= maxSteps))
		{
			deltaSteps *= 0;
		}
	}
	else if (minSteps == 0 && toContinue == true)
	{
		curSteps += deltaSteps;
	}
}
// Apply a translation to the Update Transformation Matrix
void UpdateTransformation::ApplyUpdate(const float dx, const float dy, const float dz)
{
	Update_Mtx.SetToTranslation(dx, dy, dz);
	Update_Mtx_REVERSED.SetToTranslation(-dx, -dy, -dz);
}
// Apply a rotation to the Update Transformation Matrix
void UpdateTransformation::ApplyUpdate(const float angle, const float rx, const float
	ry, const float rz)
{
	Update_Mtx.SetToRotation(angle, rx, ry, rz);
	Update_Mtx_REVERSED.SetToRotation(-angle, rx, ry, rz);
}
// Set the minSteps and maxSteps
void UpdateTransformation::SetSteps(const int minSteps, const int maxSteps)
{
	this->minSteps = minSteps;
	this->maxSteps = maxSteps;
}
// Get the minSteps and maxSteps
void UpdateTransformation::GetSteps(int& minSteps, int& maxSteps)
{
	minSteps = this->minSteps;
	maxSteps = this->maxSteps;
}
// Get the direction of update
bool UpdateTransformation::GetDirection(void) const
{
	if (deltaSteps == -1)
		return false;
	return true;
}
// Get the Update_Mtx
Mtx44 UpdateTransformation::GetUpdateTransformation(void)
{
	if (deltaSteps == -1)
		return Update_Mtx_REVERSED;
	else if (deltaSteps == 0)
	{
		Update_Mtx.SetToIdentity();
		return Update_Mtx;
	}
	else
		return Update_Mtx;
}

void UpdateTransformation::setContinue(bool toggle)
{
	toContinue = toggle;
}