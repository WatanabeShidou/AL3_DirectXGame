#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("black.jpg");
	worldTransform_.Initialize();
	worldTransform_.scale_.x = 1;
	worldTransform_.scale_.y = 1;
	worldTransform_.scale_.z = 1;
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	
}

void EnemyBullet::OnCollision() { isDead_ = true; }

void EnemyBullet::Update() {
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y -= velocity_.y;
	worldTransform_.translation_.z -= velocity_.z;
	worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}


