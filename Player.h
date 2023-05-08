﻿#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "PlayerBullet.h"


/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="textureHandle_"></param>
	void Initialize(Model* model, uint32_t textureHandle);
	
	Input* input_ = nullptr;
	

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

private:
	WorldTransform worldTranceform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0;

	PlayerBullet* bullet_ = nullptr;
};
