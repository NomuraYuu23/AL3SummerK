#pragma once
#include "input.h"

#include "ViewProjection.h"
#include "WorldTransform.h"

class FollowCamera {

public:
	void Initialize();

	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	ViewProjection* GetViewProjectionAddress() { return &viewProjection_; }

private:
	Input* input_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 追従対象
	const WorldTransform* target_ = nullptr;
};
