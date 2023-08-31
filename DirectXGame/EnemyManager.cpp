#include "EnemyManager.h"

/// <summary>
/// シングルトンインスタンスの取得
/// </summary>
/// <returns></returns>
EnemyManager* EnemyManager::GetInstance() {
	static EnemyManager instance;
	return &instance;
}

/// <summary>
/// 初期化
/// </summary>
void EnemyManager::Initialize(const std::vector<Model*>& models) {

	models_ = models;
	enemyCount_ = enemyMax;

	for (size_t i = 0; i < enemyMax; i++) {
		AddEnemy(i);
	}
}

/// <summary>
/// 更新
/// </summary>
void EnemyManager::Update() {

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	for (EnemyEffect* enemyEffect : enemyEffects_) {
		enemyEffect->Update();
	}

}

/// <summary>
/// 描画
/// </summary>
void EnemyManager::Draw(const ViewProjection& viewProjection) {

	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection);
	}
	for (EnemyEffect* enemyEffect : enemyEffects_) {
		enemyEffect->Draw(viewProjection);
	}

}

/// <summary>
/// エネミー追加
/// </summary>
void EnemyManager::AddEnemy(size_t positionNum) {

	Enemy* enemy = new Enemy();

	enemy->Initialize(
	    models_, initPositionData[positionNum], enemySpeed[positionNum], enemyRotateSpeed[positionNum]);
	enemies_.push_back(enemy);

}

/// <summary>
/// エネミー削除
/// </summary>
void EnemyManager::DeleteEnemy() {

	for (Enemy* enemy : enemies_) {
		if (enemy->IsDead()) {
			enemyCount_--;

			EnemyEffect* enemyEffect = new EnemyEffect();
			enemyEffect->Initialize(models_, enemy->GetWorldTransform().translation_);
			enemyEffects_.push_back(enemyEffect);

		}
	}

	// デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	enemyEffects_.remove_if([](EnemyEffect* enemyEffect) {
		if (enemyEffect->IsDead()) {
			delete enemyEffect;
			return true;
		}
		return false;
	});

}

/// <summary>
///	削除
/// </summary>
void EnemyManager::Delete() {
	enemies_.remove_if([](Enemy* enemy) {
		delete enemy;
		return true;
	});
	enemyEffects_.remove_if([](EnemyEffect* enemyEffect) {
		delete enemyEffect;
		return true;
	});

}

/// <summary>
/// リセット
/// </summary>
void EnemyManager::Reset() {

	if (enemyCount_ == 0) {
		Delete();
		enemyCount_ = enemyMax;

		for (size_t i = 0; i < enemyMax; i++) {
			AddEnemy(i);
		}
	}

}
