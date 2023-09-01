#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("cube.jpg");
	worldTransform_.Initialize();
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 0.5f;
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void PlayerBullet::OnCollision() { isDead_ = true; }

void PlayerBullet::Update() { 
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	worldTransform_.UpdateMatrix(); 
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) 
{	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
