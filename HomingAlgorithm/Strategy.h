//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Strategy.cpp
//!
//! @brief  �X�g���e�W�[�p�^�[����h�t�@�C��
//!
//! @date   2017/09/29
//!
//! @author �����@����
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

//	���d�C���N���[�h�̖h�~
#pragma once

//	�w�b�_�t�@�C���̃C���N���[�h
#include <d3d11.h>
#include <SimpleMath.h>
#include "Player.h"
//#include "Enemy.h"

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

//	��ǂ݌^�z�[�~���O
class Prefetch :public Homing
{
public:
	void homing(Player& player, Enemy& enemy) override;
};

//	�ԍ����m�ی^�z�[�~���O
class Interval :public Homing
{
public:
	void homing(Player& player, Enemy& enemy) override;
};

//	�҂������^�z�[�~���O
class Ambush :public Homing
{
public:
	void homing(Player& player, Enemy& enemy) override;
};

//	�ǐՌ^�z�[�~���O
class Pursuit :public Homing
{
	void homing(Player& player, Enemy& enemy) override;
};