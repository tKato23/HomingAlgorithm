#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//コンストラクタ
Enemy::Enemy()
{
	m_sinAngle = 0.0f;
	m_Player = nullptr;

	Initialize();
}

//デストラクタ
Enemy::~Enemy()
{

}

//初期化処理
void Enemy::Initialize()
{
	//自機パーツの読み込み
	m_ObjEnemy.resize(ENEMY_PARTS_NUM);
	m_ObjEnemy[BODY].LoadModel(L"Resources/body.cmo");
	m_ObjEnemy[L_WING].LoadModel(L"Resources/LeftWing.cmo");
	m_ObjEnemy[R_WING].LoadModel(L"Resources/RightWing.cmo");
	m_ObjEnemy[L_ENGINE].LoadModel(L"Resources/Engine.cmo");
	m_ObjEnemy[R_ENGINE].LoadModel(L"Resources/Engine.cmo");
	m_ObjEnemy[L_WEAPON].LoadModel(L"Resources/weapon.cmo");
	m_ObjEnemy[R_WEAPON].LoadModel(L"Resources/weapon.cmo");

	//パーツを親子関係をセット
	m_ObjEnemy[L_WING].SetParent(&m_ObjEnemy[BODY]);
	m_ObjEnemy[R_WING].SetParent(&m_ObjEnemy[BODY]);
	m_ObjEnemy[L_ENGINE].SetParent(&m_ObjEnemy[BODY]);
	m_ObjEnemy[R_ENGINE].SetParent(&m_ObjEnemy[BODY]);
	m_ObjEnemy[L_WEAPON].SetParent(&m_ObjEnemy[L_WING]);
	m_ObjEnemy[R_WEAPON].SetParent(&m_ObjEnemy[R_WING]);

	//親からのオフセット(座標のずらし分)をセット
	m_ObjEnemy[BODY].SetTranslation(Vector3(0.0f, 2.0f, 0.0f));
	m_ObjEnemy[L_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));
	m_ObjEnemy[R_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));
	m_ObjEnemy[L_ENGINE].SetTranslation(Vector3(-0.1f, 0.0f, 0.5f));
	m_ObjEnemy[L_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));
	m_ObjEnemy[R_ENGINE].SetTranslation(Vector3(0.1f, 0.0f, 0.5f));
	m_ObjEnemy[R_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));
	m_ObjEnemy[L_WEAPON].SetTranslation(Vector3(-0.5f, -0.05f, -0.025f));
	m_ObjEnemy[L_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));
	m_ObjEnemy[R_WEAPON].SetTranslation(Vector3(0.5f, -0.05f, -0.025f));
	m_ObjEnemy[R_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));

	//タイマーの初期化
	m_Timer = 0;

	//目標角度の初期化
	m_DistAngle = 0.0f;

	//弾丸用の当たり判定を設定する
	m_CollisionNodeEnemy.Initialize();
	m_CollisionNodeEnemy.SetParent(&m_ObjEnemy[BODY]);
	m_CollisionNodeEnemy.SetTrans(Vector3(0, 0, 0));
	m_CollisionNodeEnemy.SetLocalRadius(Vector3(0.8f, 0.8f, 0.8f));
}

//更新処理
void Enemy::Update()
{
	//ロボットの挙動の更新
	this->Action();


	//当たり判定の更新
	m_CollisionNodeEnemy.Update();

	IntervalHoming();

	//各パーツの更新
	Calc();
}

//行列更新
void Enemy::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_ObjEnemy.begin(); it != m_ObjEnemy.end(); it++)
	{
		it->Update();
	}
}

//描画
void Enemy::Draw()
{
	//各パーツの描画
	for (std::vector<Obj3d>::iterator it = m_ObjEnemy.begin(); it != m_ObjEnemy.end(); it++)
	{
		it->Draw();
	}

	//当たり判定の描画
	//m_CollisionNodeEnemy.Draw();
}

