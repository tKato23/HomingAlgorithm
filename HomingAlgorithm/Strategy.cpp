//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Strategy.cpp
//!
//! @brief  ストラテジーパターンのcppファイル
//!
//! @date   2017/09/29
//!
//! @author 加藤　竜哉
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

//	ヘッダファイルのインクルード
#include "Strategy.h"
#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//	先読み型ホーミング
void Prefetch::homing(Player& player, Enemy& enemy)
{
	Vector3 moveV = Vector3(0.02f, 0.02f, 0.02f);

	//	相対距離
	Vector3 dis;
	dis.x = player.GetTrans().x - enemy.GetTrans().x;
	dis.y = player.GetTrans().y - enemy.GetTrans().y;
	dis.z = player.GetTrans().z - enemy.GetTrans().z;

	//	相対速度
	Vector3 vel;
	vel.x = player.GetMoveV().x - moveV.x;
	vel.y = player.GetMoveV().y - moveV.y;
	vel.z = player.GetMoveV().z - moveV.z;

	//	接近時間　
	float time = 0.0f;
	float distance = sqrtf(dis.x * dis.x + dis.y * dis.y + dis.z * dis.z);
	float velocity = sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);

	time = distance / velocity;
	time = fabsf(time);

	//	予測地点
	Vector3 pos;
	pos.x = player.GetTrans().x + (player.GetMoveV().x * time);
	pos.y = player.GetTrans().y + (player.GetMoveV().y * time);
	pos.z = player.GetTrans().z + (player.GetMoveV().z * time);

	//	ベクトル
	pos = pos - enemy.GetTrans();

	//	プレイヤーの方向への移動ベクトルの回転？
	//Vector3 EnemyRot = Vector3(0.0f, pos.y, 0.0f);
	//enemy.SetRot(EnemyRot);

	pos = pos * moveV;

	//	敵に反映させる
	enemy.SetTrans(enemy.GetTrans() + pos);
}

//	間合い確保型ホーミング
void Interval::homing(Player& player, Enemy& enemy)
{
	//追尾対象(プレイヤー)へのベクトル
	Vector3 TurnVec = player.GetTrans() - enemy.GetTrans();

	//ベクトルの正規化
	TurnVec.Normalize();

	//追尾対象へのベクトルに移動ベクトルを乗算する
	TurnVec = TurnVec * enemy.GetMoveV();

	//座標を移動させる
	Vector3 pos = enemy.GetTrans();
	enemy.SetTrans(pos + TurnVec);
}

//	待ち伏せ型ホーミング
void Ambush::homing(Player& player, Enemy& enemy)
{
	//移動ベクトル(速度)
	Vector3 moveV = Vector3(0.07f, 0.07f, 0.07f);

	//追尾対象(プレイヤー)へのベクトル
	Vector3 TurnVec = player.GetTrans() - enemy.GetTrans();

	//	一定距離以内なら追尾を行う
	if (TurnVec.x < 10.0f &&  TurnVec.x > -10.0f && TurnVec.y <= 10.0f
		&& TurnVec.y > -10.0f && TurnVec.z < 10.0f && TurnVec.z > -10.0f)
	{
		//ベクトルの正規化
		TurnVec.Normalize();

		//追尾対象へのベクトルに移動ベクトルを乗算する
		TurnVec = TurnVec * moveV;

		//座標を移動させる
		Vector3 pos = enemy.GetTrans();
		enemy.SetTrans(pos + TurnVec);

		float angle = atan2f(TurnVec.x, TurnVec.z);
		enemy.SetRot(Vector3(0.0f, angle + XM_PI, 0.0f));

	}
}

//	追跡型ホーミング
void Pursuit::homing(Player& player, Enemy& enemy)
{

	//移動ベクトル(速度)
	Vector3 moveV = Vector3(0.07f, 0.07f, 0.07f);
	
	//プレイヤーへの向き
	Vector3 direction = player.GetTrans() - enemy.GetTrans();
	//正規化
	direction.Normalize();

	//	敵の前方向ベクトルとプレイヤーへのベクトルとの内積
	Vector3 front = Vector3(0.0f, 0.0f, -1.0f);
	front = Vector3::TransformNormal(front, enemy.GetWorld());
	float dotvec = front.Dot(direction);

	float angle = dotvec;
	if (dotvec > -1.0f && dotvec < 1.0f)
	{

		angle = acosf(dotvec);
	}
	else
	{
		angle = 0.0f;
	}

	//	radianからdegreeへ
	angle = XMConvertToDegrees(angle);

	//	視点となる範囲
	if (angle < 45.0f && angle > -45.0f)
	{

		//速度ベクトル
		direction = direction * moveV;
		//座標を移動させる
		Vector3 pos = enemy.GetTrans();
		enemy.SetTrans(pos + direction);

		float rot = atan2f(direction.x, direction.z);
		enemy.SetRot(Vector3(0.0f, rot + XM_PI, 0.0f));
	}
}