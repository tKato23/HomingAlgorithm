#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>
#include "Obj3d.h"
#include "CollisionNode.h"
#include "Player.h"
#include "Strategy.h"


class Enemy
{
public:
	Enemy();

	~Enemy();
	//初期化処理
	void Initialize();

	//更新処理
	void Update();

	//行列の計算
	void Calc();

	//描画処理
	void Draw();

	//エネミーの挙動
	void Action();

	//エネミーの角度を取得する
	const DirectX::SimpleMath::Vector3& GetRot() { return m_ObjEnemy[BODY].GetRotation(); };

	//エネミーのクオータニオンを取得する
	const DirectX::SimpleMath::Quaternion& GetRotQ() { return m_ObjEnemy[BODY].GetRotationQ(); };

	//エネミーの位置を取得する
	const DirectX::SimpleMath::Vector3& GetTrans() { return m_ObjEnemy[BODY].GetTranslation(); }

	//エネミーの移動ベクトルを取得する
	const DirectX::SimpleMath::Vector3& GetMoveV() { return m_moveV; }

	//エネミーの当たり判定を取得する
	const SphereNode& GetCollisionNodeEnemy() { return m_CollisionNodeEnemy; }

	//	エネミーのワールド座標
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_ObjEnemy[BODY].GetWorld(); }

	//エネミーの角度をセットする
	void SetRot(const DirectX::SimpleMath::Vector3& rotation) { m_ObjEnemy[BODY].SetRotation(rotation); }

	//エネミーの角度をセットする
	void SetRotQ(const DirectX::SimpleMath::Quaternion& quaternion) { m_ObjEnemy[BODY].SetRotationQ(quaternion); }

	//エネミーの位置をセットする
	void SetTrans(const DirectX::SimpleMath::Vector3& translation) { m_ObjEnemy[BODY].SetTranslation(translation); }

	//	エネミーのワールド行列をセットする
	void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_ObjEnemy[BODY].SetWorld(world); }

	//プレイヤーをセットする
	void SetPlayer(Player* player) { m_Player = player; }

	//追跡型の自動追尾
	void PursuitHouming();

	//先読み型の自動追尾
	void PrefetchHoming();

	//	待ち伏せ型の自動追尾
	void AmbushHoming();

	//	ホーミングのフラグをセットする
	void SetHomingFlag(bool flag);

	//	タイプの取得とセット
	Homing::Type getCurrentType() { return m_currentType; }
	void SetHomingType(Homing::Type type) { m_currentType = type; }

	Homing** getHomingDictionary() { return m_homingDictionary; }
	void addStrategy(Homing::Type type);
	void HomingExecute();

	//間合い確保型の自動追尾
	void IntervalHoming();

private:

	//自機の3Dオブジェクト
	std::vector<Obj3d> m_ObjEnemy;

	//	プレイヤー
	Player* m_Player;

	//	定数

	static const float MOVE_SPEED;
	static const int MAX_STRATEGY_NUM = 4;

	// サイン用の引数となる角度
	float m_sinAngle;

	//タイマー
	int m_Timer;

	//移動ベクトル
	DirectX::SimpleMath::Vector3 m_moveV;

	//目標角度
	float m_DistAngle;

	//自機パーツ
	enum ENEMYPARTS
	{
		BODY,
		L_WING,
		R_WING,
		L_ENGINE,
		R_ENGINE,
		L_WEAPON,
		R_WEAPON,

		ENEMY_PARTS_NUM
	};

	//エネミーの当たり判定
	SphereNode m_CollisionNodeEnemy;

	//	ホーミングを実行するフラグ
	bool m_homingFlag;

	Homing* m_homingDictionary[MAX_STRATEGY_NUM];
	Homing::Type m_currentType;
};