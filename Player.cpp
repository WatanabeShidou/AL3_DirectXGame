#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	
	assert(model);
	
	model_ = model;
	textureHandle_ = textureHandle;
	
	worldTranceform_.Initialize();

}

void Player::Update() { worldTranceform_.TransferMatrix(); }

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTranceform_, viewProjection, textureHandle_);
}
