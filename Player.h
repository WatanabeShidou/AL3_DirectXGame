#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "Mathutility.h"
#include <Sprite.h>
#include <cassert>
#include "Collider.h"


/// <summary>
/// 自キャラ
/// </summary>
class Player : public Collider{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="textureHandle_"></param>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);
	//Vector3 GetWorldPosition();
	Vector3 GetWorldPosition() override;
	//void OnCollision();
	void OnCollision() override;
	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;
	Input* input_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	const std::list<PlayerBullet*>& GetBullet() { return bullets_; }
	int attackTimer_;

	Vector3 Normalize(const Vector3& v) {
		Vector3 result;
		result.x = (v.x / sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
		result.y = (v.y / sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
		result.z = (v.z / sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
		return result;
	}
	
	Matrix4x4 MakeViewportMatrix(
	    float left, float top, float width, float height, float minDepth, float maxDepth) {
		Matrix4x4 result;
		result.m[0][0] = width / 2.0f;
		result.m[0][1] = 0.0f;
		result.m[0][2] = 0.0f;
		result.m[0][3] = 0.0f;

		result.m[1][0] = 0.0f;
		result.m[1][1] = -height / 2.0f;
		result.m[1][2] = 0.0f;
		result.m[1][3] = 0.0f;

		result.m[2][0] = 0.0f;
		result.m[2][1] = 0.0f;
		result.m[2][2] = maxDepth - minDepth;
		result.m[2][3] = 0.0f;

		result.m[3][0] = left + (width / 2.0f);
		result.m[3][1] = top + (height / 2.0f);
		result.m[3][2] = minDepth;
		result.m[3][3] = 1.0f;
		return result;
	}

	WorldTransform worldTransform3DReticle_;
	float playerHP_;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 
	/// </summary>
	void DrawUI();
	void Draw2D();
	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update(const ViewProjection* viewProjection_);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	void SetParent(const WorldTransform* parent);
	Vector3 Transform(const Vector3& vecter, const Matrix4x4& matrix) {
		Vector3 result;
		result.x = float(
		    vecter.x * matrix.m[0][0] + vecter.y * matrix.m[1][0] + vecter.z * matrix.m[2][0] +
		    1.0f * matrix.m[3][0]);
		result.y = float(
		    vecter.x * matrix.m[0][1] + vecter.y * matrix.m[1][1] + vecter.z * matrix.m[2][1] +
		    1.0f * matrix.m[3][1]);
		result.z = float(
		    vecter.x * matrix.m[0][2] + vecter.y * matrix.m[1][2] + vecter.z * matrix.m[2][2] +
		    1.0f * matrix.m[3][2]);
		float w = vecter.x * matrix.m[0][3] + vecter.y * matrix.m[1][3] +
		          vecter.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
		assert(w != 0.0f);
		result.x /= w;
		result.y /= w;
		result.z /= w;
		return result;
	}
	WorldTransform worldTransform_;
private:
	
	Model* model_ = nullptr;
	Sprite* sprite2DReticle_ = nullptr;
	Sprite* sprite2D = nullptr;
	uint32_t textureHandle_ = 0;
	PlayerBullet* bullet_ = nullptr;
	
};
