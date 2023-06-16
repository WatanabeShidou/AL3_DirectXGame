#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 trans, Vector3 rot);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTranceform_; }

	Vector3 move = {0.f, 0.f, 0.f};
	Vector3 Rot = {0.f, 0.001f, 0.f};

private:
	WorldTransform worldTranceform_;

	ViewProjection viewProjection_;
};
