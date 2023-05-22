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
	
	switch(phase_) { 
	case Phase::Approach:
	default:
		worldTranceform_.translation_.z -= 0.3f;
		if (worldTranceform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		worldTranceform_.translation_.y += 0.3f;
		worldTranceform_.translation_.x -= 0.3f;
		break;
	}

}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTranceform_, viewProjection, textureHandle_);
}
