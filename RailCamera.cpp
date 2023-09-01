#include "RailCamera.h"
#include "ImGuiManager.h"


void RailCamera::Initialize(Vector3 trans,Vector3 rot) { 
	worldTransform_.Initialize();
	for (int a = 0; a < 20; a++) {
		worldTransform_.translation_ = trans;
	}
	worldTransform_.rotation_ = rot;

	viewProjection_.Initialize();
}

void RailCamera::Update() { 
	
	//worldTransform_.translation_.x += move.x;
	//worldTransform_.translation_.y += move.y;
	

	if (worldTransform_.translation_.z <= 10.0f) {
		// worldTransform_.translation_.z = 0.0f;
		worldTransform_.translation_.z += move.z;
	}
	
	//SHORT GetAsyncKeyState(int vKey);
	
	if (GetAsyncKeyState('D') & 0x8000) {
		if (worldTransform_.rotation_.y < 0.05f) {
			worldTransform_.rotation_.y += 0.001f;
		}
		if (worldTransform_.rotation_.z > -0.05f) {
			worldTransform_.rotation_.z -= 0.002f;
		}
	}

	if (GetAsyncKeyState('A') & 0x8000) {
		if (worldTransform_.rotation_.y > -0.05f) {
			worldTransform_.rotation_.y -= 0.001f;
		}
		if (worldTransform_.rotation_.z < 0.05f) {
			worldTransform_.rotation_.z += 0.002f;
		}
	} 

	if (GetAsyncKeyState('S') & 0x8000) {
		if (worldTransform_.rotation_.x < 0.05f) {
			worldTransform_.rotation_.x += 0.001f;
		}
	}

	if (GetAsyncKeyState('W') & 0x8000) {
		if (worldTransform_.rotation_.x > -0.05f) {
			worldTransform_.rotation_.x -= 0.001f;
		}
	} 
	//worldTransform_.rotation_.x += Rot.x;
	//worldTransform_.rotation_.y += Rot.y;
	//worldTransform_.rotation_.z += Rot.z;

	worldTransform_.UpdateMatrix();
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.TransferMatrix();
	
	
	//ImGui::Begin("Camera");
	//ImGui::SliderFloat3("trans", &worldTransform_.translation_.x, -2.0f, 2.0f);
	//ImGui::SliderFloat3("rot", &worldTransform_.rotation_.x, -2.0, 2.0f);
	//ImGui::End();

}

void RailCamera::Rotate() {
	

}
