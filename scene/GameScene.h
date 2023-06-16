#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Model* model_ = nullptr;
	Skydome* skydome_ = nullptr;
	RailCamera* railCamera_ = nullptr;
	Model* modelSkydome_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	void CheckAllCollisions();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	uint32_t textureHandle_ = 0;
	//テクスチャハンドル
	
	/// </summary>
};
