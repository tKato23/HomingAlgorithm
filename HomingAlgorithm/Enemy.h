#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>
#include "Obj3d.h"
#include "CollisionNode.h"
#include "Player.h"
#include "Strategy.h"


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

	//�G�l�~�[�̈ʒu���Z�b�g����
	void SetTrans(const DirectX::SimpleMath::Vector3& translation) { m_ObjEnemy[BODY].SetTranslation(translation); }

	void SetPlayer(Player* player) { m_Player = player; }

	//��ǂ݌^�̎����ǔ�
	void PrefetchHoming();

	//�ԍ����m�ی^�̎����ǔ�
	void IntervalHoming();

	//	�҂������^�̎����ǔ�
	void AmbushHoming();

	//	�z�[�~���O�̃t���O���Z�b�g����
	void SetHomingFlag(bool flag);

	//	�^�C�v�̎擾�ƃZ�b�g
	Homing::Type getCurrentType() { return m_currentType; }
	void SetHomingType(Homing::Type type) { m_currentType = type; }

	Homing** getHomingDictionary() { return m_homingDictionary; }
	void addStrategy(Homing::Type type);
	void HomingExecute();

private:

	//���@��3D�I�u�W�F�N�g
	std::vector<Obj3d> m_ObjEnemy;

	//	�v���C���[
	Player* m_Player;

	//	�萔
	static const float MOVE_SPEED;
	static const int MAX_STRATEGY_NUM = 4;

	// �T�C���p�̈����ƂȂ�p�x
	float m_sinAngle;

	//�^�C�}�[
	int m_Timer;

	//�ړ��x�N�g��
	DirectX::SimpleMath::Vector3 m_moveV;

	//�ڕW�p�x
	float m_DistAngle;

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

	//	�z�[�~���O�����s����t���O
	bool m_homingFlag;

	Homing* m_homingDictionary[MAX_STRATEGY_NUM];
	Homing::Type m_currentType;
};