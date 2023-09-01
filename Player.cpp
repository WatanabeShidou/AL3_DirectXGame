#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {
	worldTransform_.translation_ = position;
	assert(model);
	
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	playerHP_ = 100;
	input_ = Input::GetInstance();
	worldTransform3DReticle_.Initialize();
	worldTransform_.scale_.z = 2;

	uint32_t textureReticle = TextureManager::Load("target.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	uint32_t texture = TextureManager::Load("phonto.png");
	sprite2D = Sprite::Create(texture, {0, 0}, {1, 1, 1, 1}, {0, 0});
}

Vector3 Player::GetWorldPosition() {
	
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};

Vector3& operator*=(Vector3& v, float s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

const Vector3 operator*(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp *= s;
}

void Player::OnCollision() {
	playerHP_ -= 0.1f;
}

Player::~Player() { 
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}



void Player::Rotate() {
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_D)) {
		if (worldTransform_.rotation_.z >= -0.10) {
			worldTransform_.rotation_.z -= kRotSpeed;
		}
	} else if (worldTransform_.rotation_.z <= 0.0f) {
		worldTransform_.rotation_.z += kRotSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		if (worldTransform_.rotation_.z <= 0.10) {
			worldTransform_.rotation_.z += kRotSpeed;
		}
	} else if (worldTransform_.rotation_.z >= 0.0f) {
		worldTransform_.rotation_.z -= kRotSpeed;
	}
	
}

void Player::Update(const ViewProjection* viewProjection_) {
	
	const float kDistancePlayerTo3DReticle = 1.0f;

	Vector3 offset = {0, 0, 1.0f};
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	offset = Normalize(offset) * kDistancePlayerTo3DReticle;
	worldTransform3DReticle_.translation_.x = offset.x + GetWorldPosition().x;
	worldTransform3DReticle_.translation_.y = offset.y + GetWorldPosition().y;
	worldTransform3DReticle_.translation_.z = offset.z + GetWorldPosition().z;
	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();
	
	Vector3 positionReticle = worldTransform3DReticle_.translation_;

	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matViewProjectionViewport =
	    viewProjection_->matView * viewProjection_->matProjection * matViewport;

	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	Vector2 spritePosition{
	
	};	

	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	spritePosition.x = (float)mousePosition.x;
	spritePosition.y = (float)mousePosition.y;
	
	sprite2DReticle_->SetPosition(Vector2(spritePosition.x, spritePosition.y));

	Matrix4x4 matVPV = viewProjection_->matView * viewProjection_->matProjection * matViewport;
	Matrix4x4 matInverseVPV = Inverse(matVPV);
	Vector3 posNear = Vector3(spritePosition.x, spritePosition.y, 0);
	Vector3 posFar = Vector3(spritePosition.x, spritePosition.y, 1);

	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	Vector3 mouseDirection;
	mouseDirection.x = posNear.x + posFar.x;
	mouseDirection.y = posNear.y + posFar.y;
	mouseDirection.z = posNear.z + posFar.z;
	mouseDirection = Normalize(mouseDirection);
	const float kDistansTestObject = 60.0f;
	worldTransform3DReticle_.translation_.x = posNear.x + mouseDirection.x * kDistansTestObject;
	worldTransform3DReticle_.translation_.y = posNear.y + mouseDirection.y * kDistansTestObject;
	worldTransform3DReticle_.translation_.z = posNear.z + mouseDirection.z * kDistansTestObject;
	
	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();
	worldTransform_.TransferMatrix();
	//ImGui::Begin("Player");
	//ImGui::Text("PlayerHP(%f)", playerHP_);
	//ImGui::Text("2DReticle:(%f,%f)", spritePosition.x, spritePosition.y);
	//ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	//ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	//ImGui::Text(
	//    "3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	//    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	//ImGui::End();
	bullets_.remove_if([](PlayerBullet* bullet) { 
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	
	worldTransform_.TransferMatrix();
	
	Vector3 move = {0, 0, 0.1f};
	
	const float kCharacterSpeed = 0.15f;

	if (worldTransform_.translation_.x >= -15) {
		if (input_->PushKey(DIK_A)) {
			move.x -= kCharacterSpeed;
		}
	}
	if (worldTransform_.translation_.x <= 15) {
		if (input_->PushKey(DIK_D)) {
			move.x += kCharacterSpeed;
		}
	}
	if (worldTransform_.translation_.y <= 7.5) {

		if (input_->PushKey(DIK_W)) {
			move.y += kCharacterSpeed;
		}
	}
	if (worldTransform_.translation_.y >= -7.5) {
		if (input_->PushKey(DIK_S)) {
			move.y -= kCharacterSpeed;
		}
	}
	
	Attack();

	for (PlayerBullet* bullet : bullets_){
		bullet->Update();	
	}

	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	//worldTransform_.translation_.z += move.z;
	
	if (worldTransform_.translation_.z <= 30.0f) {
		worldTransform_.translation_.z += move.z;
	}
	
	worldTransform_.UpdateMatrix();
	
	/*worldTransform3DReticle_.translation_.z += move.z;
	if (worldTransform3DReticle_.translation_.z >= 80.0f) {
		worldTransform3DReticle_.translation_.z = 0.0f;
	}*/
	
	//ImGui::Begin("Debug1");
	//float inPutFloat[3] = {
	//    worldTransform_.translation_.x, worldTransform_.translation_.y,
	//    worldTransform_.translation_.z};
	//ImGui::SliderFloat3("Player", inPutFloat, -20.0f, 20.0f);
	//worldTransform_.translation_ = {inPutFloat[0], inPutFloat[1], inPutFloat[2]};
	//ImGui::End();

}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Attack() {
	
	
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		attackTimer_ += 1;
		if (attackTimer_ == 15) {

			const float kBulletSpeed = 2.0f;
			Vector3 velocity(0, 0, kBulletSpeed);
			// velocity = TransformNormal(velocity, worldTransform_.matWorld_);
			velocity.x = worldTransform3DReticle_.translation_.x - worldTransform_.translation_.x;
			velocity.y = worldTransform3DReticle_.translation_.y - worldTransform_.translation_.y;
			velocity.z = worldTransform3DReticle_.translation_.z - worldTransform_.translation_.z;
			velocity = Normalize(velocity) * kBulletSpeed;
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);

			bullets_.push_back(newBullet);
			attackTimer_ = 0;
		}
	} else {
		attackTimer_ = 0;
	}
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
void Player::DrawUI() {
	sprite2DReticle_->Draw();
	
}

void Player::Draw2D() { sprite2D->Draw(); }
