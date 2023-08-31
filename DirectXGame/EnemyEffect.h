#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

#include "Matrix4x4.h"
#include "Vector3.h"

class EnemyEffect {

public:

	void Initialize(
	    const std::vector<Model*>& models, Vector3 translation,
	    Vector3 acceleration = {0.0f, -9.8f / 60.0f, 0.0f});

	void Update();

	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 死んでいるか
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

private:
	
	WorldTransform worldTransform_;

	std::vector<Model*> models_;

	Vector3 velocity_;
	
	Vector3 acceleration_;

	bool isDead_;

};
