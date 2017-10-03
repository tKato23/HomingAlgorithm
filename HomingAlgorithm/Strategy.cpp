//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Strategy.cpp
//!
//! @brief  �X�g���e�W�[�p�^�[����cpp�t�@�C��
//!
//! @date   2017/09/29
//!
//! @author �����@����
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

//	�w�b�_�t�@�C���̃C���N���[�h
#include "Strategy.h"
#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//	��ǂ݌^�z�[�~���O
void Prefetch::homing(Player& player, Enemy& enemy)
{
	Vector3 moveV = Vector3(0.02f, 0.02f, 0.02f);

	//	���΋���
	Vector3 dis;
	dis.x = player.GetTrans().x - enemy.GetTrans().x;
	dis.y = player.GetTrans().y - enemy.GetTrans().y;
	dis.z = player.GetTrans().z - enemy.GetTrans().z;

	//	���Α��x
	Vector3 vel;
	vel.x = player.GetMoveV().x - moveV.x;
	vel.y = player.GetMoveV().y - moveV.y;
	vel.z = player.GetMoveV().z - moveV.z;

	//	�ڋߎ��ԁ@
	float time = 0.0f;
	float distance = sqrtf(dis.x * dis.x + dis.y * dis.y + dis.z * dis.z);
	float velocity = sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);

	time = distance / velocity;
	time = fabsf(time);

	//	�\���n�_
	Vector3 pos;
	pos.x = player.GetTrans().x + (player.GetMoveV().x * time);
	pos.y = player.GetTrans().y + (player.GetMoveV().y * time);
	pos.z = player.GetTrans().z + (player.GetMoveV().z * time);

	//	�x�N�g��
	pos = pos - enemy.GetTrans();

	//	�v���C���[�̕����ւ̈ړ��x�N�g���̉�]�H
	//Vector3 EnemyRot = Vector3(0.0f, pos.y, 0.0f);
	//enemy.SetRot(EnemyRot);

	pos = pos * moveV;

	//	�G�ɔ��f������
	enemy.SetTrans(enemy.GetTrans() + pos);
}

//	�ԍ����m�ی^�z�[�~���O
void Interval::homing(Player& player, Enemy& enemy)
{
	//�ǔ��Ώ�(�v���C���[)�ւ̃x�N�g��
	Vector3 TurnVec = player.GetTrans() - enemy.GetTrans();

	//�x�N�g���̐��K��
	TurnVec.Normalize();

	//�ǔ��Ώۂւ̃x�N�g���Ɉړ��x�N�g������Z����
	TurnVec = TurnVec * enemy.GetMoveV();

	//���W���ړ�������
	Vector3 pos = enemy.GetTrans();
	enemy.SetTrans(pos + TurnVec);
}

//	�҂������^�z�[�~���O
void Ambush::homing(Player& player, Enemy& enemy)
{
	//�ړ��x�N�g��(���x)
	Vector3 moveV = Vector3(0.07f, 0.07f, 0.07f);

	//�ǔ��Ώ�(�v���C���[)�ւ̃x�N�g��
	Vector3 TurnVec = player.GetTrans() - enemy.GetTrans();

	//	��苗���ȓ��Ȃ�ǔ����s��
	if (TurnVec.x < 10.0f &&  TurnVec.x > -10.0f && TurnVec.y <= 10.0f
		&& TurnVec.y > -10.0f && TurnVec.z < 10.0f && TurnVec.z > -10.0f)
	{
		//�x�N�g���̐��K��
		TurnVec.Normalize();

		//�ǔ��Ώۂւ̃x�N�g���Ɉړ��x�N�g������Z����
		TurnVec = TurnVec * moveV;

		//���W���ړ�������
		Vector3 pos = enemy.GetTrans();
		enemy.SetTrans(pos + TurnVec);

		//	�v���C���[�̕����������悤�ɉ�]
		float angle = atan2f(TurnVec.x, TurnVec.z);
		enemy.SetRot(Vector3(0.0f, angle + XM_PI, 0.0f));

	}
}

//	�ǐՌ^�z�[�~���O
void Pursuit::homing(Player& player, Enemy& enemy)
{

	//�ړ��x�N�g��(���x)
	Vector3 moveV = Vector3(0.07f, 0.07f, 0.07f);
	
	//�v���C���[�ւ̌���
	Vector3 direction = player.GetTrans() - enemy.GetTrans();
	//���K��
	direction.Normalize();

	//	�G�̑O�����x�N�g���ƃv���C���[�ւ̃x�N�g���Ƃ̓���
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

	//	radian����degree��
	angle = XMConvertToDegrees(angle);

	//	���_�ƂȂ�͈�
	if (angle < 45.0f && angle > -45.0f)
	{

		//���x�x�N�g��
		direction = direction * moveV;
		//���W���ړ�������
		Vector3 pos = enemy.GetTrans();
		enemy.SetTrans(pos + direction);

		//	�v���C���[�̕����ɉ�]
		float rot = atan2f(direction.x, direction.z);
		enemy.SetRot(Vector3(0.0f, rot + XM_PI, 0.0f));
	}
}