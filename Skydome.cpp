#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) 
{ 
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	
	worldTransform_.translation_ = {0, 0, 0};
}

void Skydome::Update() 
{ 
	worldTransform_.UpdateMatrix();
	//
	//if (worldTransform_.translation_.z <= 20.0f) {
	//	//worldTransform_.translation_.z = 0.0f;
	//	worldTransform_.translation_.z += move.z;
	//}
}

void Skydome::Draw(const ViewProjection& viewProjection) 
{ 
	model_->Draw(worldTransform_, viewProjection);
}
