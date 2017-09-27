#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>
#include "Obj3d.h"
#include "CollisionNode.h"
#include "Player.h"

class Enemy
{
public:
	Enemy();

	~Enemy();
	//����������
	void Initialize();

	//�X�V����
	void Update();

	//�s��̌v�Z
	void Calc();

	//�`�揈��
	void Draw();

	//�G�l�~�[�̋���
	void Action();

	//�G�l�~�[�̊p�x���擾����
	const DirectX::SimpleMath::Vector3& GetRot();

	//�G�l�~�[�̈ʒu���擾����
	const DirectX::SimpleMath::Vector3& GetTrans();

	//�G�l�~�[�̈ړ��x�N�g�����擾����
	const DirectX::SimpleMath::Vector3& GetMoveV();

	//�G�l�~�[�̊p�x���Z�b�g����
	void SetRot(const DirectX::SimpleMath::Vector3& rotation);

	//�G�l�~�[�̈ʒu���Z�b�g����
	void SetTrans(const DirectX::SimpleMath::Vector3& translation);

	//�G�l�~�[�̓����蔻����擾����
	const SphereNode& GetCollisionNodeEnemy() { return m_CollisionNodeEnemy; }

	//��ǂ݌^�̎����ǔ�
	void PrefetchHoming();

	//����^�̎����ǔ�
	void TurnHoming(DirectX::SimpleMath::Vector3 targetPos);

	void UpdateBresenham(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 targetPos);

	void SetPlayer(Player* player) { m_Player = player; }				//�v���C���[���Z�b�g

private:
	//���@��3D�I�u�W�F�N�g
	std::vector<Obj3d> m_ObjEnemy;

	//	�v���C���[
	Player* m_Player;

	//	�O�t���[���̃^�[�Q�b�g���W
	static const int STEP_MAX = 100;
	DirectX::SimpleMath::Vector3 m_nextPos[STEP_MAX];
	DirectX::SimpleMath::Vector3 m_oldTargetPos;
	std::vector<DirectX::SimpleMath::Vector3> m_movePos;
	int m_stepCnt;

	// �T�C���p�̈����ƂȂ�p�x
	float m_sinAngle;

	//�^�C�}�[
	int m_Timer;

	//�ړ��x�N�g��
	DirectX::SimpleMath::Vector3 m_moveV;

	//�ڕW�p�x
	float m_DistAngle;

	//�~�T�C���U�����Ǘ�����t���O
	bool m_weapon_flag;
	bool m_a_flag;

	//���@�p�[�c
	enum ENEMYPARTS
	{
		BODY,
		L_WING,
		R_WING,
		L_ENGINE,
		R_ENGINE,
		L_WEAPON,
		R_WEAPON,

		ENEMY_PARTS_NUM
	};

	//�G�l�~�[�̓����蔻��
	SphereNode m_CollisionNodeEnemy;
};