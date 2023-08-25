#include "PlayerBullet.h"

#include "Vector3.h"
#include <cassert>

/// <summary>
/// 初期化
/// </summary>
void PlayerBullet::Initialize(
    const std::vector<Model*>& models, const Vector3& position, const Vector3& velocity) {

	// nullポインタチェック
	assert(models.front());

	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffe);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000001);
}

/// <summary>
/// 更新
/// </summary>
void PlayerBullet::Update() {

	// 移動
	worldTransform_.translation_ =
	    Vector3Calc::GetInstance()->Add(worldTransform_.translation_, velocity_);

	// 行列を更新
	worldTransform_.UpdateMatrix();

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}

/// <summary>
/// 衝突を検出したら呼び出されるコールバック関数
/// </summary>
void PlayerBullet::OnCollision() {

	isDead_ = true;

}
