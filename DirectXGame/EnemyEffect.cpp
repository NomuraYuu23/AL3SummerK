#include "EnemyEffect.h"

void EnemyEffect::Initialize(const std::vector<Model*>& models, Vector3 translation, Vector3 acceleration) {

	worldTransform_.Initialize();
	worldTransform_.translation_ = translation;
	worldTransform_.UpdateMatrix();

	models_ = models;

	velocity_ = {0.0f,2.0f,0.0f};

	acceleration_ = acceleration;

	isDead_ = false;

}

void EnemyEffect::Update() {

	Vector3Calc* calc = Vector3Calc::GetInstance();

	velocity_ = calc->Add(velocity_, acceleration_);

	worldTransform_.translation_ = calc->Add(worldTransform_.translation_, velocity_);

	worldTransform_.UpdateMatrix();

	if (worldTransform_.matWorld_.m[3][1] <= -200.0f) {
		isDead_ = true;
	}

}

void EnemyEffect::Draw(const ViewProjection& viewProjection) {

	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}

}
