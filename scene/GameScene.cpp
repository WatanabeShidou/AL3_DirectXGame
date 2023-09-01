#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <iostream>
#include <fstream>
#include "ImGuiManager.h"
#include <stdlib.h>

GameScene::GameScene() { }

void GameScene::CheckAllCollisions() 
{ 
	//Vector3 posA, posB;
	//
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();
	const std::list<EnemyBullet*>& enemyBullets = bullets_;
	const std::list<Enemy*>& Enemys_ = Enemys;

	////自キャラと敵弾の当たり判定
	//posA = player_->GetWorldPosition();
	//for (EnemyBullet* bullets: enemyBullets) {
	//	posB = bullets->GetWorldPosition();

	//	float dis = (posB.x - posA.x) * (posB.x - posA.x) + 
	//				(posB.y - posA.y) * (posB.y - posA.y) +
	//	            (posB.z - posA.z) * (posB.z - posA.z);
	//	if (dis <= (player_->GetRadius() + bullets->GetRadius())*2) {
	//		player_->OnCollision();
	//		bullets->OnCollision();
	//	}
	//}
	////敵キャラと自弾の判定
	//for (Enemy* Enemy_ : Enemys_) {
	//	posA = Enemy_->GetWorldPosition();
	//	for (PlayerBullet* bullets : playerBullets) {
	//		posB = bullets->GetWorldPosition();
	//		float dis = (posB.x - posA.x) * (posB.x - posA.x) +
	//		            (posB.y - posA.y) * (posB.y - posA.y) +
	//		            (posB.z - posA.z) * (posB.z - posA.z);
	//		if (dis <= (Enemy_->GetRadius() + bullets->GetRadius()) * 2) {
	//			Enemy_->OnCollision();
	//			bullets->OnCollision();
	//		}
	//	}
	//}
	//
	////敵弾と自弾の判定
	//for (PlayerBullet* playerbullets : playerBullets) {
	//	posA = playerbullets->GetWorldPosition();
	//	for (EnemyBullet* enemybullets : enemyBullets) {
	//		posB = enemybullets->GetWorldPosition();
	//		float dis = (posB.x - posA.x) * (posB.x - posA.x) +
	//		            (posB.y - posA.y) * (posB.y - posA.y) +
	//		            (posB.z - posA.z) * (posB.z - posA.z);
	//		if (dis <= (playerbullets->GetRadius() + enemybullets->GetRadius()) * 2) {
	//			enemybullets->OnCollision();
	//			playerbullets->OnCollision();
	//		}
	//	}
	//}
	// 敵弾と自弾の判定
	for (PlayerBullet* playerbullets : playerBullets) {
		for (EnemyBullet* enemybullets : enemyBullets) {
			ChackCollisionPair(playerbullets, enemybullets);
		}
	}
	// 敵キャラと自弾の判定
	for (Enemy* Enemy_ : Enemys_) {
		for (PlayerBullet* bullets : playerBullets) {
			ChackCollisionPair(Enemy_, bullets);
		}
	}
	for (Enemy* Enemy_ : Enemys_) {
		ChackCollisionPair(Enemy_, player_);
	}
	// 自キャラと敵弾の当たり判定
	for (EnemyBullet* bullets : enemyBullets) {
		ChackCollisionPair(player_,bullets);
	}
}

void GameScene::LoadEnemyPopData() {

	std::ifstream file;

	file.open("./Resources/EnemyPop.csv");

	assert(file.is_open());

	enemyPopCommands[0] << file.rdbuf();

	file.close();
	std::ifstream file1;

	file1.open("./Resources/EnemyPop2.csv");

	assert(file1.is_open());

	enemyPopCommands[1] << file1.rdbuf();

	file1.close();
	std::ifstream file2;

	file2.open("./Resources/EnemyPop3.csv");

	assert(file2.is_open());

	enemyPopCommands[2] << file2.rdbuf();

	file2.close();
}

