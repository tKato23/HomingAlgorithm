
//多重インクルードの防止
#pragma once

//ヘッダファイルのインクルード
#include <d3d11.h>
#include <Keyboard.h>
#include "CollisionNode.h"

class Player
{
public:
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

	//プレイヤーの角度を取得する
	const DirectX::SimpleMath::Vector3& GetRot() { return m_ObjPlayer[BODY].GetRotation(); }

	//プレイヤーのクオータニオンを取得する
	const DirectX::SimpleMath::Quaternion& GetRotQ() { return m_ObjPlayer[BODY].GetRotationQ(); }

	//プレイヤーの位置を取得する
	const DirectX::SimpleMath::Vector3& GetTrans() { return m_ObjPlayer[BODY].GetTranslation(); }

	//プレイヤーのワールド行列を取得する
	const DirectX::SimpleMath::Matrix& GetWor() { return m_ObjPlayer[BODY].GetWorld(); }

	//プレイヤーの移動ベクトルを取得する
	const DirectX::SimpleMath::Vector3& GetMoveV() { return m_moveV; }
	
	//プレイヤーの当たり判定(排斥処理用)を取得する
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

	//プレイヤーの角度をセットする
	void SetRot(const DirectX::SimpleMath::Vector3& rotation) { m_ObjPlayer[BODY].SetRotation(rotation); }

	//プレイヤーのクオータニオンをセットする
	void SetRotQ(const DirectX::SimpleMath::Quaternion& quaternion) { m_ObjPlayer[BODY].SetRotationQ(quaternion); }

	//プレイヤーの位置をセットする
	void SetTrans(const DirectX::SimpleMath::Vector3& translation){ m_ObjPlayer[BODY].SetTranslation(translation); }

private:
	//定数宣言
	const float ROT_SPEED = 0.05f;	//機体の回転速度

	//自機の3Dオブジェクト
	std::vector<Obj3d> m_ObjPlayer;

	// キーボード
	DirectX::Keyboard* m_pKeyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	//プレイヤーの移動ベクトル
	DirectX::SimpleMath::Vector3 m_moveV;

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

	//全身の当たり判定(排斥処理用)
	SphereNode m_CollisionNodeBody;
};