#include "Enemy.h"
#include <math.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//	静的メンバ定数の定義
const float Enemy::INTERVAL_SPACE = 2.5f;
const float Enemy::MOVE_SPEED = -0.02f;

//コンストラクタ
Enemy::Enemy()
{
	m_Timer = 0;
	m_weapon_speed = Vector3::Zero;
	m_weapon_flag = false;
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

	{//弾丸用の当たり判定を設定する
		m_CollisionNodeEnemy.Initialize();

		m_CollisionNodeEnemy.SetParent(&m_ObjEnemy[BODY]);
		m_CollisionNodeEnemy.SetTrans(Vector3(0, 0, 0));
		m_CollisionNodeEnemy.SetLocalRadius(Vector3(0.8f, 0.8f, 0.8f));
	}

	m_homingFlag = false;

	//	タイプの配列
	Homing::Type strategyTypearray[] = {
		Homing::PREFETCH,
		Homing::INTERVAL,
		Homing::AMBUSH,
		Homing::PURSUIT
	};

	//	それぞれのタイプを追加
	for (int i = 0; i < MAX_STRATEGY_NUM; i++)
	{
		addStrategy(strategyTypearray[i]);
	}

	//	初期化として先読み型をセット
	this->SetHomingType(Homing::Type::PREFETCH);
}

//更新処理
void Enemy::Update()
{
	//ロボットの挙動の更新
	this->Action();

	//各パーツの更新
	Calc();

	//当たり判定の更新
	m_CollisionNodeEnemy.Update();

	//	プレイヤーへの追尾
	if (m_homingFlag == true)
	{
		this->HomingExecute();
	}

	m_Timer++;

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

	}
	//左エンジンの回転
	Vector3 l_angle = m_ObjEnemy[L_ENGINE].GetRotation();
	m_ObjEnemy[L_ENGINE].SetRotation(l_angle + Vector3(0, 0, -0.05f));

	//右エンジンの回転
	Vector3 r_angle = m_ObjEnemy[R_ENGINE].GetRotation();
	m_ObjEnemy[R_ENGINE].SetRotation(r_angle + Vector3(0, 0, 0.05f));

	//移動ベクトル
	m_moveV = Vector3(0.0f, 0.0f, 0.0f);

	//移動量ベクトルを自機の角度分回転させる
	m_moveV = Vector3::TransformNormal(m_moveV, m_ObjEnemy[BODY].GetWorld());

	//ミサイル発射
	//if (m_Timer >= 60)
	//{
		//if (m_weapon_flag)
		//{
			//ResetWeapon();
		//}
		//else
		//{
			//FireWeapon();
		//}
	//}

	//// 弾丸を前進させる
	//if (m_weapon_flag)
	//{
	//	Vector3 pos_l = m_ObjEnemy[L_WEAPON].GetTranslation();
	//	Vector3 pos_r = m_ObjEnemy[R_WEAPON].GetTranslation();

	//	m_ObjEnemy[L_WEAPON].SetTranslation(pos_l + m_weapon_speed);
	//	m_ObjEnemy[R_WEAPON].SetTranslation(pos_r + m_weapon_speed);
	//}
}

//ミサイルを発射する関数
void Enemy::FireWeapon()
{
	m_weapon_flag = false;

	//発射するパーツのワールド行列を取得
	Matrix worldm_l = m_ObjEnemy[L_WEAPON].GetWorld();
	Matrix worldm_r = m_ObjEnemy[R_WEAPON].GetWorld();

	Vector3 scale_l;		//ワールドスケーリング(左ミサイル)
	Vector3 scale_r;		//ワールドスケーリング(右ミサイル)

	Quaternion rotation_l;	//ワールド回転(左ミサイル)
	Quaternion rotation_r;	//ワールド回転(右ミサイル)

	Vector3 translation_l;	//ワールド座標(左ミサイル)
	Vector3 translation_r;	//ワールド座標(右ミサイル)

	//ワールド行列から各要素を抽出
	worldm_l.Decompose(scale_l, rotation_l, translation_l);
	worldm_r.Decompose(scale_r, rotation_r, translation_r);

	//発射パーツを親から分離して独立
	m_ObjEnemy[L_WEAPON].SetParent(nullptr);
	m_ObjEnemy[L_WEAPON].SetScale(scale_l);
	m_ObjEnemy[L_WEAPON].SetRotationQ(rotation_l);
	m_ObjEnemy[L_WEAPON].SetTranslation(translation_l);

	m_ObjEnemy[R_WEAPON].SetParent(nullptr);
	m_ObjEnemy[R_WEAPON].SetScale(scale_r);
	m_ObjEnemy[R_WEAPON].SetRotationQ(rotation_r);
	m_ObjEnemy[R_WEAPON].SetTranslation(translation_r);

	//弾丸の速度を設定
	m_weapon_speed = Vector3(MOVE_SPEED, MOVE_SPEED, MOVE_SPEED);
	m_weapon_speed = Vector3::Transform(m_weapon_speed, rotation_l);

	//追尾対象(プレイヤー)へのベクトル
	Vector3 Vec_l = m_Player->GetTrans() - m_ObjEnemy[L_WEAPON].GetTranslation();
	Vector3 Vec_r = m_Player->GetTrans() - m_ObjEnemy[R_WEAPON].GetTranslation();

	//float angle_X = atan2f(Vec_l.z, Vec_l.y);
	//float angle_Y = atan2f(Vec_l.x, Vec_l.z);
	//float angle_Z = atan2f(Vec_l.y, Vec_l.x);

	////this->SetRot(Vector3(angle_X + XM_PI, angle_Y + XM_PI, 0.0f));
	//m_ObjEnemy[L_WEAPON].SetRotation(Vector3(0.0f, angle_Y + XM_PI, 0.0f));
	//m_ObjEnemy[R_WEAPON].SetRotation(Vector3(0.0f, angle_Y + XM_PI, 0.0f));

	//ベクトルの正規化
	Vec_l.Normalize();
	Vec_r.Normalize();

	//追尾対象へのベクトルに移動ベクトルを乗算する
	Vec_l = Vec_l * m_weapon_speed;
	Vec_r = Vec_r * m_weapon_speed;

	// 弾丸を前進させる
	Vector3 pos_l = m_ObjEnemy[L_WEAPON].GetTranslation();
	Vector3 pos_r = m_ObjEnemy[R_WEAPON].GetTranslation();

	m_ObjEnemy[L_WEAPON].SetTranslation(pos_l + Vec_l);
	m_ObjEnemy[R_WEAPON].SetTranslation(pos_r + Vec_r);

	m_weapon_flag = true;
}

