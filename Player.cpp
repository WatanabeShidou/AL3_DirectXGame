#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"

void Player::Initialize(Model* model, uint32_t textureHandle,Vector3 position) {
	worldTranceform_.translation_ = position;
	assert(model);
	
	model_ = model;
	textureHandle_ = textureHandle;
	
	worldTranceform_.Initialize();
	input_ = Input::GetInstance();
}

Vector3 Player::GetWorldPosition() {
	
	Vector3 worldPos;
	worldPos.x = worldTranceform_.matWorld_.m[3][0];
	worldPos.y = worldTranceform_.matWorld_.m[3][1];
	worldPos.z = worldTranceform_.matWorld_.m[3][2];

	return worldPos;
};




void Player::OnCollision() {}

Player::~Player() { 
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Rotate() {
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTranceform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTranceform_.rotation_.y += kRotSpeed;
	}
}

void Player::Update() {
	bullets_.remove_if([](PlayerBullet* bullet) { 
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	
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

	for (PlayerBullet* bullet : bullets_){
		bullet->Update();	
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

	worldTranceform_.UpdateMatrix();

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
	 
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		
		velocity = TransformNormal(velocity, worldTranceform_.matWorld_);
		
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		bullets_.push_back(newBullet);
	}
}

void Player::SetParent(const WorldTransform* parent) { worldTranceform_.parent_ = parent; }