#pragma once

#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "EnemyBullet.h"
#include <list>
#include "Mathutility.h"

class GameScene;
class Player;
/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	
	void OnCollision();
	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;
	
	int timer_;
	Vector3 start;
	Vector3 end;
	Vector3 sub;
	enum class Phase {
		start,
		Approach,
		Leave,
	};
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	void Initialize(Model* model, const Vector3& position);
	Vector3 velocity_;
	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition() {
		Vector3 worldPos;

		worldPos.x = worldTranceform_.translation_.x;
		worldPos.y = worldTranceform_.translation_.y;
		worldPos.z = worldTranceform_.translation_.z;

		return worldPos;
	};
	void SetGameScene(GameScene* gamescene) { gamescene_ = gamescene; }
	bool GetisDead() { return isDead_; } bool isDead_ = false;
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
	GameScene* gamescene_ = nullptr;
	Model* model_ = nullptr;
	Player* player_ = nullptr;
	uint32_t textureHandle_ = 0;
	EnemyBullet* bullet_ = nullptr;
	Phase phase_ = Phase::start;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	
};
