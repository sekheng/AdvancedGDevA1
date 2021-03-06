#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H

#include "CameraBase.h"

class FPSCamera : public CameraBase
{
public:
	FPSCamera();
	virtual ~FPSCamera();

    Mtx44 GetViewMatrix();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

	Vector3 &GetCameraPos();
    void SetCameraPos(Vector3 pos);
	Vector3 &GetCameraTarget();
	void SetCameraTarget(Vector3 pos);
	Vector3 &GetCameraUp();
	void SetCameraUp(Vector3 pos);
	Vector3 &GetCameraRotation();
	void SetCameraRotation(Vector3 Rotation);
	Vector3 &GetCameraRight();

private:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	Vector3 rotation;
	Vector3 right;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
};

#endif