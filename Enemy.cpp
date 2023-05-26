#include "Enemy.h"
#include <cassert>
#include "Player.h"

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("cube.jpg");
	worldTranceform_.Initialize();

	worldTranceform_.translation_ = position;
	timer_ = 0;
	
}

void Enemy::Update() {
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	worldTranceform_.UpdateMatrix();
	
	/*switch(phase_) { 
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
	}*/
	Attack();

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTranceform_, viewProjection, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Attack() {
	assert(player_);
	timer_ += 1;
	
	if (timer_ == 60)	
	{
		const float kBulletSpeed = 1.0f;
		end = player_->GetWorldPosition();
		start = GetWorldPosition();
		sub.x = start.x - end.x;
		sub.y = start.y - end.y;
		sub.z = start.z - end.z;

		float length = sqrt(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z);
		Vector3 dir(sub.x / length, sub.y / length, sub.z / length);
		Vector3 velocity(dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed);
		
		velocity = TransformNormal(velocity, worldTranceform_.matWorld_);

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTranceform_.translation_, velocity);

		bullets_.push_back(newBullet);
		timer_ = 0;
	}
}
