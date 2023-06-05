#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) 
{ 
	assert(model);
	model_ = model;

	worldTranceform_.Initialize();
	worldTranceform_.translation_ = {0, 0, 0};
}

void Skydome::Update() 
{ 
	worldTranceform_.UpdateMatrix();
}

void Skydome::Draw(const ViewProjection& viewProjection) 
{ 
	model_->Draw(worldTranceform_, viewProjection);
}
