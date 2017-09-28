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

	{//弾丸用の当たり判定を設定する
		m_CollisionNodeEnemy.Initialize();

		m_CollisionNodeEnemy.SetParent(&m_ObjEnemy[BODY]);
		m_CollisionNodeEnemy.SetTrans(Vector3(0, 0, 0));
		m_CollisionNodeEnemy.SetLocalRadius(Vector3(0.8f, 0.8f, 0.8f));
	}

	//m_oldTargetPos = Vector3::Zero;
}

//更新処理
void Enemy::Update()
{
	//ロボットの挙動の更新
	this->Action();

	//定期的に進行方向を変える
	m_Timer++;	//メンバ変数でタイマーを作り、カウントダウン

	//if (m_Timer < 0)
	//{
		////カウントが0に達したらタイマーを60に戻す
		//m_Timer = 60;

		////目標角度を変更
		//float rnd = (float)rand() / RAND_MAX - 0.5f;
		//rnd *= 180.0f;

		//rnd = XMConvertToRadians(rnd);

		////メンバ変数で目標角度を保持
		//m_DistAngle += rnd;
	//}

	//Vector3 angle = m_ObjEnemy[BODY].GetRotation();
	/*SetRot(Vector3(0, m_DistAngle, 0));*/

	//目標角度に向かって、機体の角度を補間する
	{
		////敵の角度を回転させる
		//Vector3 rotv = m_ObjEnemy[BODY].GetRotation();

		//////目標角度への、最短角度を取得
		////float angle = GetShortAngleRad(rotv.y, XMConvertToRadians(m_DistAngle));
		//
		//
		//float angle = m_DistAngle - rotv.y;

		////180度を超える場合、逆回りにする
		//if (angle > XM_PI)
		//{
		//	angle -= XM_2PI;
		//}

		//if (angle < -XM_PI)
		//{
		//	angle += XM_2PI;
		//}


		////補間
		//rotv.y += angle * 0.01f;

		//SetRot(rotv);

		//m_ObjEnemy[BODY].SetRotation(rotv);
	}

	//機体の向いている方向に進む
	{
		////今の座標を取得
		//Vector3 trans = m_ObjEnemy[BODY].GetTranslation();

		//Vector3 rotv = m_ObjEnemy[BODY].GetRotation();
		//Matrix rotm = Matrix::CreateRotationY(rotv.y);
		//m_moveV = Vector3::TransformNormal(m_moveV, rotm);

		////座標を移動
		//trans += m_moveV;

		////移動後の座標をセット
		//m_ObjEnemy[BODY].SetTranslation(trans);
	}

	//各パーツの更新
	Calc();

	//当たり判定の更新
	m_CollisionNodeEnemy.Update();

	//	プレイヤーへの追尾
	//PrefetchHoming();

	TurnHoming();
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
	m_moveV = Vector3(0.0f, 0.0f, -0.02f);

	float angle = m_ObjEnemy[BODY].GetRotation().y;

	//移動量ベクトルを自機の角度分回転させる
	m_moveV = Vector3::TransformNormal(m_moveV, m_ObjEnemy[BODY].GetWorld());
}

//エネミーの角度を取得する
const DirectX::SimpleMath::Vector3& Enemy::GetRot()
{
	return m_ObjEnemy[BODY].GetRotation();
}

//エネミーの位置を取得する
const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	return m_ObjEnemy[BODY].GetTranslation();
}

//エネミーの移動ベクトルを取得する
const DirectX::SimpleMath::Vector3 & Enemy::GetMoveV()
{
	return m_moveV;
}

//エネミーの角度をセットする
void Enemy::SetRot(const Vector3& rotation)
{
	m_ObjEnemy[BODY].SetRotation(rotation);
}

//エネミーの位置をセットする
void Enemy::SetTrans(const Vector3& translation)
{
	m_ObjEnemy[BODY].SetTranslation(translation);
}

