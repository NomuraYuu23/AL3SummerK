#include "Player.h"
#include "Enemy.h"
#include "GlobalVariables.h"
#include <sstream>
#include <cassert>
#include "WinApp.h"

// ImGui
#include "ImGuiManager.h"
#include <algorithm>

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Player::Initialize(
    const std::vector<Model*>& models, const std::vector<Model*>& modelsBullet, float lockonRange) {

	// nullポインタチェック
	assert(models.front());

	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	// インプット取得
	input_ = Input::GetInstance();
	matCalc_ = Matrix4x4Calc::GetInstance();
	vecClac_ = Vector3Calc::GetInstance();

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffe);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000001);

	// 弾モデル
	modelsBullet_ = modelsBullet;

	// レティクル
	reticle3DWorldTransform_.Initialize();

	// レティクル範囲
	lockonRange_ = lockonRange;
	// レティクル距離
	lockonLength_ = 1000.0f;

	// 外部での値設定
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	// ワールドトランスフォーム
	globalVariables->AddItem(groupName, "WorldTransform Translate", worldTransform_.translation_);
	globalVariables->AddItem(groupName, "WorldTransform Rotate", worldTransform_.rotation_);

}

/// <summary>
/// 更新
/// </summary>
void Player::Update(Sprite* spriteLockon) {

// 調整項目の適用
#ifdef _DEBUG
	// ApplyGlobalVariables();
#endif // DEBUG

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Move();

	ReticleUpdate(spriteLockon);

	Attack();

	// ワールド行列更新
	worldTransform_.UpdateMatrix();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Player::Draw(const ViewProjection& viewProjection) {

	uint8_t i = 0;

	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
		i++;
	}

	// 弾
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

/// <summary>
/// 調整項目の適用
/// </summary>
void Player::ApplyGlobalVariables() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	// ワールドトランスフォーム
	worldTransform_.translation_ =
	    globalVariables->GetVector3Value(groupName, "WorldTransform Translate");
	worldTransform_.rotation_ =
	    globalVariables->GetVector3Value(groupName, "WorldTransform Rotate");
}

/// <summary>
/// 移動
/// </summary>
void Player::Move() {

	// 速さ
	const float speed = 0.3f;

	// 移動量
	Vector3 move = {};

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	if (input_->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed;
		move.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed;
	}

	// カメラの角度から回転行列を計算する
	Matrix4x4 rotateMatrixX = matCalc_->MakeRotateXMatrix(0.0f);
	Matrix4x4 rotateMatrixY = matCalc_->MakeRotateYMatrix(viewProjection_->rotation_.y);
	Matrix4x4 rotateMatrixZ = matCalc_->MakeRotateZMatrix(viewProjection_->rotation_.z);

	Matrix4x4 rotateMatrix =
	    matCalc_->Multiply(rotateMatrixX, matCalc_->Multiply(rotateMatrixY, rotateMatrixZ));

	// 移動ベクトルをカメラの角度だけ回転する
	move = matCalc_->TransformNormal(move, rotateMatrix);

	// 移動
	worldTransform_.translation_ = vecClac_->Add(worldTransform_.translation_, move);
	worldTransform_.translation_.x =
	    std::clamp(worldTransform_.translation_.x, -moveLimit_, moveLimit_);
	worldTransform_.translation_.z =
	    std::clamp(worldTransform_.translation_.z, -moveLimit_, moveLimit_);

	// 移動方向に見た目を合わせる(Y軸)
	worldTransform_.rotation_.y = viewProjection_->rotation_.y;

}

/// <summary>
/// 攻撃
/// </summary>
void Player::Attack() {

	if (firingIntervalCount > 0.0f) {
		firingIntervalCount -= 1.0f / 60.0f;
		if (firingIntervalCount <= 0.0f) {
			firingIntervalCount = 0.0f;
		}
		return;
	}

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// を押していたら
	if (!input_->GetJoystickState(0, joyState)) {
		return;
	}

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		// 弾の位置
		Vector3 position = GetWorldPosition();
		position.z += std::cosf(viewProjection_->rotation_.y) *
		              std::cosf(viewProjection_->rotation_.x) * 5.0f; // コサイン
		position.x += std::sinf(viewProjection_->rotation_.y) *
		              std::cosf(viewProjection_->rotation_.x) * 5.0f;
		position.y += -std::sinf(viewProjection_->rotation_.x) *
		              std::cosf(viewProjection_->rotation_.z) * 5.0f;

		Vector3 velocity(0, 0, 0);
		Vector3 reticle3DWorldPosition{
		    reticle3DWorldTransform_.matWorld_.m[3][0],
			reticle3DWorldTransform_.matWorld_.m[3][1],
		    reticle3DWorldTransform_.matWorld_.m[3][2]};
		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = vecClac_->Subtract(reticle3DWorldPosition, position);
		velocity = vecClac_->Multiply(kBulletSpeed, vecClac_->Normalize(velocity));

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(modelsBullet_, position, viewProjection_->rotation_, velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);

		//クールタイム
		firingIntervalCount = kFiringInterval;

	}
}

