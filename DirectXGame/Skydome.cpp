#include "Skydome.h"
#include <cassert>

// 初期化
void Skydome::Initialize(Model* model) {

	// NULLポインタチェック
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_.y -= 15.0f;
	worldTransform_.UpdateMatrix();
}

// 更新
void Skydome::Update() {

	// 行列を更新
	worldTransform_.UpdateMatrix();
}

// 描画
void Skydome::Draw(const ViewProjection& viewProjection) {

	// 3Dモデル描画
	model_->Draw(worldTransform_, viewProjection);
}
