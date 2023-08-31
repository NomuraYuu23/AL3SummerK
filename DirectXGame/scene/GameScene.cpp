#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	enemyManager_->Delete();

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// サイト
	uint32_t sightTextureHandle = TextureManager::Load("./Resources/sprite/sight.png");
	spriteSight_.reset(Sprite::Create(
	    sightTextureHandle, Vector2(WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f),
	    Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
	// ロックオン
	uint32_t lockonTextureHandle = TextureManager::Load("./Resources/sprite/lockon.png");
	spriteLockon_.reset(Sprite::Create(
	    lockonTextureHandle, Vector2(WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f),
	    Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
	// クリア
	uint32_t clearTextureHandle = TextureManager::Load("./Resources/sprite/clear.png");
	spriteClear_.reset(Sprite::Create(
	    clearTextureHandle, Vector2(WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f),
	    Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
	// エンド
	uint32_t endTextureHandle = TextureManager::Load("./Resources/sprite/end.png");
	spriteEnd_.reset(Sprite::Create(
	    endTextureHandle, Vector2(WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight * 2.0f / 3.0f),
	    Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
	// ui
	uint32_t uiTextureHandle = TextureManager::Load("./Resources/sprite/ui.png");
	spriteUI_.reset(Sprite::Create(
	    uiTextureHandle, Vector2(WinApp::kWindowWidth * 7.0f / 8.0f, WinApp::kWindowHeight / 2.0f),
	    Vector4(1.0f, 1.0f, 1.0f, 0.8f), Vector2(0.5f, 0.5f)));
	// 数字
	uint32_t numTextureHandle = TextureManager::Load("./Resources/sprite/num.png");
	spriteNum_[0].reset(Sprite::Create(
	    numTextureHandle, Vector2(0, 0),
	    Vector4(1.0f, 1.0f, 1.0f, 0.8f), Vector2(0.5f, 0.5f)));
	spriteNum_[1].reset(Sprite::Create(
	    numTextureHandle, Vector2(0, 0), Vector4(1.0f, 1.0f, 1.0f, 0.8f), Vector2(0.5f, 0.5f)));

	//プレイヤー
	player_ = std::make_unique<Player>();
	//プレイヤー3Dモデル
	modelPlayer_.reset(Model::CreateFromOBJ("player",true));

	std::vector<Model*> playerModels = {
		modelPlayer_.get(),
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
	// エンドフラグ
	endFlg_ = false;
	// スピード
	blackoutSpeed_ = 0.03f;

	// シーン終了フラグ
	endOfScene_ = false;

}

void GameScene::Update() {

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// を押していたら
	if (!input_->GetJoystickState(0, joyState)) {
		return;
	}

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
	player_->SetEnemies(enemyManager_->GetEnemies());

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
		if (!endFlg_ && 
			joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			endFlg_ = true;
		}
		if (endFlg_) {
			blackoutColor_.w += blackoutSpeed_;
			spriteBlackout_->SetColor(blackoutColor_);
			if (blackoutColor_.w > 1.0f) {
				blackoutColor_.w = 1.0f;
				endOfScene_ = true;
			}
		}
	}

	//クリア
	size_t enemyCount = enemyManager_->GetEnemyCount();
	Vector2 numSize = {80.0f, 120.0f};

	spriteNum_[0]->SetPosition(Vector2(WinApp::kWindowWidth * 16.5f / 20.0f, WinApp::kWindowHeight / 4.0f));
	spriteNum_[0]->SetTextureRect(Vector2(float(enemyCount / 10) * numSize.x, 0.0f), numSize);
	spriteNum_[0]->SetSize(numSize);
	
	spriteNum_[1]->SetPosition(
	    Vector2(WinApp::kWindowWidth * 16.5f / 20.0f + numSize.x, WinApp::kWindowHeight / 4.0f));
	spriteNum_[1]->SetTextureRect(Vector2(float(enemyCount % 10) * numSize.x, 0.0f), numSize);
	spriteNum_[1]->SetSize(numSize);

	if (enemyCount <= 0) {
		clearFlg = true;
	}

	preJoyState = joyState;

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
	if (clearFlg) {
		spriteClear_->Draw();
		spriteEnd_->Draw();
	} else {
		spriteSight_->Draw();
		spriteLockon_->Draw();
	}
	spriteUI_->Draw();
	spriteNum_[0]->Draw();
	spriteNum_[1]->Draw();
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
	// エンドフラグ
	endFlg_ = false;

	// シーン終了フラグ
	endOfScene_ = false;

}
