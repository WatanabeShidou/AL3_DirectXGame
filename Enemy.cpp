#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("cube.jpg");
	worldTranceform_.Initialize();

	worldTranceform_.translation_ = position;
}

void Enemy::Update() {
	
	worldTranceform_.UpdateMatrix();
	worldTranceform_.translation_.z -= 1;
}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTranceform_, viewProjection, textureHandle_);
}