////追跡型の自動追尾
//void Enemy::PursuitHouming()
//{
//	//移動ベクトル(速度)
//	m_moveV = Vector3(0.07f, 0.07f, 0.07f);
//
//	//プレイヤーへの向き
//	Vector3 direction = m_Player->GetTrans() - this->GetTrans();
//	//正規化
//	direction.Normalize();
//	//速度ベクトル
//	direction = direction*m_moveV;
//	//座標を移動させる
//	Vector3 pos = this->GetTrans();
//	this->SetTrans(pos + direction);
//	this->SetRot(m_Player->GetRot());
//}
//
////	先読み型の自動追尾
//void Enemy::PrefetchHoming()
//{
//	//	相対距離
//	Vector3 dis;
//	dis.x = m_Player->GetTrans().x - GetTrans().x;
//	dis.y = m_Player->GetTrans().y - GetTrans().y;
//	dis.z = m_Player->GetTrans().z - GetTrans().z;
//
//	//	相対速度
//	Vector3 vel;
//	vel.x = m_Player->GetMoveV().x - GetMoveV().x;
//	vel.y = m_Player->GetMoveV().y - GetMoveV().y;
//	vel.z = m_Player->GetMoveV().z - GetMoveV().z;
//
//	//	接近時間　
//	float time = 0.0f;
//	float distance = sqrtf(dis.x * dis.x + dis.y * dis.y + dis.z * dis.z);
//	float velocity = sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
//
//	//float angle = m_ObjEnemy[BODY].GetRotation().y;
//
//	//移動量ベクトルを自機の角度分回転させる
//	//m_moveV = Vector3::TransformNormal(m_moveV, m_ObjEnemy[BODY].GetWorld());
//
//	//	プレイヤーの方向への移動ベクトルの回転？
//	//Vector3 EnemyRot = Vector3(0.0f, pos.y, 0.0f);
//	//SetRot(EnemyRot);
//
//
//	//移動ベクトル(速度)
//	m_moveV = Vector3(0.07f, 0.07f, 0.07f);
//
//	//	敵に反映させる
//	this->SetTrans(this->GetTrans() - pos);
//
//	pos.Normalize();
//	float angle = atan2f(pos.x, pos.z);
//	SetRot(Vector3(0.0f, angle + XM_2PI, 0.0f));
//
//}
//
//
////待ち伏せ型の自動追尾
//void Enemy::AmbushHoming()
//{
//	//追尾対象(プレイヤー)へのベクトル
//	Vector3 TurnVec = m_Player->GetTrans() - this->GetTrans();
//
//	//	一定距離以内なら追尾を行う
//	if (TurnVec.x < 10.0f &&  TurnVec.x > -10.0f && TurnVec.y <= 10.0f 
//		&& TurnVec.y > -10.0f && TurnVec.z < 10.0f && TurnVec.z > -10.0f)
//	{
//		//ベクトルの正規化
//		TurnVec.Normalize();
//
//		//追尾対象へのベクトルに移動ベクトルを乗算する
//		TurnVec = TurnVec * m_moveV;
//
//		//座標を移動させる
//		Vector3 pos = this->GetTrans();
//		this->SetTrans(pos + TurnVec);
//
//		float angle = atan2f(TurnVec.x, TurnVec.z);
//		SetRot(Vector3(0.0f, angle + XM_PI, 0.0f));
//
//	}
//}
//
////間合い確保型の自動追尾
//void Enemy::IntervalHoming()
//{
//	//移動ベクトル
//	m_moveV = Vector3(MOVE_SPEED, MOVE_SPEED, MOVE_SPEED);
//
//	//追尾対象(プレイヤー)へのベクトル
//	Vector3 Vec = m_Player->GetTrans() - this->GetTrans();
//
//	float angle_X = atan2f(Vec.z, Vec.y);
//	float angle_Y = atan2f(Vec.x, Vec.z);
//	float angle_Z = atan2f(Vec.y, Vec.x);
//
//	this->SetRot(Vector3(angle_X + XM_PI, angle_Y + XM_PI, 0.0f));
//	//this->SetRot(Vector3(0.0f, angle_Y, 0.0f));
//
//	//ベクトルの正規化
//	Vec.Normalize();
//
//	//追尾対象へのベクトルに移動ベクトルを乗算する
//	Vec = Vec * m_moveV;
//
//	//座標を移動させる
//	Vector3 pos = this->GetTrans();
//
//	this->SetTrans(pos + TurnVec);
//}

