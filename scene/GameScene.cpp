#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

void GameScene::CheckAllCollisions() 
{ 
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullet();

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
	posA = enemy_->GetWorldPosition();
	for (PlayerBullet* bullets : playerBullets) {
		posB = bullets->GetWorldPosition();
		float dis = (posB.x - posA.x) * (posB.x - posA.x) + 
					(posB.y - posA.y) * (posB.y - posA.y) +
		            (posB.z - posA.z) * (posB.z - posA.z);
		if (dis <= (player_->GetRadius() + bullets->GetRadius()) * 2) {
			enemy_->OnCollision();
			bullets->OnCollision();
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

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	
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
}

void GameScene::Update() 
{
	player_->Update();
	player_->Rotate();
	
	enemy_->Update();
	skydome_->Update();
	
	
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
	enemy_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
