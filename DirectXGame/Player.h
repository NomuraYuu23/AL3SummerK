#pragma once
#pragma once
#include "BaseCharacter.h"
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include <memory>
#include <optional>
#include <vector>

// Input
#include "Input.h"

#include "Matrix4x4.h"
#include "Vector3.h"

// 弾
#include "PlayerBullet.h"

class Enemy;

class Player : public BaseCharacter {

public:
	// モデル
	enum class playerModelIndex {
		kModelIndexBody = 0,   // 体
		kModelIndexHead = 1,   // 頭
		kModelIndexUL_arm = 2, // 左上腕
		kModelIndexLL_arm = 3, // 左下腕
		kModelIndexUR_arm = 4, // 右上腕
		kModelIndexLR_arm = 5, // 右下腕
		kModelIndexUL_leg = 6, // 左上足
		kModelIndexLL_leg = 7, // 左下足
		kModelIndexUR_leg = 8, // 右上足
		kModelIndexLR_leg = 9, // 右下足
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(
	    const std::vector<Model*>& models, const std::vector<Model*>& modelsBullet,
	    float lockonRange);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(Sprite* spriteLockon);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

	/// <summary>
	/// カメラ追従のためのワールドトランスフォーム取得
	/// </summary>
	WorldTransform* GetWorldTransformAddress() { return &worldTransform_; }

	/// <summary>
	/// ビュープロジェクション設定
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// レティクル更新
	/// </summary>
	void ReticleUpdate(Sprite* spriteLockon);

	/// <summary>
	/// エネミー登録
	/// </summary>
	/// <param name="enemies"></param>
	void SetEnemies(std::list<Enemy*> enemies) { enemies_ = enemies; }

	/// <summary>
	/// 弾取得
	/// </summary>
	std::list<PlayerBullet*> GetBullets() { return bullets_; }

	Enemy* GetLockonEnemy() { return lockonEnemy_; }

	// 衝突時に呼ばれる関数
	void OnCollision() override{};

private:
	// インプット
	Input* input_;
	Matrix4x4Calc* matCalc_;
	Vector3Calc* vecClac_;

	// モデル用ワールドトランスフォーム
	WorldTransform modelWorldTransforms_[static_cast<int>(playerModelIndex::kModelIndexLR_leg) + 1];

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;
	// モデル
	std::vector<Model*> modelsBullet_;
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	// 発射間隔(秒)
	const float kFiringInterval = 1.0f;
	// 発射クールタイム
	float firingIntervalCount = 0.0f;

	// カメラから標準オブジェクトの距離
	const float kDistanceObject = 80.0f;

	// 敵キャラ
	std::list<Enemy*> enemies_;
	//ロックオンしているエネミー
	Enemy* lockonEnemy_ = nullptr;
	// ロックオンポジション
	Vector3 lockonPosition = {0.0f, 0.0f, 0.0f};
	// ロックオン移動開始地点
	Vector3 lockonPositionStart = {0.0f, 0.0f, 0.0f};
	// ロックオン線形補間t
	float lockonT = 1.0f;

	// レティクル3D
	WorldTransform reticle3DWorldTransform_;
	// レティクル範囲
	float lockonRange_ = 0.0f;
	// レティクル距離
	float lockonLength_ = 0.0f;
};
