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
#include <sstream>
#include "Collider.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public Collider {

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
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	void SetPhase(int phase) { phase = phaseNumber_; }

	bool waitFlag;
	int waitTimer;
	int phaseNumber_;
	int f;
	int spawnTimer_;
	int score_;
	int plusScore_;
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
	std::list<EnemyBullet*> bullets_;
	
	std::list<Enemy*> Enemys;
	void AddEnemy(Vector3 pos);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵弾を追加
	/// </summary>
	/// <param name="enemybullet"></param>
	void AddEnemyBullet(EnemyBullet* enemybullet);
	std::stringstream enemyPopCommands[10];
	//std::stringstream enemyPopCommands1;
	int enemyHP_;
	int enemyAlive_[20];
	int scene_;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	uint32_t textureHandle_ = 0;
	uint32_t textureHandle1_ = 0;
	//テクスチャハンドル
	void ChackCollisionPair(Collider* colliderA, Collider* colliderB);
};
