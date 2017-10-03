#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//コンストラクタ
Player::Player(DirectX::Keyboard * keyboard)
{
	m_pKeyboard = keyboard;
	m_weapon_speed = Vector3::Zero;
	m_weapon_flag = false;

	Initialize();
}

//デストラクタ
Player::~Player()
{

}

//初期化処理
void Player::Initialize()
{
	//自機パーツの読み込み
	m_ObjPlayer.resize(PLAYER_PARTS_NUM);
	m_ObjPlayer[BODY].LoadModel(L"Resources/body.cmo");
	m_ObjPlayer[L_WING].LoadModel(L"Resources/LeftWing.cmo");
	m_ObjPlayer[R_WING].LoadModel(L"Resources/RightWing.cmo");
	m_ObjPlayer[L_ENGINE].LoadModel(L"Resources/Engine.cmo");
	m_ObjPlayer[R_ENGINE].LoadModel(L"Resources/Engine.cmo");
	m_ObjPlayer[L_WEAPON].LoadModel(L"Resources/weapon.cmo");
	m_ObjPlayer[R_WEAPON].LoadModel(L"Resources/weapon.cmo");

	//パーツを親子関係をセット
	m_ObjPlayer[L_WING].SetParent(&m_ObjPlayer[BODY]);
	m_ObjPlayer[R_WING].SetParent(&m_ObjPlayer[BODY]);
	m_ObjPlayer[L_ENGINE].SetParent(&m_ObjPlayer[BODY]);
	m_ObjPlayer[R_ENGINE].SetParent(&m_ObjPlayer[BODY]);
	m_ObjPlayer[L_WEAPON].SetParent(&m_ObjPlayer[L_WING]);
	m_ObjPlayer[R_WEAPON].SetParent(&m_ObjPlayer[R_WING]);

	//親からのオフセット(座標のずらし分)をセット
	m_ObjPlayer[BODY].SetTranslation(Vector3(0.0f, 1.0f, 0.0f));
	m_ObjPlayer[L_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));
	m_ObjPlayer[R_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));
	m_ObjPlayer[L_ENGINE].SetTranslation(Vector3(-0.1f, 0.0f, 0.5f));
	m_ObjPlayer[L_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));
	m_ObjPlayer[R_ENGINE].SetTranslation(Vector3(0.1f, 0.0f, 0.5f));
	m_ObjPlayer[R_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));
	m_ObjPlayer[L_WEAPON].SetTranslation(Vector3(-0.5f, -0.05f, -0.025f));
	m_ObjPlayer[L_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));
	m_ObjPlayer[R_WEAPON].SetTranslation(Vector3(0.5f, -0.05f, -0.025f));
	m_ObjPlayer[R_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));


	//当たり判定を設定する(サークルとの排斥処理用)
	m_CollisionNodeBody.Initialize();
	m_CollisionNodeBody.SetParent(&m_ObjPlayer[BODY]);
	m_CollisionNodeBody.SetTrans(Vector3(0, 0, 0.15f));
	m_CollisionNodeBody.SetLocalRadius(Vector3(1.0f, 0.15f, 0.4f));
}

//更新処理
void Player::Update()
{
	//ロボットの挙動の更新
	this->Action();

	//行列更新
	Calc();
}

//行列更新
void Player::Calc()
{
	//プレイヤーの更新処理
	for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	{
		it->Update();
	}

	//当たり判定の更新処理
	m_CollisionNodeBody.Update();
}

//描画
void Player::Draw()
{
	//各パーツの描画
	for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	{
		it->Draw();
	}
}

