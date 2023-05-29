#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "Mathutility.h"


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
	Vector3 GetWorldPosition(){
		Vector3 worldPos;

		worldPos.x = worldTranceform_.translation_.x;
		worldPos.y = worldTranceform_.translation_.y;
		worldPos.z = worldTranceform_.translation_.z;

		return worldPos;
	};
	void OnCollision();
	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;
	Input* input_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	const std::list<PlayerBullet*>& GetBullet() { return bullets_; }
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

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
