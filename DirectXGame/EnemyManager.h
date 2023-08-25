#pragma once
#include "Enemy.h"
#include "Model.h"

class Player;

class EnemyManager {

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static EnemyManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models, Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// エネミー追加
	/// </summary>
	void AddEnemy(size_t positionNum);

	/// <summary>
	/// エネミー削除
	/// </summary>
	void DeleteEnemy();

	/// <summary>
	///	削除
	/// </summary>
	void Delete();

	/// <summary>
	///
	/// </summary>
	std::list<Enemy*> GetEnemies() { return enemies_; }

private:
	EnemyManager() = default;
	~EnemyManager() = default;
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;

	// エネミー
	std::list<Enemy*> enemies_;
	// エネミーモデル
	std::vector<Model*> models_;
	// プレイヤー
	Player* player_;

	// エネミー数
	size_t enemyCount;
	// エネミー最大数
	const size_t enemyMax = 20;

	// エネミーポジションデータ
	Vector3 initPositionData[4] = {
	    {-10.0f, 0.0f, 30.0f},
	    {-10.0f, 5.0f, 30.0f},
	    {10.0f,  0.0f, 30.0f},
	    {10.0f,  5.0f, 30.0f},
	};
};
