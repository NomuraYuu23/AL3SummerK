#include "Ground.h"
#include <cassert>

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
void Ground::Initialize(Model* model) {

	// nullポインタチェック
	assert(model);

	model_ = model;

	// ワールド変換データの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.y -= 15.0f;
	worldTransform_.UpdateMatrix();
}

/// <summary>
/// 更新
/// </summary>
void Ground::Update() {}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Ground::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
