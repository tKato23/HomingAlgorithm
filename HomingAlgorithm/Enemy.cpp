#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//コンストラクタ
Enemy::Enemy()
{
	m_sinAngle = 0.0f;

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

	//初期位置の設定
	Vector3 pos;
	pos.x = rand() % 5;
	pos.y= 2;
	pos.z = rand() % 5;

	this->SetTrans(pos);

	{//弾丸用の当たり判定を設定する
		m_CollisionNodeEnemy.Initialize();

		m_CollisionNodeEnemy.SetParent(&m_ObjEnemy[BODY]);
		m_CollisionNodeEnemy.SetTrans(Vector3(0, 0, 0));
		m_CollisionNodeEnemy.SetLocalRadius(Vector3(0.8f, 0.8f, 0.8f));
	}
}

//更新処理
void Enemy::Update()
{
	//ロボットの挙動の更新
	this->Action();

	//定期的に進行方向を変える
	m_Timer--;	//メンバ変数でタイマーを作り、カウントダウン

	if (m_Timer < 0)
	{
		//カウントが0に達したらタイマーを60に戻す
		m_Timer = 60;

		//目標角度を変更
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		rnd *= 180.0f;

		rnd = XMConvertToRadians(rnd);

		//メンバ変数で目標角度を保持
		m_DistAngle += rnd;
	}

	//Vector3 angle = m_ObjEnemy[BODY].GetRotation();
	/*SetRot(Vector3(0, m_DistAngle, 0));*/

	//目標角度に向かって、機体の角度を補間する
	{
		//敵の角度を回転させる
		Vector3 rotv = m_ObjEnemy[BODY].GetRotation();

		////目標角度への、最短角度を取得
		//float angle = GetShortAngleRad(rotv.y, XMConvertToRadians(m_DistAngle));
		
		
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

		SetRot(rotv);

		//m_ObjEnemy[BODY].SetRotation(rotv);
	}

	//機体の向いている方向に進む
	{
		//今の座標を取得
		Vector3 trans = m_ObjEnemy[BODY].GetTranslation();

		Vector3 move(0, 0, -0.02f);
		Vector3 rotv = m_ObjEnemy[BODY].GetRotation();
		Matrix rotm = Matrix::CreateRotationY(rotv.y);
		move = Vector3::TransformNormal(move, rotm);

		//座標を移動
		trans += move;

		//移動後の座標をセット
		m_ObjEnemy[BODY].SetTranslation(trans);
	}

	//各パーツの更新
	Calc();

	//当たり判定の更新
	m_CollisionNodeEnemy.Update();
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
	Vector3 moveV(0, 0, -0.02f);

	float angle = m_ObjEnemy[BODY].GetRotation().y;

	//移動量ベクトルを自機の角度分回転させる
	moveV = Vector3::TransformNormal(moveV, m_ObjEnemy[BODY].GetWorld());

	////座標を移動させる
	Vector3 pos = m_ObjEnemy[BODY].GetTranslation();
	m_ObjEnemy[BODY].SetTranslation(pos + moveV);
}

//プレイヤーの移動を取得する
const DirectX::SimpleMath::Vector3& Enemy::GetRot()
{
	return m_ObjEnemy[BODY].GetRotation();
}

//プレイヤーの角度を取得する
const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	return m_ObjEnemy[BODY].GetTranslation();
}

//プレイヤーの移動をセットする
void Enemy::SetRot(const Vector3& rotation)
{
	m_ObjEnemy[BODY].SetRotation(rotation);
}

//プレイヤーの角度をセットする
void Enemy::SetTrans(const Vector3& translation)
{
	m_ObjEnemy[BODY].SetTranslation(translation);
}

//追尾ホーミング

//	先読み型の自動追尾
void Enemy::PrefetchHoming(Vector3 targetPos)
{

	////	敵とプレイヤー座標の差
	//Vector3 differencePos　m_player->GetTrans() - GetTrans();

	////	移動ベクトルの差
	//Vector3 differenceVec;

}