#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <memory>
#include <optional>
#include <vector>

// Input
#include "Input.h"

#include "BaseCharacter.h"

class Enemy : public BaseCharacter {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(
	    const std::vector<Model*>& models, Vector3 position = {0.0f, 0.0f, 0.0f}, float speed = 0.1f, float rotateSpeed = 0.1f);
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 死んでいるか
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	// 衝突時に呼ばれる関数
	void OnCollision() override;

private:
	// インプット
	Input* input_;
	Matrix4x4Calc* matCalc_;
	Vector3Calc* vecClac_;

	// 速度
	Vector3 velocity_;
	// 1フレームでの補間割合t
	float moveT = 0.025f;
	// 速さ
	float speed_ = 0.1f;
	// 回転速さ
	float rotateSpeed_ = 0.1f;

	// デスフラグ
	bool isDead_ = false;
};
