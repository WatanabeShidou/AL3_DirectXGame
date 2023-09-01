#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"


class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);
	Vector3 move = {0.f, 0.f, 0.1f};

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
};
