#include "Enemy.h"
#include "Player.h"

// ImGui
#include "ImGuiManager.h"
#include <numbers>

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Enemy::Initialize(const std::vector<Model*>& models, Player* player,
    Vector3 position) {

	// nullポインタチェック
	assert(models.front());
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	worldTransform_.translation_ = position;

	// インプット取得
	input_ = Input::GetInstance();
	matCalc_ = Matrix4x4Calc::GetInstance();
	vecClac_ = Vector3Calc::GetInstance();

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffd);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000002);

	// プレイヤーのトランスフォーム
	player_ = player;

	velocity_ = {0.1f, 0.1f, 0.1f};
}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update() {

	Move();

	// ワールド行列更新
	worldTransform_.UpdateMatrix();
}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Enemy::Draw(const ViewProjection& viewProjection) {

	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}

/// <summary>
/// 移動
/// </summary>
void Enemy::Move() {

	worldTransform_.rotation_.y += rotateSpeed;
	if (worldTransform_.rotation_.y >= 2.0f * float(std::numbers::pi)) {
		worldTransform_.rotation_.y -= 2.0f * float(std::numbers::pi);
	}

	// 移動速度
	Vector3 velocity(0.0f, 0.0f, speed);

	// 速度ベクトルを向きに合わせて回転させる
	velocity_ = matCalc_->TransformNormal(velocity, worldTransform_.matWorld_);
	worldTransform_.translation_ = vecClac_->Add(worldTransform_.translation_, velocity_);

}

// 衝突時に呼ばれる関数
void Enemy::OnCollision() {

	// 死亡フラグを立てる
	isDead_ = true;

}
