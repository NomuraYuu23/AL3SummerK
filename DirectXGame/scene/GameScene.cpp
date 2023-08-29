#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	
	uint32_t sightTextureHandle = TextureManager::Load("./Resources/sprite/sight.png");
	spriteSight_.reset(Sprite::Create(
	    sightTextureHandle, Vector2(WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f),
	    Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));

	uint32_t lockonTextureHandle = TextureManager::Load("./Resources/sprite/lockon.png");
	spriteLockon_.reset(Sprite::Create(
	    lockonTextureHandle, Vector2(WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f),
	    Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));

	//プレイヤー
	player_ = std::make_unique<Player>();
	//プレイヤー3Dモデル
	modelPlayerBody_.reset(Model::CreateFromOBJ("testBoneLowerJoint",true));
	modelPlayerHead_.reset(Model::CreateFromOBJ("testBoneLowerJoint", true));
	modelPlayerUL_arm_.reset(Model::CreateFromOBJ("testBoneUpperJoint", true));
	modelPlayerLL_arm_.reset(Model::CreateFromOBJ("testBoneUpperJoint", true));
	modelPlayerUR_arm_.reset(Model::CreateFromOBJ("testBoneUpperJoint", true));
	modelPlayerLR_arm_.reset(Model::CreateFromOBJ("testBoneUpperJoint", true));
	modelPlayerUL_leg_.reset(Model::CreateFromOBJ("testBoneUpperJoint", true));
	modelPlayerLL_leg_.reset(Model::CreateFromOBJ("testBoneUpperJoint", true));
	modelPlayerUR_leg_.reset(Model::CreateFromOBJ("testBoneUpperJoint", true));
	modelPlayerLR_leg_.reset(Model::CreateFromOBJ("testBoneUpperJoint", true));

	std::vector<Model*> playerModels = {
		modelPlayerBody_.get(),
		modelPlayerHead_.get(),

		modelPlayerUL_arm_.get(),
		modelPlayerLL_arm_.get(),
		modelPlayerUR_arm_.get(),
		modelPlayerLR_arm_.get(),

		modelPlayerUL_leg_.get(),
		modelPlayerLL_leg_.get(),
		modelPlayerUR_leg_.get(),
		modelPlayerLR_leg_.get(),

	};

	//プレイヤー弾モデル
	modelPlayerBullet_.reset(Model::CreateFromOBJ("bullet",true));

	std::vector<Model*> playerBulletModels = {
		modelPlayerBullet_.get()
	};

	//自キャラの初期化
	player_->Initialize(playerModels, playerBulletModels, spriteSight_->GetSize().x / 2.0f);

	//追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget((player_.get()->GetWorldTransformAddress()));
	player_->SetViewProjection(followCamera_->GetViewProjectionAddress());

	//エネミー
	enemyManager_ = EnemyManager::GetInstance();
	//エネミーモデル
	modelEnemy_.reset(Model::CreateFromOBJ("enemy", true));
	std::vector<Model*> enemyModels = {
		modelEnemy_.get()
	};
	//エネミー
	enemyManager_->Initialize(enemyModels);
	
	player_->SetEnemies(enemyManager_->GetEnemies());

	//グラウンド
	ground_ = std::make_unique<Ground>();
	//グラウンドモデル
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	//グラウンドの初期化
	ground_->Initialize(modelGround_.get());

	//スカイドーム
	skydome_ = std::make_unique<Skydome>();
	//スカイドームモデル
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	//スカイドームの初期化
	skydome_->Initialize(modelSkydome_.get());

	// 衝突マネージャー
	collisionManager.reset(new CollisionManager);

	// クリアフラグ
	clearFlg = false;
	// 暗転
	uint32_t blackoutTextureHandle = TextureManager::Load("./Resources/white1x1.png");

	// 色
	blackoutColor_ = {0.0f, 0.0f, 0.0f, 1.0f};

	spriteBlackout_.reset(Sprite::Create(
	    blackoutTextureHandle, Vector2(WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f),
	    blackoutColor_, Vector2(0.5f, 0.5f)));
	spriteBlackout_->SetSize(Vector2(WinApp::kWindowWidth, WinApp::kWindowHeight));
	
	// スタートフラグ
	startFlg_ = false;
	// スピード
	blackoutSpeed_ = 0.01f;

	// シーン終了フラグ
	endOfScene_ = false;

}

void GameScene::Update() {

	//ゲームスタート前
	if (!startFlg_) {
		blackoutColor_.w -= blackoutSpeed_;
		spriteBlackout_->SetColor(blackoutColor_);
		if (blackoutColor_.w <= 0.0f) {
			blackoutColor_.w = 0.0f;
			startFlg_ = true;
		}
	}

	// オブジェクト更新

	enemyManager_->DeleteEnemy();

	// プレイヤー
	player_->Update(spriteLockon_.get());

	// カメラ設定
	CameraSetting();

	// エネミー
	enemyManager_->Update();

	// リストをクリア
	collisionManager->ListClear();
	// コライダーをリストに登録
	// 自機について
	collisionManager->ListRegister(player_.get());
	// 敵全てについて
	for (Enemy* enemy : enemyManager_->GetEnemies()) {
		collisionManager->ListRegister(enemy);
	}
	// 自弾全てについて
	for (PlayerBullet* playerBullet : player_->GetBullets()) {
		collisionManager->ListRegister(playerBullet);
	}
	// 当たり判定
	collisionManager->CheakAllCollision();

	//タイトルへ
	if (clearFlg) {
		blackoutColor_.w += blackoutSpeed_;
		spriteBlackout_->SetColor(blackoutColor_);
		if (blackoutColor_.w > 1.0f) {
			blackoutColor_.w = 1.0f;
			endOfScene_ = true;
		}
	}

	//クリア
	if (enemyManager_->GetEnemyCount() <= 0) {
		clearFlg = true;
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// グラウンド
	//ground_->Draw(*viewProjection_);
	// スカイドーム
	skydome_->Draw(*viewProjection_);
	// モデル
	player_->Draw(*viewProjection_);

	enemyManager_->Draw(*viewProjection_);


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	spriteSight_->Draw();
	spriteLockon_->Draw();
	spriteBlackout_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

/// <summary>
/// カメラ処理
/// </summary>
void GameScene::CameraSetting() {

	// 追従カメラの更新
	followCamera_->Update();

	viewProjection_ = followCamera_.get()->GetViewProjectionAddress();

}

/// <summary>
/// リセット
/// </summary>
void GameScene::Reset() {

	//プレイヤー
	player_->Reset();
	//追従カメラ
	followCamera_->Reset();
	//エネミー
	enemyManager_->Reset();
	player_->SetEnemies(enemyManager_->GetEnemies());
	// クリアフラグ
	clearFlg = false;
	// 色
	blackoutColor_ = {0.0f, 0.0f, 0.0f, 1.0f};
	spriteBlackout_->SetColor(blackoutColor_);
	// スタートフラグ
	startFlg_ = false;

	// シーン終了フラグ
	endOfScene_ = false;

}
