#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//コンストラクタ
Enemy::Enemy()
{
	m_sinAngle = 0.0f;
	m_Player = nullptr;

	Initialize();

	//	初期化処理
	m_Player = nullptr;
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
}

//更新処理
void Enemy::Update()
{
	//ロボットの挙動の更新
	this->Action();

	////定期的に進行方向を変える
	//m_Timer--;	//メンバ変数でタイマーを作り、カウントダウン


	if (m_Timer < 0)
	{
		//カウントが0に達したらタイマーを60に戻す
		//_Timer = 1;

	//if (m_Timer < 0)
	//{
	//	//カウントが0に達したらタイマーを60に戻す
	//	m_Timer = 60;


	//	//目標角度を変更
	//	float rnd = (float)rand() / RAND_MAX - 0.5f;
	//	rnd *= 180.0f;

	//	rnd = XMConvertToRadians(rnd);


		////メンバ変数で目標角度を保持
		//m_DistAngle += rnd;
	}

	/*Vector3 angle = m_ObjEnemy[BODY].GetRotation();
	SetRot(Vector3(0, m_DistAngle, 0));*/

	//	//メンバ変数で目標角度を保持
	//	m_DistAngle += rnd;
	//}

	//Vector3 angle = m_ObjEnemy[BODY].GetRotation();
	///*SetRot(Vector3(0, m_DistAngle, 0));*/


	////目標角度に向かって、機体の角度を補間する
	//{
	//	//敵の角度を回転させる
	//	Vector3 rotv = m_ObjEnemy[BODY].GetRotation();

	//	////目標角度への、最短角度を取得
	//	//float angle = GetShortAngleRad(rotv.y, XMConvertToRadians(m_DistAngle));
	//	
	//	
	//	float angle = m_DistAngle - rotv.y;

	//	//180度を超える場合、逆回りにする
	//	if (angle > XM_PI)
	//	{
	//		angle -= XM_2PI;
	//	}

	//	if (angle < -XM_PI)
	//	{
	//		angle += XM_2PI;
	//	}


	//	//補間
	//	rotv.y += angle * 0.01f;

	//	SetRot(rotv);

	//	m_ObjEnemy[BODY].SetRotation(rotv);
	//}

	//機体の向いている方向に進む
	{
		////今の座標を取得
		//Vector3 trans = m_ObjEnemy[BODY].GetTranslation();

		//Vector3 rotv = m_ObjEnemy[BODY].GetRotation();
		//Matrix rotm = Matrix::CreateRotationY(rotv.y);
		//m_moveV = Vector3::TransformNormal(m_moveV, rotm);

		////座標を移動
		//trans += m_moveV;

	//当たり判定の更新
		m_CollisionNodeEnemy.Update();

		IntervalHoming();

		//各パーツの更新
		Calc();
	}

	//当たり判定の更新
	m_CollisionNodeEnemy.Update();

	//	プレイヤーへの追尾

	//PursuitHouming();

	//PrefetchHoming();

	//TurnHoming();

	//	先読み型のホーミング
	//PrefetchHoming();

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
	//移動ベクトル（Z座標）
	m_moveV = Vector3(MOVE_SPEED, MOVE_SPEED, MOVE_SPEED);

	//移動量ベクトルを自機の角度分回転させる
	//m_moveV = Vector3::TransformNormal(m_moveV, m_ObjEnemy[BODY].GetWorld());
}


//追跡型の自動追尾
void Enemy::PursuitHouming()
{
	//移動ベクトル(速度)
	m_moveV = Vector3(0.07f, 0.07f, 0.07f);

	//プレイヤーへの向き
	Vector3 direction = m_Player->GetTrans() - this->GetTrans();
	//正規化
	direction.Normalize();
	//速度ベクトル
	direction = direction*m_moveV;
	//座標を移動させる
	Vector3 pos = this->GetTrans();
	this->SetTrans(pos + direction);
	this->SetRot(m_Player->GetRot());
}

//	先読み型の自動追尾
void Enemy::PrefetchHoming()
{
	////	敵とプレイヤー座標の差
	//Vector3 differencePos = m_Player->GetTrans() - GetTrans();

	////	移動ベクトルの差
	//Vector3 differenceVec = m_Player->GetMoveV() - GetMoveV();

	//	相対距離
	Vector3 dis;
	dis.x = m_Player->GetTrans().x - GetTrans().x;
	dis.y = m_Player->GetTrans().y - GetTrans().y;
	dis.z = m_Player->GetTrans().z - GetTrans().z;

	//	相対速度
	Vector3 vel;
	vel.x = m_Player->GetMoveV().x - GetMoveV().x;
	vel.y = m_Player->GetMoveV().y - GetMoveV().y;
	vel.z = m_Player->GetMoveV().z - GetMoveV().z;

	//	接近時間　
	float time = 0.0f;
	float distance = sqrtf(dis.x * dis.x + dis.y * dis.y + dis.z * dis.z);
	float velocity = sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);

	//float angle = m_ObjEnemy[BODY].GetRotation().y;

	//移動量ベクトルを自機の角度分回転させる
	//m_moveV = Vector3::TransformNormal(m_moveV, m_ObjEnemy[BODY].GetWorld());


	//移動ベクトル(速度)
	m_moveV = Vector3(0.07f, 0.07f, 0.07f);

}


//待ち伏せ型の自動追尾
void Enemy::AmbushHoming()
{
	//追尾対象(プレイヤー)へのベクトル
	Vector3 TurnVec = m_Player->GetTrans() - this->GetTrans();

	//	一定距離以内なら追尾を行う
	if (TurnVec.x < 10.0f &&  TurnVec.x > -10.0f && TurnVec.y <= 10.0f 
		&& TurnVec.y > -10.0f && TurnVec.z < 10.0f && TurnVec.z > -10.0f)
	{
		//ベクトルの正規化
		TurnVec.Normalize();

		//追尾対象へのベクトルに移動ベクトルを乗算する
		TurnVec = TurnVec * m_moveV;

		//座標を移動させる
		Vector3 pos = this->GetTrans();
		this->SetTrans(pos + TurnVec);

		this->SetRot(m_Player->GetRot());
	}
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