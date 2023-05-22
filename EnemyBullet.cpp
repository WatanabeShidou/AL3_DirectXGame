#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("black.jpg");
	worldTranceform_.Initialize();

	worldTranceform_.translation_ = position;
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	worldTranceform_.translation_.x -= velocity_.x;
	worldTranceform_.translation_.y -= velocity_.y;
	worldTranceform_.translation_.z -= velocity_.z;
	worldTranceform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTranceform_, viewProjection, textureHandle_);
}
