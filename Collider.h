#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
class Collider {
private:
	const float radius_ = 1.0f;

public:
	const float GetRadius() { return radius_; }
	void SetRadius();
	virtual void OnCollision();
	virtual Vector3 GetWorldPosition();
};