//	ホーミングのフラグをセットする
void Enemy::SetHomingFlag(bool flag)
{
	m_homingFlag = flag;
}

//ミサイルを再装着する関数
void Enemy::ResetWeapon()
{
	if (!m_weapon_flag)
	{
		return;
	}

	//親子関係とオフセットを初期値に戻す
	m_ObjEnemy[L_WEAPON].SetParent(&m_ObjEnemy[L_WING]);
	m_ObjEnemy[L_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));
	m_ObjEnemy[L_WEAPON].SetRotation(Vector3(0, 0, 0));
	m_ObjEnemy[L_WEAPON].SetTranslation(Vector3(-0.5f, -0.05f, -0.025f));

	m_ObjEnemy[R_WEAPON].SetParent(&m_ObjEnemy[R_WING]);
	m_ObjEnemy[R_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));
	m_ObjEnemy[R_WEAPON].SetRotation(Vector3(0, 0, 0));
	m_ObjEnemy[R_WEAPON].SetTranslation(Vector3(0.5f, -0.05f, -0.025f));

	m_weapon_flag = false;
}

//	dictionaryに追加を行う関数
void Enemy::addStrategy(Homing::Type type)
{

	switch (type)
	{
	case Homing::PREFETCH:
		m_homingDictionary[type] = new Prefetch();
		break;

	case Homing::INTERVAL:
		m_homingDictionary[type] = new Interval();
		break;

	case Homing::AMBUSH:
		m_homingDictionary[type] = new Ambush();
		break;

	case Homing::PURSUIT:
		m_homingDictionary[type] = new Pursuit();
		break;

	default:
		break;
	}
}

//待ち伏せ型の自動追尾
void Enemy::AmbushHoming()
{
	//追尾対象(プレイヤー)へのベクトル
	Vector3 TurnVec = m_Player->GetTrans() - this->GetTrans();
}

//	実行する
void Enemy::HomingExecute()
{
	m_homingDictionary[m_currentType]->homing(*m_Player, *this);
}

////間合い確保型の自動追尾
//void Enemy::IntervalHoming()
//{
//	//追尾対象(プレイヤー)へのベクトル
//	Vector3 Vec = m_Player->GetTrans() - this->GetTrans();
//
//	float distance_Square;
//
//	distance_Square = Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z;
//
//	//半径の和の二乗
//	float radius_Square;
//
//	radius_Square = INTERVAL_SPACE + INTERVAL_SPACE;
//	radius_Square = radius_Square * radius_Square;
//
//	//距離が半径の和より大きければ当たっていない
//	if (distance_Square > radius_Square)
//	{
//		//ベクトルの正規化
//		Vec.Normalize();
//
//		//移動ベクトル
//		m_moveV = Vector3(0.07f, 0.07f, 0.07f);
//
//		//追尾対象へのベクトルに移動ベクトルを乗算する
//		Vec = Vec * m_moveV;
//
//		//float angle_X = atan2f(Vec.z, Vec.y) - XM_PIDIV2;
//		float angle_Y = atan2f(Vec.x, Vec.z) + XM_PI;
//		//float angle_Z = atan2f(Vec.y, Vec.x);
//
//		this->SetRot(Vector3(0.0f, angle_Y, 0.0f));
//
//		//座標を移動させる
//		Vector3 pos = this->GetTrans();
//		this->SetTrans(pos + Vec);
//	}
	//else
	//{
	//	//ベクトルの正規化
	//	Vec.Normalize();

	//	//移動ベクトル
	//	m_moveV = Vector3(MOVE_SPEED, MOVE_SPEED, MOVE_SPEED);

	//	//追尾対象へのベクトルに移動ベクトルを乗算する
	//	Vec = Vec * m_moveV;

	//	//float angle_X = atan2f(Vec.z, Vec.y) - XM_PIDIV2;
	//	float angle_Y = atan2f(Vec.x, Vec.z) + XM_PI;
	//	//float angle_Z = atan2f(Vec.y, Vec.x);

	//	this->SetRot(Vector3(0.0f, angle_Y, 0.0f));

	//	//座標を移動させる
	//	Vector3 pos = this->GetTrans();
	//	this->SetTrans(pos - Vec);
	//}
//}
