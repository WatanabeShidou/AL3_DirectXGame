#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"


class PlayerBullet {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	Vector3 velocity_;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
	bool IsDead() const { return isDead_; }
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTranceform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0;
};