void Player::ReticleUpdate(Sprite* spriteLockon) {

	// ビュー行列、射影行列、ビューポート行列の合成行列を計算する
	// ビューポート行列
	Matrix4x4 matViewport =
	    matCalc_->MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列,ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = matCalc_->Multiply(
	    viewProjection_->matView, matCalc_->Multiply(viewProjection_->matProjection, matViewport));

	// 合成行列の逆行列を計算
	Matrix4x4 matInverseVPV = matCalc_->Inverse(matViewProjectionViewport);

	// ニア―、ファー
	Vector3 posNear =
	    Vector3(float(WinApp::kWindowWidth) / 2.0f, float(WinApp::kWindowHeight) / 2.0f, 0);
	Vector3 posFar =
	    Vector3(float(WinApp::kWindowWidth) / 2.0f, float(WinApp::kWindowHeight) / 2.0f, 1);
	// スクリーン座標系からワールド座標系へ
	posNear = matCalc_->Transform(posNear, matInverseVPV);
	posFar = matCalc_->Transform(posFar, matInverseVPV);

	// 中心距離
	Vector3 direction = vecClac_->Subtract(posFar, posNear);
	direction = vecClac_->Normalize(direction);
	lockonPosition = vecClac_->Add(posNear, vecClac_->Multiply(kDistanceObject, direction));

	// 今の位置
	Vector3 reticle3DWorldPosition = {
	    reticle3DWorldTransform_.matWorld_.m[3][0], 
		reticle3DWorldTransform_.matWorld_.m[3][1],
	    reticle3DWorldTransform_.matWorld_.m[3][2]};

	bool lockonChange = true;

	Vector3 position = GetWorldPosition();
	position = matCalc_->Transform(position, matViewProjectionViewport);

	// 現在ロックオンしているエネミーをロックオンし続けられるか
	if (lockonEnemy_) {
		if (!lockonEnemy_->IsDead()) {
			Vector3 positionEnemy = lockonEnemy_->GetWorldPosition();
			positionEnemy = matCalc_->Transform(positionEnemy, matViewProjectionViewport);

			float distance = std::sqrtf(
			    std::powf(positionEnemy.x - position.x, 2.0f) +
			    std::powf(positionEnemy.y - position.y, 2.0f));

			// プレイヤーからエネミーまでの距離
			float length = vecClac_->Length(vecClac_->Subtract(position, positionEnemy));

			if (distance < lockonRange_ + lockonEnemy_->GetRadius() && length < lockonLength_) {
				lockonChange = false;
			} else {
				lockonEnemy_ = nullptr;
			}
		}
	}

	// ロックオン変更
	if (lockonChange) {
		Enemy* lockonEnemy = nullptr;

		for (Enemy* enemy : enemies_) {
			if (!enemy->IsDead()) {
				// エネミーワールドポジション
				Vector3 positionEnemy = enemy->GetWorldPosition();
				positionEnemy = matCalc_->Transform(positionEnemy, matViewProjectionViewport);

				// xy上での距離
				float distance = std::sqrtf(
				    std::powf(positionEnemy.x - position.x, 2.0f) +
				    std::powf(positionEnemy.y - position.y, 2.0f));

				// プレイヤーからエネミーまでの距離
				float length = vecClac_->Length(vecClac_->Subtract(position, positionEnemy));

				// ロックオン範囲、ロックオン距離にいる
				if (distance < lockonRange_ + enemy->GetRadius() && length < lockonLength_) {
					// 敵をロックオン
					if (!lockonEnemy) {
						lockonEnemy = enemy;
					} else {
						float preLength = vecClac_->Length(vecClac_->Subtract(
						    GetWorldPosition(), lockonEnemy->GetWorldPosition()));
						if (length < preLength) {
							lockonEnemy = enemy;
						}
					}
				}
			}

			// 敵をロックオン
			if (lockonEnemy) {
				lockonT = 0;
				lockonEnemy_ = lockonEnemy;
			}
		}
	}

	// 線形補間で移動
	if (lockonEnemy_) {
		if (!lockonEnemy_->IsDead()) {
			lockonPosition = lockonEnemy_->GetWorldPosition();
		}
	}

	reticle3DWorldPosition = vecClac_->Lerp(reticle3DWorldPosition, lockonPosition, lockonT);
	if (lockonT >= 1.0f) {
		lockonT = 1.0f;
	} else {
		lockonT += 0.025f;
	}

	// レティクル行列更新
	reticle3DWorldTransform_.translation_ = reticle3DWorldPosition;
	reticle3DWorldTransform_.UpdateMatrix();

	
	Vector3 spriteLockonPosition =
	    matCalc_->Transform(reticle3DWorldPosition, matViewProjectionViewport);

	spriteLockon->SetPosition(Vector2(spriteLockonPosition.x, spriteLockonPosition.y)); 

}

/// <summary>
/// リセット
/// </summary>
void Player::Reset() {

	// レティクル
	reticle3DWorldTransform_.scale_ = {1, 1, 1};
	reticle3DWorldTransform_.rotation_ = {0, 0, 0};
	reticle3DWorldTransform_.translation_ = {0, 0, 0};
	reticle3DWorldTransform_.UpdateMatrix();
	// ワールド変換データの初期化
	worldTransform_.scale_ = {1,1,1};
	worldTransform_.rotation_ = {0,0,0};
	worldTransform_.translation_ = {0, 0, 0};
	worldTransform_.UpdateMatrix();

	//エネミークリア
	enemies_.clear();
	//弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		delete bullet;
		return true;
	});

}
