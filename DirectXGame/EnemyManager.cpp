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
}

/// <summary>
/// 描画
/// </summary>
void EnemyManager::Draw(const ViewProjection& viewProjection) {

	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection);
	}
}

/// <summary>
/// エネミー追加
/// </summary>
void EnemyManager::AddEnemy(size_t positionNum) {

	Enemy* enemy = new Enemy();

	enemy->Initialize(models_, initPositionData[positionNum]);
	enemies_.push_back(enemy);
}

/// <summary>
/// エネミー削除
/// </summary>
void EnemyManager::DeleteEnemy() {

	for (Enemy* enemy : enemies_) {
		if (enemy->IsDead()) {
			enemyCount_--;
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
}

/// <summary>
///	削除
/// </summary>
void EnemyManager::Delete() {
	enemies_.remove_if([](Enemy* enemy) {
		delete enemy;
		return true;
	});
}
