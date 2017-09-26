#pragma once

#include <d3d11.h>
#include <Keyboard.h>
#include "CollisionNode.h"

class Player
{
public:
	//定数宣言
	const float ROT_SPEED = 0.05f;				//機体の回転速度

	//自機パーツ
	enum PLAYERPARTS
	{
		BODY,
		L_WING,
		R_WING,
		L_ENGINE,
		R_ENGINE,

		PLAYER_PARTS_NUM
	};

	//コンストラクタ
	Player(DirectX::Keyboard* keyboard);

	//デストラクタ
	~Player();

	//初期化処理
	void Initialize();

	//更新処理
	void Update();

	// 行列の計算
	void Calc();

	//描画処理
	void Draw();

	//プレイヤーの挙動
	void Action();

	//プレイヤーの移動を取得する
	const DirectX::SimpleMath::Vector3& GetRot();

	//プレイヤーの角度を取得する
	const DirectX::SimpleMath::Vector3& GetTrans();

	//プレイヤーのワールド行列を取得する
	const DirectX::SimpleMath::Matrix& GetWor();

	//プレイヤーの移動をセットする
	void SetRot(const DirectX::SimpleMath::Vector3& rotation);

	//プレイヤーの角度をセットする
	void SetTrans(const DirectX::SimpleMath::Vector3& translation);

	//プレイヤーの当たり判定(排斥処理用)を取得する
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

	//プレイヤーの当たり判定(くぐった時の判定用)を取得する
	const SphereNode& GetCollisionNodeHit() { return m_CollisionNodeHit; }

	//プレイヤーの速度を取得する
	const DirectX::SimpleMath::Vector3 GetVelocity() { return m_Velocity; }

private:
	//自機の3Dオブジェクト
	std::vector<Obj3d> m_ObjPlayer;

	// キーボード
	DirectX::Keyboard* m_pKeyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	// サイン用の引数となる角度
	float m_sinAngle;

	//全身の当たり判定(排斥処理用)
	SphereNode m_CollisionNodeBody;

	//全身の当たり判定(くぐった時の判定用)
	SphereNode m_CollisionNodeHit;

	//速度
	DirectX::SimpleMath::Vector3 m_Velocity;

	//前進速度
	float m_speed;
};