#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	
	assert(model);
	
	model_ = model;
	textureHandle_ = textureHandle;
	
	worldTranceform_.Initialize();
	input_ = Input::GetInstance();
}

Player::~Player() { delete bullet_; }

void Player::Rotate() {
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTranceform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTranceform_.rotation_.y += kRotSpeed;
	}
}

void Player::Update() {
	worldTranceform_.TransferMatrix();
	
	Vector3 move = {0, 0, 0};
	
	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	
	Attack();

	if (bullet_) {
		bullet_->Update();	
	}

	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

	worldTranceform_.translation_.x = max(worldTranceform_.translation_.x, -kMoveLimitX);
	worldTranceform_.translation_.x = min(worldTranceform_.translation_.x, +kMoveLimitX);
	worldTranceform_.translation_.y = max(worldTranceform_.translation_.y, -kMoveLimitY);
	worldTranceform_.translation_.y = min(worldTranceform_.translation_.y, +kMoveLimitY);

	worldTranceform_.translation_.x += move.x;
	worldTranceform_.translation_.y += move.y;
	worldTranceform_.translation_.z += move.z;

	worldTranceform_.matWorld_ = MakeAffineMatrix(
	    worldTranceform_.scale_, 
		worldTranceform_.rotation_,
		worldTranceform_.translation_);
	
	
	ImGui::Begin("Debug1");
	float inPutFloat[3] = {
	    worldTranceform_.translation_.x, worldTranceform_.translation_.y,
	    worldTranceform_.translation_.z};
	ImGui::SliderFloat3("Player", inPutFloat, -20.0f, 20.0f);
	worldTranceform_.translation_ = {inPutFloat[0], inPutFloat[1], inPutFloat[2]};
	ImGui::End();



}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTranceform_, viewProjection, textureHandle_);
	 
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_Z)) {
		PlayerBullet* newBullet = new PlayerBullet();
		if (bullet_) {
			delete bullet_;
			bullet_ = nullptr;
		}
		
		newBullet->Initialize(model_, worldTranceform_.translation_);

		bullet_ = newBullet;
	}
}
