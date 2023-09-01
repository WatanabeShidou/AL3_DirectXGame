#include "Enemy.h"
#include <cassert>
#include "Player.h"
#include "GameScene.h"
#include <stdlib.h>
#include "ImGuiManager.h"
#include <math.h>


void Enemy::OnCollision() {
	enemyHP_ -= 10;
	
	if (enemyHP_ == 0) {
		
		isDead_ = true;
	}
}

Enemy::~Enemy() {
	
}

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("cube.jpg");
	textureHandle2_ = TextureManager::Load("black.jpg");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	ta = 0;
	ba = 0.7f;
	ca = 2;
	da = 0.7f;
	timer_ = 0;
	attackMode_ = 0;
	phaseNumber_ = 0;
	phaseTimer_ = 0;
	enemyMove_ = 1;
	enemyAttackCount_ = 0;
}


void Enemy::Update() {
	
	worldTransform_.UpdateMatrix();
	
	if (isDead_ == true) {
		
	}
	switch (phase_) {
	case Phase::Approach:
	default:
		if (gamescene_->phaseNumber_ < 5) {

			phaseTimer_ += 1;
			if (phaseTimer_ <= 80) {

				if (worldTransform_.translation_.x <= 50.0f) {
					worldTransform_.translation_.x += 0.5f;

				} else if (worldTransform_.translation_.x >= 50.0f) {
					enemyHP_ = 0;
				}
				if (enemyHP_ == 0) {
					isDead_ = true;
				}
			}
			if (phaseTimer_ >= 200) {
				worldTransform_.translation_.x -= 0.5f;
				worldTransform_.translation_.y -= 0.5f;
				if (phaseTimer_ >= 260) {
					phaseTimer_ = 0;
				}
			}
			if (worldTransform_.translation_.y <= -34) {
				isDead_ = true;
			}
		}
		if (gamescene_->phaseNumber_ >= 15 && gamescene_->phaseNumber_ < 25) {
			phaseTimer_ += 1;
			if (phaseTimer_ <= 120) {

				worldTransform_.translation_.x += 0.5f;
			}
		}
		if (gamescene_->phaseNumber_ >= 25 && gamescene_->phaseNumber_ < 105) {
			phaseTimer_ += 1;
			if (phaseTimer_ >= 150) {
				worldTransform_.translation_.z -= 0.20f;
			}
			if (worldTransform_.translation_.z <= -10) {
				phaseTimer_ = 0;
				
				isDead_ = true;
			}
		}

		if (gamescene_->phaseNumber_ == 105) {
			//if (enemyAttackCount_ <= 2) {

				if (enemyMove_ == 1) {
					if (worldTransform_.translation_.x >= 50.0f) {
						ta = 0;
						enemyAttackCount_ += 1;
						enemyMove_ = 2;
						timer_ = 0;
					}
				}
				if (enemyMove_ == 2) {
					if (worldTransform_.translation_.x <= -50.0f) {
						ta = 0;
						enemyAttackCount_ += 1;
						enemyMove_ = 1;
						timer_ = 0;
					}
				}
				if (enemyMove_ == 1) {

					ta += 0.003f;
					worldTransform_.translation_.x += easeInQuad(ta, ba, ca, da);
					worldTransform_.translation_.y = sinf(Theta) * (amplitude - 10);
					Theta += 3.14f / 180.0f;
				}
				if (enemyMove_ == 2) {

					ta += 0.003f;
					worldTransform_.translation_.x -= easeInQuad(ta, ba, ca, da);
					worldTransform_.translation_.y = sinf(Theta) * (amplitude - 10);
					Theta += 3.14f / 180.0f;
				}
			//}
		}

		break;
	
	case Phase::Leave:
		
		
		break;
	}
	Attack();
	//if (--deathTimer_ <= 0) {
		//isDead_ = true;
	//}

	//ImGui::Begin("Enemy");
	//ImGui::Text("HP:%d", enemyHP_);
	//ImGui::Text("Attaccount:%d", enemyAttackCount_);
	//ImGui::Text("timer:%d", timer_);
	//ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Attack() {
	//if (isDead_ == true) {
		
	//}
	assert(player_);
	//phaseNumber_ += 1;
	timer_ += 1;

	if (gamescene_->phaseNumber_ == 105) {
		if (attackMode_ == 0) {
			    if (timer_ == 30) {
				    const float kBulletSpeed = 1.0f;
				    end = player_->GetWorldPosition();
				    start = GetWorldPosition();
				    sub.x = start.x - end.x;
				    sub.y = start.y - end.y;
				    sub.z = start.z - end.z;

				    float length = sqrt(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z);
				    Vector3 dir(sub.x / length, sub.y / length, sub.z / length);
				    Vector3 velocity(
				        dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed);

				    velocity = TransformNormal(velocity, worldTransform_.matWorld_);

				    EnemyBullet* newBullet = new EnemyBullet();

				    newBullet->Initialize(model_, worldTransform_.translation_, velocity);

				    gamescene_->AddEnemyBullet(newBullet);

				    timer_ = 0;
				    attackMode_ = 1;
			    }
		}
		if (attackMode_ == 1) {
			    if (timer_ == 30) {
				    const float kBulletSpeed = 2.0f;
				    end = player_->GetWorldPosition();
				    start = GetWorldPosition();
				    sub.x = start.x - end.x;
				    sub.y = start.y - end.y;
				    sub.z = start.z - end.z;

				    float length = sqrt(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z);
				    Vector3 dir(sub.x / length, sub.y / length, sub.z / length);
				    Vector3 velocity(
				        dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed);

				    velocity = TransformNormal(velocity, worldTransform_.matWorld_);

				    EnemyBullet* newBullet = new EnemyBullet();

				    newBullet->Initialize(model_, worldTransform_.translation_, velocity);

				    gamescene_->AddEnemyBullet(newBullet);
				    timer_ = 0;
				    attackMode_ = 2;
			    }
		}
			    if (attackMode_ == 2) {
				    if (timer_ == 60) {
					    const float kBulletSpeed = 0.50f;
					    end = player_->GetWorldPosition();
					    start = GetWorldPosition();
					    sub.x = (start.x + rand() % 20 - 10) - end.x;
					    sub.y = (start.y + rand() % 20 - 10) - end.y;
					    sub.z = start.z - end.z;

					    float length = sqrt(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z);
					    Vector3 dir(sub.x / length, sub.y / length, sub.z / length);
					    Vector3 velocity(
					        dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed);

					    velocity = TransformNormal(velocity, worldTransform_.matWorld_);

					    EnemyBullet* newBullet = new EnemyBullet();

					    newBullet->Initialize(model_, worldTransform_.translation_, velocity);

					    gamescene_->AddEnemyBullet(newBullet);
					    timer_ = 0;
					    attackMode_ = 0;
				    }
			    }
		
	}


	if (worldTransform_.translation_.x <= 50.0f && worldTransform_.translation_.x >= -50.0f) {
		if (worldTransform_.translation_.y <= 34.0f && worldTransform_.translation_.y >= -34.0f) {
			if (gamescene_->phaseNumber_ >= 0 && gamescene_->phaseNumber_ < 5) {
				if (attackMode_ == 0) {
					if (timer_ == 30) {
						const float kBulletSpeed = 1.0f;
						end = player_->GetWorldPosition();
						start = GetWorldPosition();
						sub.x = start.x - end.x;
						sub.y = start.y - end.y;
						sub.z = start.z - end.z;

						float length = sqrt(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z);
						Vector3 dir(sub.x / length, sub.y / length, sub.z / length);
						Vector3 velocity(
						    dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed);

						velocity = TransformNormal(velocity, worldTransform_.matWorld_);

						EnemyBullet* newBullet = new EnemyBullet();

						newBullet->Initialize(model_, worldTransform_.translation_, velocity);

						gamescene_->AddEnemyBullet(newBullet);

						timer_ = 0;
					}
				}
			}
			if (gamescene_->phaseNumber_ >= 15 && gamescene_->phaseNumber_ < 25) {
				attackMode_ = 1;
				if (attackMode_ == 1) {
					if (timer_ == 30) {
						const float kBulletSpeed = 2.0f;
						end = player_->GetWorldPosition();
						start = GetWorldPosition();
						sub.x = start.x - end.x;
						sub.y = start.y - end.y;
						sub.z = start.z - end.z;

						float length = sqrt(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z);
						Vector3 dir(sub.x / length, sub.y / length, sub.z / length);
						Vector3 velocity(
						    dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed);

						velocity = TransformNormal(velocity, worldTransform_.matWorld_);

						EnemyBullet* newBullet = new EnemyBullet();

						newBullet->Initialize(model_, worldTransform_.translation_, velocity);

						gamescene_->AddEnemyBullet(newBullet);
						timer_ = 0;
						attackMode_ = 0;
					}
				}
			}
			if (gamescene_->phaseNumber_ >= 5 && gamescene_->phaseNumber_ < 15) {
				attackMode_ = 2;
				if (attackMode_ == 2) {
					if (timer_ == 60) {
						const float kBulletSpeed = 0.50f;
						end = player_->GetWorldPosition();
						start = GetWorldPosition();
						sub.x = (start.x + rand() % 20 - 10) - end.x;
						sub.y = (start.y + rand() % 20 - 10) - end.y;
						sub.z = start.z - end.z;

						float length = sqrt(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z);
						Vector3 dir(sub.x / length, sub.y / length, sub.z / length);
						Vector3 velocity(
						    dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed);

						velocity = TransformNormal(velocity, worldTransform_.matWorld_);

						EnemyBullet* newBullet = new EnemyBullet();

						newBullet->Initialize(model_, worldTransform_.translation_, velocity);

						gamescene_->AddEnemyBullet(newBullet);
						timer_ = 0;
						// attackMode_ = 0;
					}
				}
			}
			
		}
	}
}

float Enemy::easeInQuad(float t, float b, float c, float d) {
	{
		t /= d;
		if (t < 0.3f) {
			return c / 2.0f * t * t + b;
		}
		t = t - 1.0f;
		return c / 2.0f * (t * (t - 2.0f) - 1.0f) + b;
	}
}

void Enemy::AddBullet(Vector3 pos,Vector3 velo) {

	EnemyBullet* obj = new EnemyBullet();
	obj->Initialize(model_, pos ,velo);

	bullets_.push_back(obj);
}
