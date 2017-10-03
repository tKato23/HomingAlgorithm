
//���d�C���N���[�h�̖h�~
#pragma once

//�w�b�_�t�@�C���̃C���N���[�h
#include <d3d11.h>
#include <SimpleMath.h>
#include "Obj3d.h"
#include "CollisionNode.h"
#include "Player.h"
#include "Strategy.h"

class Enemy
{
public:
	//�R���X�g���N�^
	Enemy();

	//�f�X�g���N�^
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

	//�G�l�~�[�̃��[���h���W
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_ObjEnemy[BODY].GetWorld(); }

	//�G�l�~�[�̊p�x���Z�b�g����
	void SetRot(const DirectX::SimpleMath::Vector3& rotation) { m_ObjEnemy[BODY].SetRotation(rotation); }

	//�G�l�~�[�̊p�x���Z�b�g����
	void SetRotQ(const DirectX::SimpleMath::Quaternion& quaternion) { m_ObjEnemy[BODY].SetRotationQ(quaternion); }

	//�G�l�~�[�̈ʒu���Z�b�g����
	void SetTrans(const DirectX::SimpleMath::Vector3& translation) { m_ObjEnemy[BODY].SetTranslation(translation); }

	//	�G�l�~�[�̃��[���h�s����Z�b�g����
	void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_ObjEnemy[BODY].SetWorld(world); }

	//�v���C���[���Z�b�g����
	void SetPlayer(Player* player) { m_Player = player; }

	//�z�[�~���O�̃t���O���Z�b�g����
	void SetHomingFlag(bool flag);

	//�^�C�v�̎擾�ƃZ�b�g
	Homing::Type getCurrentType() { return m_currentType; }
	void SetHomingType(Homing::Type type) { m_currentType = type; }
	
	//	�z�[�~���O�����̎擾
	Homing** getHomingDictionary() { return m_homingDictionary; }

	//	�^�C�v�̒ǉ�
	void addStrategy(Homing::Type type);

	//	���s����
	void HomingExecute();

private:
	//�萔�錾
	static const int MAX_STRATEGY_NUM = 4;

	//���@��3D�I�u�W�F�N�g
	std::vector<Obj3d> m_ObjEnemy;

	//�v���C���[
	Player* m_Player;

	//�ړ��x�N�g��
	DirectX::SimpleMath::Vector3 m_moveV;

	//���@�p�[�c
	enum ENEMYPARTS
	{
		BODY,
		L_WING,
		R_WING,
		L_ENGINE,
		R_ENGINE,

		ENEMY_PARTS_NUM
	};

	//�G�l�~�[�̓����蔻��
	SphereNode m_CollisionNodeEnemy;

	//	�z�[�~���O�����s����t���O
	bool m_homingFlag;

	//	�e�z�[�~���O�̏��
	Homing* m_homingDictionary[MAX_STRATEGY_NUM];

	//	���݂̃^�C�v
	Homing::Type m_currentType;
};