//ロボットの挙動
void Enemy::Action()
{
	//自機パーツのギミック
	{
		//左エンジンの回転
		Vector3 l_angle = m_ObjEnemy[L_ENGINE].GetRotation();
		m_ObjEnemy[L_ENGINE].SetRotation(l_angle + Vector3(0, 0, -0.05f));

		//右エンジンの回転
		Vector3 r_angle = m_ObjEnemy[R_ENGINE].GetRotation();
		m_ObjEnemy[R_ENGINE].SetRotation(r_angle + Vector3(0, 0, 0.05f));

		if (m_weapon_flag == true)
		{
			if (m_ObjEnemy[L_WEAPON].GetParent() != nullptr && m_ObjEnemy[R_WEAPON].GetParent() != nullptr)
			{
				Vector3 a =
					m_ObjEnemy[L_WEAPON].GetParent()->GetParent()->GetTranslation()
					+ m_ObjEnemy[L_WEAPON].GetParent()->GetTranslation()
					+ m_ObjEnemy[L_WEAPON].GetTranslation();

				Vector3 b =
					m_ObjEnemy[R_WEAPON].GetParent()->GetParent()->GetTranslation()
					+ m_ObjEnemy[R_WEAPON].GetParent()->GetTranslation()
					+ m_ObjEnemy[R_WEAPON].GetTranslation();

				Vector3 l_angle =
					m_ObjEnemy[L_WEAPON].GetParent()->GetParent()->GetRotation()
					+ m_ObjEnemy[L_WEAPON].GetParent()->GetRotation()
					+ m_ObjEnemy[L_WEAPON].GetRotation();

				Vector3 r_angle =
					m_ObjEnemy[R_WEAPON].GetParent()->GetParent()->GetRotation()
					+ m_ObjEnemy[R_WEAPON].GetParent()->GetRotation()
					+ m_ObjEnemy[R_WEAPON].GetRotation();

				m_ObjEnemy[L_WEAPON].SetRotation(l_angle);
				m_ObjEnemy[R_WEAPON].SetRotation(r_angle);

				m_a_flag = true;

				m_ObjEnemy[L_WEAPON].SetTranslation(a += Vector3(0, 0, -0.075f));
				m_ObjEnemy[R_WEAPON].SetTranslation(b += Vector3(0, 0, -0.075f));

				m_ObjEnemy[L_WEAPON].SetParent(nullptr);
				m_ObjEnemy[R_WEAPON].SetParent(nullptr);
			}
		}
	}

	//float angle = m_ObjEnemy[BODY].GetRotation().y;

	//移動量ベクトルを自機の角度分回転させる
	//m_moveV = Vector3::TransformNormal(m_moveV, m_ObjEnemy[BODY].GetWorld());


	//移動ベクトル(速度)
	m_moveV = Vector3(0.07f, 0.07f, 0.07f);

}

//	先読み型の自動追尾
void Enemy::PrefetchHoming(Vector3 targetPos)
{

}

//間合い確保型の自動追尾
void Enemy::IntervalHoming()
{
	//追尾対象(プレイヤー)へのベクトル
	Vector3 TurnVec = m_Player->GetTrans() - this->GetTrans();

	//ベクトルの正規化
	TurnVec.Normalize();

	//追尾対象へのベクトルに移動ベクトルを乗算する
	TurnVec = TurnVec * m_moveV;

	//座標を移動させる
	Vector3 pos = this->GetTrans();
	this->SetTrans(pos + TurnVec);


	//目標角度に向かって、機体の角度を補間する

	//敵の角度を回転させる
	Vector3 rotv = m_ObjEnemy[BODY].GetRotation();

	float angle = m_DistAngle - rotv.y;

	//180度を超える場合、逆回りにする
	if (angle > XM_PI)
	{
		angle -= XM_2PI;
	}

	if (angle < -XM_PI)
	{
		angle += XM_2PI;
	}

	//補間
	rotv.y += angle * 0.01f;

	rotv.y = 90.0f;

	this->SetRot(rotv);

	//m_ObjEnemy[BODY].SetRotation(rotv);
}