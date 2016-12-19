#include "UpdateRotation.h"
UpdateRotation::UpdateRotation()
	: curSteps(0)
	, deltaSteps(1)
	, minSteps(0)
	, maxSteps(0)
	, toContinue(false)
{
	Update_Mtx.SetToIdentity();
	Update_Mtx_REVERSED.SetToIdentity();
}
UpdateRotation::~UpdateRotation()
{
}

// Reset the transformation matrix to identity matrix
void UpdateRotation::Reset(void)
{
	Update_Mtx.SetToIdentity();
	Update_Mtx_REVERSED.SetToIdentity();
}
// Update the steps
void UpdateRotation::Update(void)
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
// Apply a rotation to the Update Transformation Matrix
void UpdateRotation::ApplyUpdate(const float angle, const float rx, const float
	ry, const float rz)
{
	Update_Mtx.SetToRotation(angle, rx, ry, rz);
	Update_Mtx_REVERSED.SetToRotation(-angle, rx, ry, rz);
}
// Set the minSteps and maxSteps
void UpdateRotation::SetSteps(const int minSteps, const int maxSteps)
{
	this->minSteps = minSteps;
	this->maxSteps = maxSteps;
}
// Get the minSteps and maxSteps
void UpdateRotation::GetSteps(int& minSteps, int& maxSteps)
{
	minSteps = this->minSteps;
	maxSteps = this->maxSteps;
}
// Get the direction of update
bool UpdateRotation::GetDirection(void) const
{
	if (deltaSteps == -1)
		return false;
	return true;
}
// Get the Update_Mtx
Mtx44 UpdateRotation::GetUpdateRotation(void)
{
	if (deltaSteps == -1)
		return Update_Mtx_REVERSED;
	return Update_Mtx;
}

void UpdateRotation::setContinue(bool toggle)
{
	toContinue = toggle;
}