void GameScene::UpdateEnemyPopCommands() {
	
	if (waitFlag) {
		waitTimer--;
		if (waitTimer <= 0) {
			waitFlag = false;
		}
		return;
	}
	
	std::string line;
	if (phaseNumber_ >= 0 && phaseNumber_ <= 5) {
		while (getline(enemyPopCommands[0], line)) {
			std::istringstream line_stream(line);

			std::string word;

			getline(line_stream, word, ',');

			if (word.find("//") == 0) {
				continue;
			}
			
			if (word.find("POP") == 0) {
				getline(line_stream, word, ',');
				float x = (float)std::atof(word.c_str());

				getline(line_stream, word, ',');
				float y = (float)std::atof(word.c_str());

				getline(line_stream, word, ',');
				float z = (float)std::atof(word.c_str());

				AddEnemy(Vector3(x, y, z));
			} else if (word.find("WAIT") == 0) {
				getline(line_stream, word, ',');

				int32_t waitTime = atoi(word.c_str());

				waitFlag = true;
				waitTimer = waitTime;

				break;
			} else if (word.find("HP") == 0) {
				getline(line_stream, word, ',');
				enemyHP_ = (int)std::atof(word.c_str());
			}
		}
	}
	if (phaseNumber_ >= 5 && phaseNumber_ <= 15) {
		while (getline(enemyPopCommands[1], line)) {
			std::istringstream line_stream(line);

			std::string word;

			getline(line_stream, word, ',');

			if (word.find("//") == 0) {
				continue;
			}
			
			if (word.find("POP") == 0) {
				getline(line_stream, word, ',');
				float x = (float)std::atof(word.c_str());
				
				getline(line_stream, word, ',');
				float y = (float)std::atof(word.c_str());
				
				getline(line_stream, word, ',');
				float z = (float)std::atof(word.c_str());
				
				AddEnemy(Vector3(x, y, z));
			} else if (word.find("WAIT") == 0) {
				getline(line_stream, word, ',');

				int32_t waitTime = atoi(word.c_str());

				waitFlag = true;
				waitTimer = waitTime;

				break;
			} else if (word.find("HP") == 0) {
				getline(line_stream, word, ',');
				enemyHP_ = (int)std::atof(word.c_str());
			}
			
		}
	}
	if (phaseNumber_ >= 15 && phaseNumber_ < 25) {
		while (getline(enemyPopCommands[2], line)) {
			std::istringstream line_stream(line);

			std::string word;

			getline(line_stream, word, ',');

			if (word.find("//") == 0) {
				continue;
			}

			if (word.find("POP") == 0) {
				getline(line_stream, word, ',');
				float x = (float)std::atof(word.c_str());

				getline(line_stream, word, ',');
				float y = (float)std::atof(word.c_str());

				getline(line_stream, word, ',');
				float z = (float)std::atof(word.c_str());

				AddEnemy(Vector3(x, y, z));
			} else if (word.find("WAIT") == 0) {
				getline(line_stream, word, ',');

				int32_t waitTime = atoi(word.c_str());

				waitFlag = true;
				waitTimer = waitTime;

				break;
			} else if (word.find("HP") == 0) {
				getline(line_stream, word, ',');
				enemyHP_ = (int)std::atof(word.c_str());
			}
		}
	}
	if (phaseNumber_ >= 25 && phaseNumber_ < 65) {
		
		while (f < 40) {
			float x = 0;
			float y = 0;
			enemyHP_ = 50;
			AddEnemy(Vector3((x + rand() % 40 - 20), (y + rand() % 20 - 10), 70));
			f += 1;
		}
	}
	if (phaseNumber_ >= 65 && phaseNumber_ < 105) {

		while (f < 80) {
			float x = 0;
			float y = 0;
			enemyHP_ = 50;
			plusScore_ = 50;
			AddEnemy(Vector3((x + rand() % 40 - 20), (y + rand() % 20 - 10), 70));
			f += 1;
		}
	}
	if (phaseNumber_ == 105) {
		while (f < 81) {
			enemyHP_ = 150;
			AddEnemy(Vector3(-30, 0, 70));
			f += 1;
		}
	}
}

GameScene::~GameScene() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
	delete model_;
	delete player_;
	delete debugCamera_;
	for (Enemy* Enemy : Enemys) {
		delete Enemy;
	}
	
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
}

