#include "RailCamera.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(Vector3 trans,Vector3 rot) { 
	worldTranceform_.Initialize();
	worldTranceform_.translation_ = trans;
	worldTranceform_.rotation_ = rot;

	viewProjection_.Initialize();
}

void RailCamera::Update() { 
	
	worldTranceform_.translation_.x += move.x;
	worldTranceform_.translation_.y += move.y;
	worldTranceform_.translation_.z += move.z;

	worldTranceform_.rotation_.x += Rot.x;
	worldTranceform_.rotation_.y += Rot.y;
	worldTranceform_.rotation_.z += Rot.z;

	worldTranceform_.UpdateMatrix();
	viewProjection_.matView = Inverse(worldTranceform_.matWorld_);
	viewProjection_.TransferMatrix();

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("trans", &worldTranceform_.translation_.x, -2.0f, 2.0f);
	ImGui::SliderFloat3("rot", &worldTranceform_.rotation_.x, -2.0, 2.0f);
	ImGui::End();

}
