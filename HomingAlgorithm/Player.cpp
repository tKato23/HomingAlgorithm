
//ヘッダファイルのインクルード
#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//コンストラクタ
Player::Player(DirectX::Keyboard * keyboard)
{
	m_pKeyboard = keyboard;
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

	//パーツを親子関係をセット
	m_ObjPlayer[L_WING].SetParent(&m_ObjPlayer[BODY]);
	m_ObjPlayer[R_WING].SetParent(&m_ObjPlayer[BODY]);
	m_ObjPlayer[L_ENGINE].SetParent(&m_ObjPlayer[BODY]);
	m_ObjPlayer[R_ENGINE].SetParent(&m_ObjPlayer[BODY]);

	//親からのオフセット(座標のずらし分)をセット
	m_ObjPlayer[BODY].SetTranslation(Vector3(0.0f, 1.0f, 0.0f));
	m_ObjPlayer[L_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));
	m_ObjPlayer[R_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));
	m_ObjPlayer[L_ENGINE].SetTranslation(Vector3(-0.1f, 0.0f, 0.5f));
	m_ObjPlayer[L_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));
	m_ObjPlayer[R_ENGINE].SetTranslation(Vector3(0.1f, 0.0f, 0.5f));
	m_ObjPlayer[R_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));

	//当たり判定を設定する
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
}