#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("cube.jpg");
	worldTranceform_.Initialize();
	
	worldTranceform_.translation_ = position;
	
}

void PlayerBullet::Update() 
{ 
	worldTranceform_.UpdateMatrix(); 
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) 
{	
	model_->Draw(worldTranceform_, viewProjection, textureHandle_);
}
