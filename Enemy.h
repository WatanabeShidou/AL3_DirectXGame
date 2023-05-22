﻿#pragma once

#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "EnemyBullet.h"
#include <list>
#include "Mathutility.h"

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	int timer_;
	enum class Phase {
		start,
		Approach,
		Leave,
	};
	~Enemy();
	std::list<EnemyBullet*> bullets_;
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	void Initialize(Model* model, const Vector3& position);
	Vector3 velocity_;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	void Attack();

private:
	WorldTransform worldTranceform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0;
	EnemyBullet* bullet_ = nullptr;
	Phase phase_ = Phase::start;
};