//ロボットの挙動
void Player::Action()
{
	//左エンジンの回転
	Vector3 l_angle = m_ObjPlayer[L_ENGINE].GetRotation();
	m_ObjPlayer[L_ENGINE].SetRotation(l_angle + Vector3(0, 0, -0.05f));

	//右エンジンの回転
	Vector3 r_angle = m_ObjPlayer[R_ENGINE].GetRotation();
	m_ObjPlayer[R_ENGINE].SetRotation(r_angle + Vector3(0, 0, 0.05f));

	// キーボードの更新
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	//現在の座標・回転角を取得
	Vector3 trans = this->GetTrans();

	//移動ベクトル
	m_moveV = Vector3(0.0f, 0.0f, 0.0f);

	//移動ベクトルを回転する
	m_moveV = Vector3::TransformNormal(m_moveV, m_ObjPlayer[BODY].GetWorld());

	//移動
	trans += m_moveV;

	//移動した座標を反映
	this->SetTrans(trans);

	//前進
	if (keystate.W || keystate.Up)
	{
		//現在の座標・回転角を取得
		Vector3 trans = m_ObjPlayer[BODY].GetTranslation();

		//移動ベクトル(Z座標の前進)
		m_moveV = Vector3(0.0f, 0.0f, -0.1f);

		//移動ベクトルを回転する
		m_moveV = Vector3::TransformNormal(m_moveV, m_ObjPlayer[BODY].GetWorld());

		//移動
		trans += m_moveV;

		//移動した座標を反映
		m_ObjPlayer[BODY].SetTranslation(trans);
	}

	//後退
	if (keystate.S || keystate.Down)
	{
		//現在の座標・回転角を取得
		Vector3 trans = m_ObjPlayer[BODY].GetTranslation();

		//移動ベクトル(Z座標の前進)
		SimpleMath::Vector3 moveV(0.0f, 0.0f, 0.1f);

		//移動ベクトルを回転する
		moveV = Vector3::TransformNormal(moveV, m_ObjPlayer[BODY].GetWorld());

		//移動
		trans += moveV;

		//移動した座標を反映
		m_ObjPlayer[BODY].SetTranslation(trans);
	}

	//左旋回
	if (keystate.A || keystate.Left)
	{
		//現在の角度を取得
		Quaternion rot = m_ObjPlayer[BODY].GetRotationQ();
		Quaternion rotadd = Quaternion::CreateFromAxisAngle(Vector3::UnitY, ROT_SPEED);
		rot = rotadd * rot;

		//回転後の角度を反映
		m_ObjPlayer[BODY].SetRotationQ(rot);
	}

	//右旋回
	if (keystate.D || keystate.Right)
	{
		//現在の角度を取得
		Quaternion rot = m_ObjPlayer[BODY].GetRotationQ();
		Quaternion rotadd = Quaternion::CreateFromAxisAngle(Vector3::UnitY, -ROT_SPEED);
		rot = rotadd * rot;

		//回転後の角度を反映
		m_ObjPlayer[BODY].SetRotationQ(rot);
	}

	//ミサイル発射
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		if (m_weapon_flag)
		{
			ResetWeapon();
		}
		else
		{
			FireWeapon();
		}
	}

	// 弾丸を前進させる
	if (m_weapon_flag)
	{
		Vector3 pos_l = m_ObjPlayer[L_WEAPON].GetTranslation();
		Vector3 pos_r = m_ObjPlayer[R_WEAPON].GetTranslation();

		m_ObjPlayer[L_WEAPON].SetTranslation(pos_l + m_weapon_speed);
		m_ObjPlayer[R_WEAPON].SetTranslation(pos_r + m_weapon_speed);
	}
}

//ミサイルを発射する関数
void Player::FireWeapon()
{
	m_weapon_flag = false;

	//発射するパーツのワールド行列を取得
	Matrix worldm_l = m_ObjPlayer[L_WEAPON].GetWorld();
	Matrix worldm_r = m_ObjPlayer[R_WEAPON].GetWorld();

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
	m_ObjPlayer[L_WEAPON].SetParent(nullptr);
	m_ObjPlayer[L_WEAPON].SetScale(scale_l);
	m_ObjPlayer[L_WEAPON].SetRotationQ(rotation_l);
	m_ObjPlayer[L_WEAPON].SetTranslation(translation_l);

	m_ObjPlayer[R_WEAPON].SetParent(nullptr);
	m_ObjPlayer[R_WEAPON].SetScale(scale_r);
	m_ObjPlayer[R_WEAPON].SetRotationQ(rotation_r);
	m_ObjPlayer[R_WEAPON].SetTranslation(translation_r);

	//弾丸の速度を設定
	m_weapon_speed = Vector3(0.0f, 0.0f, -0.1f);
	m_weapon_speed = Vector3::Transform(m_weapon_speed, rotation_l);

	m_weapon_flag = true;
}

//ミサイルを再装着する関数
void Player::ResetWeapon()
{
	if (!m_weapon_flag)
	{
		return;
	}

	//親子関係とオフセットを初期値に戻す
	m_ObjPlayer[L_WEAPON].SetParent(&m_ObjPlayer[L_WING]);
	m_ObjPlayer[L_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));
	m_ObjPlayer[L_WEAPON].SetRotation(Vector3(0, 0, 0));
	m_ObjPlayer[L_WEAPON].SetTranslation(Vector3(-0.5f, -0.05f, -0.025f));

	m_ObjPlayer[R_WEAPON].SetParent(&m_ObjPlayer[R_WING]);
	m_ObjPlayer[R_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));
	m_ObjPlayer[R_WEAPON].SetRotation(Vector3(0, 0, 0));
	m_ObjPlayer[R_WEAPON].SetTranslation(Vector3(0.5f, -0.05f, -0.025f));

	m_weapon_flag = false;
}