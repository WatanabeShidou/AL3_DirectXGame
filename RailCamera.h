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

	void Rotate();

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	Vector3 move = {0.01f, 0.f, 0.1f};
	Vector3 Rot = {0.f, 0.001f, 0.f};
	//Input* input_ = nullptr;
	BYTE key[256];
	WorldTransform worldTransform_;
private:
	
	ViewProjection viewProjection_;
};