void GameScene::Initialize() {
		scene_ = 0;
		LoadEnemyPopData();
		dxCommon_ = DirectXCommon::GetInstance();
		input_ = Input::GetInstance();
		audio_ = Audio::GetInstance();
		TextureManager::Load("target.png");
		textureHandle_ = TextureManager::Load("black.jpg");
	    textureHandle1_ = TextureManager::Load("phonto.png");
		model_ = Model::Create();
		worldTransform_.Initialize();
		viewProjection_.Initialize();

		phaseNumber_ = 0;
		score_ = 0;
		plusScore_ = 10;
		f = 0;
		Vector3 enemy{
		    30,
		    2,
		    100,
		};

		Vector3 playerPosition{0, 0, 20};

		enemy_ = new Enemy();
		enemy_->Initialize(model_, enemy);

		player_ = new Player();
		player_->Initialize(model_, textureHandle_, playerPosition);

		modelSkydome_ = Model::CreateFromOBJ("skydome", true);
		skydome_ = new Skydome();
		skydome_->Initialize(modelSkydome_);

		railCamera_ = new RailCamera();

		railCamera_->Initialize(worldTransform_.translation_, worldTransform_.rotation_);

		debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
		AxisIndicator::GetInstance()->SetVisible(true);
		AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

		enemy_->SetPlayer(player_);
		enemy_->SetGameScene(this);
		// AddEnemy({0.f, 5.f, 30.f});
		// AddEnemy({0.f, 8.f, 30.f});
	
	
}

void GameScene::AddEnemyBullet(EnemyBullet* enemybullet) { bullets_.push_back(enemybullet); }

void GameScene::ChackCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 posA, posB;

	//colliderA->SetRadius(player_->radius_);
	
	posA = colliderA->GetWorldPosition();
	posB = colliderB->GetWorldPosition();
	float dis = (posB.x - posA.x) * (posB.x - posA.x) +
			    (posB.y - posA.y) * (posB.y - posA.y) +
			    (posB.z - posA.z) * (posB.z - posA.z);
	if (dis <= (colliderA->GetRadius() + colliderB->GetRadius() * 2)) {
		colliderA->OnCollision();
		colliderB->OnCollision();
		
	}
}

void GameScene::AddEnemy(Vector3 pos) {
	Enemy* obj = new Enemy();
	obj->Initialize(model_, pos);
	obj->SetPlayer(player_);
	obj->SetGameScene(this);
	
	obj->SetEnemyHP(enemyHP_);
	
	Enemys.push_back(obj);
}

void GameScene::Update() {
	
	if (scene_ == 0) {
		if (GetAsyncKeyState('X') & 0x8000) {
			scene_ = 1;
		}
	}
	if (scene_ == 1) {
		/*if (GetAsyncKeyState('R') & 0x8000) {
	
			Enemys.remove_if([this](Enemy* enemy) {
				delete enemy;

				return true;
			});
			bullets_.remove_if([](EnemyBullet* bullet) {
				delete bullet;
				return true;
			});
			LoadEnemyPopData();	
			UpdateEnemyPopCommands();
			scene_ = 0;
			return GameScene::Initialize();
			
		}*/
		
		UpdateEnemyPopCommands();
	/*	ImGui::Begin("enemy");
		ImGui::Text("attack:%d", enemy_->attackMode_);
		ImGui::Text("number:%d", phaseNumber_);
		ImGui::Text("Score:%d", score_);
		ImGui::Text("timer:%d", spawnTimer_);
		ImGui::End();*/
		bullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});

		Enemys.remove_if([this](Enemy* enemy) {
			if (enemy->enemyHP_ == 0) {
				score_ += plusScore_;
			}
			if (enemy->GetisDead()) {
				phaseNumber_ += 1;

				delete enemy;

				return true;
			}
			return false;
		});

		// phaseNumber_ = 25;
		// if (railCamera_->worldTransform_.rotation_.y <= 2.0f) {
		//	if (phaseNumber_ == 25) {
		//		player_->SetParent(&railCamera_->GetWorldTransform());
		//		railCamera_->worldTransform_.rotation_.y += 0.005f;
		//	}
		// }

		for (EnemyBullet* bullet : bullets_) {
			bullet->Update();
		}
		for (Enemy* Enemy : Enemys) {
			Enemy->Update();
		}
		railCamera_->Rotate();
		player_->Rotate();
		skydome_->Update();
		player_->Update(&viewProjection_);
#ifdef _DEBUG
		if (input_->PushKey(DIK_Z)) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
#endif

		if (isDebugCameraActive_) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		} else if (!isDebugCameraActive_) {
			railCamera_->Update();
			viewProjection_.matView = railCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
			// worldTransform_.rotation_.y = railCamera_->GetWorldTransform().rotation_.y;
		}

		CheckAllCollisions();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	
	
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	player_->Draw(viewProjection_);
	for (Enemy* Enemy : Enemys) {
		Enemy->Draw(viewProjection_);
	}
	skydome_->Draw(viewProjection_);
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->DrawUI();
	if (scene_ == 0) {
		player_->Draw2D();
	}
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
