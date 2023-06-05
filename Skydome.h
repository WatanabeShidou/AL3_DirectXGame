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

private:
	WorldTransform worldTranceform_;

	Model* model_ = nullptr;
};
