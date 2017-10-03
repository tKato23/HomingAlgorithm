//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Strategy.cpp
//!
//! @brief  ストラテジーパターンのhファイル
//!
//! @date   2017/09/29
//!
//! @author 加藤　竜哉
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

//	多重インクルードの防止
#pragma once

//	ヘッダファイルのインクルード
#include <d3d11.h>
#include <SimpleMath.h>
#include "Player.h"

class Enemy;

class Homing
{
public:
	enum Type
	{
		PREFETCH,
		INTERVAL,
		AMBUSH,
		PURSUIT
	};

	virtual void homing(Player& player, Enemy& enemy) = 0;
};

//	先読み型ホーミング
class Prefetch :public Homing
{
public:
	void homing(Player& player, Enemy& enemy) override;
};

//	間合い確保型ホーミング
class Interval :public Homing
{
public:
	void homing(Player& player, Enemy& enemy) override;

private:
	//定数宣言
	static const float INTERVAL_SPACE;
	static const float MOVE_SPEED;
};

//	待ち伏せ型ホーミング
class Ambush :public Homing
{
public:
	void homing(Player& player, Enemy& enemy) override;
};

//	追跡型ホーミング
class Pursuit :public Homing
{
	void homing(Player& player, Enemy& enemy) override;
};