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

//	静的メンバ定数の定義
const float Interval::INTERVAL_SPACE = 2.5f;
const float Interval::MOVE_SPEED = 0.07f;

//	先読み型ホーミング
void Prefetch::homing(Player& player, Enemy& enemy)
{
	//　移動ベクトル
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

	float angle_Y = atan2f(pos.x, pos.z) + XM_PI;
	enemy.SetRot(Vector3(0.0f, angle_Y, 0.0f));

	//	敵に反映させる
	enemy.SetTrans(enemy.GetTrans() + pos);
}

//間合い確保型ホーミング
void Interval::homing(Player& player, Enemy& enemy)
{
	//追尾対象(プレイヤー)へのベクトル
	Vector3 Vec = player.GetTrans() - enemy.GetTrans();

	float distance_Square;

	//三平方の定理で、ベクトルの長さを計算する
	distance_Square = Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z;

	float radius_Square;

	//半径の和の二乗
	radius_Square = INTERVAL_SPACE + INTERVAL_SPACE;
	radius_Square = radius_Square * radius_Square;

	//距離が半径の和より大きければ前進する
	if (distance_Square > radius_Square)
	{
		//ベクトルの正規化
		Vec.Normalize();

		//移動ベクトル
		Vector3 moveV = Vector3(MOVE_SPEED, MOVE_SPEED, MOVE_SPEED);

		//追尾対象へのベクトルに移動ベクトルを乗算する
		Vec = Vec * moveV;

		//角度の設定
		float angle_Y = atan2f(Vec.x, Vec.z) + XM_PI;
		enemy.SetRot(Vector3(0.0f, angle_Y, 0.0f));

		//座標を移動させる
		Vector3 pos = enemy.GetTrans();
		enemy.SetTrans(pos + Vec);
	}
	//距離が半径の和より小さければ後退する
	else if (distance_Square + 1.0f < radius_Square)
	{
		//ベクトルの正規化
		Vec.Normalize();

		//移動ベクトル
		Vector3 moveV = Vector3(MOVE_SPEED, MOVE_SPEED, MOVE_SPEED);

		//追尾対象へのベクトルに移動ベクトルを乗算する
		Vec = Vec * moveV;

		//角度の設定
		float angle_Y = atan2f(Vec.x, Vec.z) + XM_PI;
		enemy.SetRot(Vector3(0.0f, angle_Y, 0.0f));

		//座標を移動させる
		Vector3 pos = enemy.GetTrans();
		enemy.SetTrans(pos - Vec);
	}
	//前進、後退の繰り返しを避ける
	else if (distance_Square >= radius_Square || distance_Square <= radius_Square)
	{
		//移動ベクトル
		Vector3 moveV = Vector3(0.0f, 0.0f, 0.0f);
	}
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