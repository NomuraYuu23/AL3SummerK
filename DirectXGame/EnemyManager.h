#pragma once
#include "Enemy.h"
#include "EnemyEffect.h"
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
	void Initialize(const std::vector<Model*>& models);

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
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	///
	/// </summary>
	std::list<Enemy*> GetEnemies() { return enemies_; }

	size_t GetEnemyCount() { return enemyCount_; }

private:
	EnemyManager() = default;
	~EnemyManager() = default;
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;

	// エネミー
	std::list<Enemy*> enemies_;
	// エネミーモデル
	std::vector<Model*> models_;

	// エネミー数
	size_t enemyCount_;
	// エネミー最大数
	const size_t enemyMax = 20;

	// エネミーポジションデータ
	Vector3 initPositionData[20] = {
	    {-80.0f, -20.0f,  50.0f},
	    {-80.0f,  10.0f,  50.0f},
	    { 80.0f, -10.0f,  50.0f},
	    { 80.0f,  20.0f,  50.0f},

        {-80.0f, -20.0f, -50.0f },
        {-80.0f,  10.0f, -50.0f },
	    { 80.0f, -10.0f, -50.0f },
        { 80.0f,  20.0f, -50.0f },

	    {-10.0f, -20.0f, -80.0f},
        {-20.0f, -20.0f, -80.0f},
        { 10.0f, -20.0f, -80.0f},
        { 20.0f, -20.0f, -80.0f},
		
	    {-10.0f,  20.0f,  80.0f},
        {-20.0f,  20.0f,  80.0f},
        { 10.0f,  20.0f,  80.0f},
        { 20.0f,  20.0f,  80.0f},

		{ 10.0f,  30.0f,  0.0f},
        {-10.0f,  30.0f,  0.0f},
        { 0.0f,   10.0f,  30.0f},
        { 0.0f,  -10.0f, -30.0f},
	};

	float enemySpeed[20]{
		0.1f, 0.1f, 0.1f, 0.1f,	
		0.1f, 0.1f, 0.1f, 0.1f,	
		0.1f, 0.1f, 0.1f, 0.1f,
		0.1f, 0.15f, 0.1f, 0.15f,
		0.1f, 0.1f, 0.15f, 0.15f
	};

	float enemyRotateSpeed[20]{
		 0.01f,  -0.01f,  0.01f,  0.01f,	
		-0.005f, 0.005f, -0.005f, -0.005f,	
		 -0.02f,  -0.02f,  -0.03f,  -0.03f,
		0.007f, -0.007f, -0.006f, 0.006f,
		-0.005f, 0.005f, -0.005f, 0.005f
	};

	//エネミーエフェクト
	std::list<EnemyEffect*> enemyEffects_;

};
