#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <iostream>
#include <fstream>

GameScene::GameScene() {}

void GameScene::CheckAllCollisions() 
{ 
	Vector3 posA, posB;
	
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();
	const std::list<EnemyBullet*>& enemyBullets = bullets_;
	const std::list<Enemy*>& Enemys_ = Enemys;

	//自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullets: enemyBullets) {
		posB = bullets->GetWorldPosition();

		float dis = (posB.x - posA.x) * (posB.x - posA.x) + 
					(posB.y - posA.y) * (posB.y - posA.y) +
		            (posB.z - posA.z) * (posB.z - posA.z);
		if (dis <= (player_->GetRadius() + bullets->GetRadius())*2) {
			player_->OnCollision();
			bullets->OnCollision();
		}
	}
	//敵キャラと自弾の判定
	for (Enemy* Enemy_ : Enemys_) {
		posA = Enemy_->GetWorldPosition();
		for (PlayerBullet* bullets : playerBullets) {
			posB = bullets->GetWorldPosition();
			float dis = (posB.x - posA.x) * (posB.x - posA.x) +
			            (posB.y - posA.y) * (posB.y - posA.y) +
			            (posB.z - posA.z) * (posB.z - posA.z);
			if (dis <= (Enemy_->GetRadius() + bullets->GetRadius()) * 2) {
				Enemy_->OnCollision();
				bullets->OnCollision();
			}
		}
	}
	
	//敵弾と自弾の判定
	for (PlayerBullet* playerbullets : playerBullets) {
		posA = playerbullets->GetWorldPosition();
		for (EnemyBullet* enemybullets : enemyBullets) {
			posB = enemybullets->GetWorldPosition();
			float dis = (posB.x - posA.x) * (posB.x - posA.x) +
			            (posB.y - posA.y) * (posB.y - posA.y) +
			            (posB.z - posA.z) * (posB.z - posA.z);
			if (dis <= (playerbullets->GetRadius() + enemybullets->GetRadius()) * 2) {
				enemybullets->OnCollision();
				playerbullets->OnCollision();
			}
		}
	}
}

void GameScene::LoadEnemyPopData() {
	
	std::ifstream file;
	file.open("./Resources/EnemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
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
	while (getline(enemyPopCommands, line)) {
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
		} 
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			waitFlag = true;
			waitTimer = waitTime;

			break;
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
	LoadEnemyPopData();
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	TextureManager::Load("target.png");
	textureHandle_ = TextureManager::Load("sumire2.png");
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	Vector3 enemy{
		30,
		2,
		50,
	};

	Vector3 playerPosition{
		0,0,20
	};

	enemy_ = new Enemy();
	enemy_->Initialize(model_,enemy);
	

	player_ = new Player();
	player_->Initialize(model_,textureHandle_,playerPosition);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	railCamera_ = new RailCamera();
	railCamera_->Initialize(worldTransform_.translation_,worldTransform_.rotation_);
	player_->SetParent(&railCamera_->GetWorldTransform());
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	enemy_->SetPlayer(player_);
	enemy_->SetGameScene(this);
	//AddEnemy({0.f, 5.f, 30.f});
	//AddEnemy({0.f, 8.f, 30.f});
	
	
}

void GameScene::AddEnemyBullet(EnemyBullet* enemybullet) {
	bullets_.push_back(enemybullet); }



void GameScene::AddEnemy(Vector3 pos) {
	Enemy* obj = new Enemy();
	obj->Initialize(model_, pos);
	obj->SetPlayer(player_);
	obj->SetGameScene(this);
	Enemys.push_back(obj);
}

void GameScene::Update() {
	
	UpdateEnemyPopCommands();
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	
	Enemys.remove_if([](Enemy* enemy) {
		if (enemy->GetisDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
	for (Enemy* Enemy : Enemys) {
		Enemy->Update();
	}
	
	player_->Rotate();
	skydome_->Update();
	player_->Update(&viewProjection_);
	#ifdef _DEBUG
	if (input_->PushKey(DIK_Z)) {
		isDebugCameraActive_ = true;
	} 
	else {
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
	}
	
	CheckAllCollisions();
	
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

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