//追尾ホーミング

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
	//Vector3 time = differencePos / differenceVec;
	float time = 0.0f;
	float distance = sqrtf(dis.x * dis.x + dis.y * dis.y + dis.z * dis.z);
	float velocity = sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);

	time = distance / velocity;
	time = fabsf(time);

	//	予測地点
	Vector3 pos;
	pos.x = m_Player->GetTrans().x + (m_Player->GetMoveV().x * time);
	pos.y = m_Player->GetTrans().y + (m_Player->GetMoveV().y * time);
	pos.z = m_Player->GetTrans().z + (m_Player->GetMoveV().z * time);

	//	ベクトル
	pos = pos - GetTrans();

	Vector3 EnemyRot = Vector3(0.0f, pos.y, 0.0f);
	SetRot(EnemyRot);

	//pos.Normalize();
	pos = pos * this->GetMoveV();

	////	目標地点
	//Vector3 targetPoint =  m_Player->GetTrans() + (m_Player->GetMoveV() * time);

	////	予測点の方向
	//Vector3 dir = targetPoint - GetTrans();
	//dir.Normalize();

	////	敵の移動先
	//Vector3 move = dir * GetMoveV();


	//	敵に反映させる
	this->SetTrans(GetTrans() - pos);
}

//void Enemy::UpdateBresenham(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 targetPos)
//{
//	if (m_stepCnt >= STEP_MAX)
//	{
//		return;
//	}
//	Vector3 nowPos = pos;
//
//	int deltaX = targetPos.x - nowPos.x;
//	int deltaY = targetPos.y - nowPos.y;
//
//	const int stepX = (deltaX >= 0) ? 1 : -1;
//	const int stepY = (deltaY >= 0) ? 1 : -1;
//
//	deltaX = abs(deltaX);
//	deltaY = abs(deltaY);
//
//	if (deltaY > deltaX)
//	{
//		int f = (deltaX << 1) - deltaY;
//		for (int i = 0; i < deltaY; i++)
//		{
//			if (f >= 0)
//			{
//				nowPos.x += stepX;
//				f -= (deltaY << 1);
//			}
//			nowPos.y += stepY;
//			f += (deltaX << 1);
//
//			m_nextPos[i] = nowPos;
//		}
//	}
//	else
//	{
//		int f = (deltaX << 1) - deltaY;
//		for (int i = 0; i < deltaY; i++)
//		{
//			if (f >= 0)
//			{
//				nowPos.x += stepX;
//				f -= (deltaY << 1);
//			}
//			nowPos.y += stepY;
//			f += (deltaX << 1);
//
//			m_nextPos[i] = nowPos;
//		}
//	}
//}
//}

//旋回型の自動追尾
void Enemy::TurnHoming()
{
	//移動ベクトル(速度)
	m_moveV = Vector3(0.07f, 0.07f, 0.07f);

	//追尾対象(プレイヤー)へのベクトル
	Vector3 TurnVec = m_Player->GetTrans() - this->GetTrans();

	//ベクトルの正規化
	TurnVec.Normalize();

	//追尾対象へのベクトルに移動ベクトルを乗算する
	TurnVec = TurnVec * m_moveV;

	//座標を移動させる
	Vector3 pos = this->GetTrans();
	this->SetTrans(pos + TurnVec);

	this->SetRot(m_Player->GetRot());

	//プレイヤーがエネミーに向かって左から接近
	//if (TurnVec.x > -5.0f)
	//{
		//this->SetTrans(Vector3(m_Player->GetTrans().x + 5.0f, this->GetTrans().y, this->GetTrans().z));
	//}
	//if (TurnVec.x < 5.0f && !(TurnVec.x > -5.0f) )
	//{
	//	this->SetTrans(Vector3(m_Player->GetTrans().x + 5.0f, this->GetTrans().y, this->GetTrans().z));
	//}

	//Vector3 speed = TurnVec * this->GetMoveV();

	//this->SetTrans(a);
}