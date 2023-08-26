#pragma once
#include "BaseCharacter.h"

class PlayerBullet : public BaseCharacter {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(
	    const std::vector<Model*>& models, const Vector3& position,
	    const Vector3& rotation, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision() override;

	bool IsDead() const { return isDead_; }

private:
	// 速度
	Vector3 velocity_;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};
