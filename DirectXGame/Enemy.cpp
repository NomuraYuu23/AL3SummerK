#include "Enemy.h"
#include "Player.h"

// ImGui
#include "ImGuiManager.h"

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

	// ホーミング
	//  自キャラのワールド座標を取得する
	Vector3 playerPos = {
	    player_->GetWorldTransform().matWorld_.m[3][0],
	    player_->GetWorldTransform().matWorld_.m[3][1],
	    player_->GetWorldTransform().matWorld_.m[3][2]};
	// 敵のワールド座標を取得する
	Vector3 enemyPos = {
	    worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
	    worldTransform_.matWorld_.m[3][2]};
	// 敵弾->自キャラの差分ベクトルを求める
	Vector3 toPlayer = vecClac_->Subtract(playerPos, enemyPos);

	if (vecClac_->Length(toPlayer) >= distanceToMove) {
		// ベクトルの正規化
		toPlayer = vecClac_->Normalize(toPlayer);
		velocity_ = vecClac_->Normalize(velocity_);

		// 球面線形補間により、今の速度と自キャラのベクトルを内挿し、新たな速度とする
		velocity_ = vecClac_->Multiply(speed, vecClac_->Slerp(velocity_, toPlayer, moveT));
		// 進行方向に見た目の回転を合わせる
		//  Y軸周りの角度(Θy)
		worldTransform_.rotation_.y = std::atan2f(velocity_.x, velocity_.z);
		// 横軸方向の長さを求める
		float length = vecClac_->Length(Vector3{velocity_.x, 0.0f, velocity_.z});
		// X軸周りの角度(Θx)
		worldTransform_.rotation_.x = std::atan2f(-velocity_.y, length);

		// 座標を移動させる(1フレーム分の移動量を足しこむ)
		worldTransform_.translation_ = vecClac_->Add(worldTransform_.translation_, velocity_);
	}
}

// 衝突時に呼ばれる関数
void Enemy::OnCollision() {

	// 死亡フラグを立てる
	isDead_ = true;

}
