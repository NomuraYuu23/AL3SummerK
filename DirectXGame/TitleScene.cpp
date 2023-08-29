#include "TitleScene.h"
#include <numbers>

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

/// <summary>
/// 初期化
/// </summary>
void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	
	viewProjection_.Initialize();

	//タイトル
	uint32_t titleTextureHandle = TextureManager::Load("./Resources/sprite/title.png");
	spriteTitle_.reset(Sprite::Create(
	    titleTextureHandle, Vector2(WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f),
	    Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
	spriteTitle_->SetSize(Vector2(WinApp::kWindowWidth, WinApp::kWindowHeight));

	//スタート
	uint32_t startTextureHandle = TextureManager::Load("./Resources/sprite/start.png");
	spriteStart_.reset(Sprite::Create(
	    startTextureHandle, Vector2(WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight * 4.0f / 5.0f),
	    Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));

	//モデル
	model_.reset(Model::CreateFromOBJ("enemy", true));
	modelWorldTransform_.Initialize();
	modelWorldTransform_.translation_.x = 7.0f;
	modelWorldTransform_.translation_.y = 4.0f;
	modelWorldTransform_.translation_.z = -30.0f;
	modelWorldTransform_.UpdateMatrix();

	// 画面暗転
	// 色
	blackoutColor_ = {0.0f, 0.0f, 0.0f, 1.0f};

	uint32_t blackoutTextureHandle = TextureManager::Load("./Resources/white1x1.png");
	spriteBlackout_.reset(Sprite::Create(
	    blackoutTextureHandle, Vector2(WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f),
	    blackoutColor_, Vector2(0.5f, 0.5f)));
	spriteBlackout_->SetSize(Vector2(WinApp::kWindowWidth, WinApp::kWindowHeight));

	// スタートフラグ
	IsWhiteout_ = true;
	// スタートフラグ
	IsBlackout_ = false;

	// スピード
	blackoutSpeed_ = 0.01f;

	endOfScene_ = false;

}

/// <summary>
/// 毎フレーム処理
/// </summary>
void TitleScene::Update() {

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// を押していたら
	if (!input_->GetJoystickState(0, joyState)) {
		return;
	}

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && !IsBlackout_ && !IsWhiteout_) {
		IsBlackout_ = true;
	} else if (IsBlackout_) {
		blackoutColor_.w += blackoutSpeed_;
		if (blackoutColor_.w >= 1.0f) {
			blackoutColor_.w = 1.0f;
			endOfScene_ = true;
		}
	} else if (IsWhiteout_) {
		blackoutColor_.w -= blackoutSpeed_;
		if (blackoutColor_.w <= 0.0f) {
			blackoutColor_.w = 0.0f;
			IsWhiteout_ = false;
		}
	}

	spriteBlackout_->SetColor(blackoutColor_);

	//モデル
	modelWorldTransform_.rotation_.y += modelRotateSpeed_;
	if (modelWorldTransform_.rotation_.y >= 2.0f * float(std::numbers::pi)) {
		modelWorldTransform_.rotation_.y =
		    modelWorldTransform_.rotation_.y - 2.0f * float(std::numbers::pi); 
	}
	modelWorldTransform_.UpdateMatrix();

}

/// <summary>
/// 描画
/// </summary>
void TitleScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	spriteTitle_->Draw();
	spriteStart_->Draw();

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
	model_->Draw(modelWorldTransform_,viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	spriteBlackout_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

/// <summary>
/// リセット
/// </summary>
void TitleScene::Reset() {

	// 色
	blackoutColor_ = {0.0f, 0.0f, 0.0f, 1.0f};
	// スタートフラグ
	IsWhiteout_ = true;
	IsBlackout_ = false;

	endOfScene_ = false;

}
