#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>
#include "Obj3d.h"
#include "CollisionNode.h"
#include "Player.h"

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
	const DirectX::SimpleMath::Vector3& GetRot();

	//エネミーの位置を取得する
	const DirectX::SimpleMath::Vector3& GetTrans();

	//エネミーの移動ベクトルを取得する
	const DirectX::SimpleMath::Vector3& GetMoveV();

	//エネミーの角度をセットする
	void SetRot(const DirectX::SimpleMath::Vector3& rotation);

	//エネミーの角度をセットする(クォータニオン)
	void SetRotQ(const DirectX::SimpleMath::Quaternion& rotationQ);

	//エネミーの位置をセットする
	void SetTrans(const DirectX::SimpleMath::Vector3& translation);

	void SetPlayer(Player* player) { m_Player = player; }

	//エネミーの当たり判定を取得する
	const SphereNode& GetCollisionNodeEnemy() { return m_CollisionNodeEnemy; }

	//先読み型の自動追尾
	void PrefetchHoming();

	//	待ち伏せ型の自動追尾
	void AmbushHoming();

	//旋回型の自動追尾
	void TurnHoming();

private:
	//自機の3Dオブジェクト
	std::vector<Obj3d> m_ObjEnemy;


	//	プレイヤー
	Player* m_Player;

	//	定数
	const float MOVE_SPEED = -0.01f;

	// サイン用の引数となる角度
	float m_sinAngle;

	//タイマー
	int m_Timer;

	//移動ベクトル
	DirectX::SimpleMath::Vector3 m_moveV;

	//目標角度
	float m_DistAngle;

	//ミサイル攻撃を管理するフラグ
	bool m_weapon_flag;
	bool m_a_flag;

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
};