#include "Skydome.h"
#include <cassert>

// 初期化
void Skydome::Initialize(Model* model) {

	// NULLポインタチェック
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

// 更新
void Skydome::Update() {

	// 行列を更新
	worldTransform_.UpdateMatrix();
}

// 描画
void Skydome::Draw(ViewProjection viewProjection) {

	// 3Dモデル描画
	model_->Draw(worldTransform_, viewProjection);
}
