#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"

/// <summary>
/// 敵の玉
/// </summary>
class EnemyBullet {
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
	void OnCollision();
	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;
	Vector3 GetWorldPosition() {
		Vector3 worldPos;

		worldPos.x = worldTranceform_.translation_.x;
		worldPos.y = worldTranceform_.translation_.y;
		worldPos.z = worldTranceform_.translation_.z;

		return worldPos;
	};
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

