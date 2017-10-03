
//���d�C���N���[�h�̖h�~
#pragma once

//�w�b�_�t�@�C���̃C���N���[�h
#include <d3d11.h>
#include <Keyboard.h>
#include "CollisionNode.h"

class Player
{
public:
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

	//�v���C���[�̊p�x���擾����
	const DirectX::SimpleMath::Vector3& GetRot() { return m_ObjPlayer[BODY].GetRotation(); }

	//�v���C���[�̃N�I�[�^�j�I�����擾����
	const DirectX::SimpleMath::Quaternion& GetRotQ() { return m_ObjPlayer[BODY].GetRotationQ(); }

	//�v���C���[�̈ʒu���擾����
	const DirectX::SimpleMath::Vector3& GetTrans() { return m_ObjPlayer[BODY].GetTranslation(); }

	//�v���C���[�̃��[���h�s����擾����
	const DirectX::SimpleMath::Matrix& GetWor() { return m_ObjPlayer[BODY].GetWorld(); }

	//�v���C���[�̈ړ��x�N�g�����擾����
	const DirectX::SimpleMath::Vector3& GetMoveV() { return m_moveV; }
	
	//�v���C���[�̓����蔻��(�r�ˏ����p)���擾����
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

	//�v���C���[�̊p�x���Z�b�g����
	void SetRot(const DirectX::SimpleMath::Vector3& rotation) { m_ObjPlayer[BODY].SetRotation(rotation); }

	//�v���C���[�̈ʒu���Z�b�g����
	void SetTrans(const DirectX::SimpleMath::Vector3& translation){ m_ObjPlayer[BODY].SetTranslation(translation); }

private:
	//�萔�錾
	const float ROT_SPEED = 0.05f;	//�@�̂̉�]���x

	//���@��3D�I�u�W�F�N�g
	std::vector<Obj3d> m_ObjPlayer;

	// �L�[�{�[�h
	DirectX::Keyboard* m_pKeyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	//�v���C���[�̈ړ��x�N�g��
	DirectX::SimpleMath::Vector3 m_moveV;

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

	//�S�g�̓����蔻��(�r�ˏ����p)
	SphereNode m_CollisionNodeBody;
};