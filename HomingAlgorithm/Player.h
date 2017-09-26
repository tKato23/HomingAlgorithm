#pragma once

#include <d3d11.h>
#include <Keyboard.h>
#include "CollisionNode.h"

class Player
{
public:
	//�萔�錾
	const float ROT_SPEED = 0.05f;				//�@�̂̉�]���x

	//���@�p�[�c
	enum PLAYERPARTS
	{
		BODY,
		L_WING,
		R_WING,
		L_ENGINE,
		R_ENGINE,

		PLAYER_PARTS_NUM
	};

	//�R���X�g���N�^
	Player(DirectX::Keyboard* keyboard);

	//�f�X�g���N�^
	~Player();

	//����������
	void Initialize();

	//�X�V����
	void Update();

	// �s��̌v�Z
	void Calc();

	//�`�揈��
	void Draw();

	//�v���C���[�̋���
	void Action();

	//�v���C���[�̈ړ����擾����
	const DirectX::SimpleMath::Vector3& GetRot();

	//�v���C���[�̊p�x���擾����
	const DirectX::SimpleMath::Vector3& GetTrans();

	//�v���C���[�̃��[���h�s����擾����
	const DirectX::SimpleMath::Matrix& GetWor();

	//�v���C���[�̈ړ����Z�b�g����
	void SetRot(const DirectX::SimpleMath::Vector3& rotation);

	//�v���C���[�̊p�x���Z�b�g����
	void SetTrans(const DirectX::SimpleMath::Vector3& translation);

	//�v���C���[�̓����蔻��(�r�ˏ����p)���擾����
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

	//�v���C���[�̓����蔻��(�����������̔���p)���擾����
	const SphereNode& GetCollisionNodeHit() { return m_CollisionNodeHit; }

	//�v���C���[�̑��x���擾����
	const DirectX::SimpleMath::Vector3 GetVelocity() { return m_Velocity; }

private:
	//���@��3D�I�u�W�F�N�g
	std::vector<Obj3d> m_ObjPlayer;

	// �L�[�{�[�h
	DirectX::Keyboard* m_pKeyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	// �T�C���p�̈����ƂȂ�p�x
	float m_sinAngle;

	//�S�g�̓����蔻��(�r�ˏ����p)
	SphereNode m_CollisionNodeBody;

	//�S�g�̓����蔻��(�����������̔���p)
	SphereNode m_CollisionNodeHit;

	//���x
	DirectX::SimpleMath::Vector3 m_Velocity;

	//�O�i���x
	float m_speed;
};