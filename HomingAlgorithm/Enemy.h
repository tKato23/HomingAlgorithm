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
	const DirectX::SimpleMath::Vector3& GetRot() { return m_ObjEnemy[BODY].GetRotation(); };

	//�G�l�~�[�̃N�I�[�^�j�I�����擾����
	const DirectX::SimpleMath::Quaternion& GetRotQ() { return m_ObjEnemy[BODY].GetRotationQ(); };

	//�G�l�~�[�̈ʒu���擾����
	const DirectX::SimpleMath::Vector3& GetTrans() { return m_ObjEnemy[BODY].GetTranslation(); }

	//�G�l�~�[�̈ړ��x�N�g�����擾����
	const DirectX::SimpleMath::Vector3& GetMoveV() { return m_moveV; }

	//�G�l�~�[�̓����蔻����擾����
	const SphereNode& GetCollisionNodeEnemy() { return m_CollisionNodeEnemy; }

	//�G�l�~�[�̊p�x���Z�b�g����
	void SetRot(const DirectX::SimpleMath::Vector3& rotation) { m_ObjEnemy[BODY].SetRotation(rotation); }

	//�G�l�~�[�̊p�x���Z�b�g����
	void SetRotQ(const DirectX::SimpleMath::Quaternion& quaternion) { m_ObjEnemy[BODY].SetRotationQ(quaternion); }

	//�G�l�~�[�̊p�x���Z�b�g����(�N�H�[�^�j�I��)
	void SetRotQ(const DirectX::SimpleMath::Quaternion& rotationQ);

	//�G�l�~�[�̈ʒu���Z�b�g����
	void SetTrans(const DirectX::SimpleMath::Vector3& translation) { m_ObjEnemy[BODY].SetTranslation(translation); }

	void SetPlayer(Player* player) { m_Player = player; }

	//��ǂ݌^�̎����ǔ�
	void PrefetchHoming();

	//�ԍ����m�ی^�̎����ǔ�
	void IntervalHoming();

	//	�҂������^�̎����ǔ�
	void AmbushHoming();

private:
	//�萔�錾
	const float MOVE_SPEED = 0.07f;		//�@�̂̉�]���x

	//���@��3D�I�u�W�F�N�g
	std::vector<Obj3d> m_ObjEnemy;


	//	�v���C���[
	Player* m_Player;

	//	�萔
	const float MOVE_SPEED = -0.01f;

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