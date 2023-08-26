#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

//プレイヤー
#include "Player.h"
//追従カメラ
#include "FollowCamera.h"
//エネミー
#include "EnemyManager.h"
//グラウンド
#include "Ground.h"
//スカイドーム
#include "Skydome.h"
//衝突マネージャー
#include "CollisionManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// カメラ処理
	/// </summary>
	void CameraSetting();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	ViewProjection* viewProjection_;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// プレイヤー3Dモデル
	std::unique_ptr<Model> modelPlayerBody_ = nullptr;
	std::unique_ptr<Model> modelPlayerHead_ = nullptr;
	std::unique_ptr<Model> modelPlayerUL_arm_ = nullptr;
	std::unique_ptr<Model> modelPlayerLL_arm_ = nullptr;
	std::unique_ptr<Model> modelPlayerUR_arm_ = nullptr;
	std::unique_ptr<Model> modelPlayerLR_arm_ = nullptr;
	std::unique_ptr<Model> modelPlayerUL_leg_ = nullptr;
	std::unique_ptr<Model> modelPlayerLL_leg_ = nullptr;
	std::unique_ptr<Model> modelPlayerUR_leg_ = nullptr;
	std::unique_ptr<Model> modelPlayerLR_leg_ = nullptr;

	// プレイヤー弾モデル
	std::unique_ptr<Model> modelPlayerBullet_ = nullptr;

	std::unique_ptr<Sprite> spriteSight_ = nullptr;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	// エネミー
	EnemyManager* enemyManager_ = nullptr;
	// エネミーモデル
	std::unique_ptr<Model> modelEnemy_ = nullptr;

	// グラウンド
	std::unique_ptr<Ground> ground_ = nullptr;
	// グラウンド3Dモデル
	std::unique_ptr<Model> modelGround_ = nullptr;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_ = nullptr;
	// スカイドーム3Dモデル
	std::unique_ptr<Model> modelSkydome_ = nullptr;

	// 衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager;

};
