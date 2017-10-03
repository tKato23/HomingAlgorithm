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

//	�ÓI�����o�萔�̒�`
const float Interval::INTERVAL_SPACE = 2.5f;
const float Interval::MOVE_SPEED = 0.07f;

//	��ǂ݌^�z�[�~���O
void Prefetch::homing(Player& player, Enemy& enemy)
{
	//�@�ړ��x�N�g��
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

	float angle_Y = atan2f(pos.x, pos.z) + XM_PI;
	enemy.SetRot(Vector3(0.0f, angle_Y, 0.0f));

	//	�G�ɔ��f������
	enemy.SetTrans(enemy.GetTrans() + pos);
}

//�ԍ����m�ی^�z�[�~���O
void Interval::homing(Player& player, Enemy& enemy)
{
	//�ǔ��Ώ�(�v���C���[)�ւ̃x�N�g��
	Vector3 Vec = player.GetTrans() - enemy.GetTrans();

	float distance_Square;

	//�O�����̒藝�ŁA�x�N�g���̒������v�Z����
	distance_Square = Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z;

	float radius_Square;

	//���a�̘a�̓��
	radius_Square = INTERVAL_SPACE + INTERVAL_SPACE;
	radius_Square = radius_Square * radius_Square;

	//���������a�̘a���傫����ΑO�i����
	if (distance_Square > radius_Square)
	{
		//�x�N�g���̐��K��
		Vec.Normalize();

		//�ړ��x�N�g��
		Vector3 moveV = Vector3(MOVE_SPEED, MOVE_SPEED, MOVE_SPEED);

		//�ǔ��Ώۂւ̃x�N�g���Ɉړ��x�N�g������Z����
		Vec = Vec * moveV;

		//�p�x�̐ݒ�
		float angle_Y = atan2f(Vec.x, Vec.z) + XM_PI;
		enemy.SetRot(Vector3(0.0f, angle_Y, 0.0f));

		//���W���ړ�������
		Vector3 pos = enemy.GetTrans();
		enemy.SetTrans(pos + Vec);
	}
	//���������a�̘a��菬������Ό�ނ���
	else if (distance_Square + 1.0f < radius_Square)
	{
		//�x�N�g���̐��K��
		Vec.Normalize();

		//�ړ��x�N�g��
		Vector3 moveV = Vector3(MOVE_SPEED, MOVE_SPEED, MOVE_SPEED);

		//�ǔ��Ώۂւ̃x�N�g���Ɉړ��x�N�g������Z����
		Vec = Vec * moveV;

		//�p�x�̐ݒ�
		float angle_Y = atan2f(Vec.x, Vec.z) + XM_PI;
		enemy.SetRot(Vector3(0.0f, angle_Y, 0.0f));

		//���W���ړ�������
		Vector3 pos = enemy.GetTrans();
		enemy.SetTrans(pos - Vec);
	}
	//�O�i�A��ނ̌J��Ԃ��������
	else if (distance_Square >= radius_Square || distance_Square <= radius_Square)
	{
		//�ړ��x�N�g��
		Vector3 moveV = Vector3(0.0f, 0.0f, 0.0f);
	}
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

		float rot = atan2f(direction.x, direction.z);
		enemy.SetRot(Vector3(0.0f, rot + XM_PI, 0.0f));
	}
}