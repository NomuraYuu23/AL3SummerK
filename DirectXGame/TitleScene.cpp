#include "TitleScene.h"

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

	uint32_t titleTextureHandle = TextureManager::Load("./Resources/sprite/title.png");
	spriteTitle_.reset(Sprite::Create(
	    titleTextureHandle, Vector2(WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f),
	    Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
	spriteTitle_->SetSize(Vector2(WinApp::kWindowWidth, WinApp::kWindowHeight));

}

/// <summary>
/// 毎フレーム処理
/// </summary>
void TitleScene::Update() {



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

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
