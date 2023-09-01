#pragma once

#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "EnemyBullet.h"
#include <list>
#include "Mathutility.h"
#include "Collider.h"
#include <sstream>
#include "Audio.h"
#include "DirectXCommon.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "AxisIndicator.h"
#include <iostream>
#include <fstream>

class GameScene;
class Player;
/// <summary>
/// 敵
/// </summary>
class Enemy : public Collider {
public:
	
	//void OnCollision();
	void OnCollision() override;
	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;
	int enemyHP_;
	int timer_;
	int phaseNumber_;
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
	int i;
	void SetEnemyHP(int enemyHP) { enemyHP_ = enemyHP; }
	//Vector3 GetWorldPosition();
	//void OnCollision() override;
	Vector3 GetWorldPosition() override {
		Vector3 worldPos;
		worldPos.x = worldTransform_.translation_.x;
		worldPos.y = worldTransform_.translation_.y;
		worldPos.z = worldTransform_.translation_.z;
		return worldPos;
	};
	int isEnemyAlive_[20];

	void SetEnemyAlive(int alive) { isEnemyAlive_[i] = alive; }
	void SetGameScene(GameScene* gamescene) { gamescene_ = gamescene; }
	bool GetisDead() { return isDead_; } 
	bool isDead_ = false;
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
	int attackMode_;
	int phaseTimer_;
	float easeInQuad(float t, float b, float c, float d); 
	float ta;
	float ba;
	float ca;
	float da;
	float Theta = 0;
	float amplitude = 32.0f;
	int enemyMove_;
	int enemyAttackCount_;
	float k = -20;
	void AddBullet(Vector3 pos ,Vector3 velo);
	std::list<EnemyBullet*> bullets_;
	std::stringstream enemyBulletCommands;

private:
	WorldTransform worldTransform_;
	GameScene* gamescene_ = nullptr;
	Model* model_ = nullptr;
	Player* player_ = nullptr;
	uint32_t textureHandle_ = 0;
	uint32_t textureHandle2_ = 0;
	EnemyBullet* bullet_ = nullptr;
	Phase phase_ = Phase::start;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	

};
