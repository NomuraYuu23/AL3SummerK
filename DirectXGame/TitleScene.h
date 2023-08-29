#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// タイトル
/// </summary>
class TitleScene {

public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

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
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// シーン終了フラグ取得
	/// </summary>
	bool GetEndOfScene() { return endOfScene_; }

private:

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	//タイトル画面
	std::unique_ptr<Sprite> spriteTitle_ = nullptr;
	//スタート
	std::unique_ptr<Sprite> spriteStart_ = nullptr;
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	WorldTransform modelWorldTransform_;
	float modelRotateSpeed_ = 0.02f;

	//画面暗転
	std::unique_ptr<Sprite> spriteBlackout_ = nullptr;
	// スタートフラグ
	bool IsWhiteout_;
	// 色
	Vector4 blackoutColor_;
	//スタートフラグ 
	bool IsBlackout_;
	//スピード
	float blackoutSpeed_;

	//シーン終了フラグ
	bool endOfScene_;

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE preJoyState;

};
