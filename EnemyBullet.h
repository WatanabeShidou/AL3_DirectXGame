#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "Collider.h"

/// <summary>
/// 敵の玉
/// </summary>
class EnemyBullet : public Collider {
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
	//void OnCollision();
	void OnCollision() override;
	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;
	//Vector3 GetWorldPosition();
	Vector3 GetWorldPosition() override {
		Vector3 worldPos;

		worldPos.x = worldTransform_.translation_.x;
		worldPos.y = worldTransform_.translation_.y;
		worldPos.z = worldTransform_.translation_.z;

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
	WorldTransform worldTransform_;
	

private:
	

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0;
};

