#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�R���X�g���N�^
Player::Player(DirectX::Keyboard * keyboard)
{
	m_pKeyboard = keyboard;
	m_weapon_speed = Vector3::Zero;
	m_weapon_flag = false;

	Initialize();
}

//�f�X�g���N�^
Player::~Player()
{

}

//����������
void Player::Initialize()
{
	//���@�p�[�c�̓ǂݍ���
	m_ObjPlayer.resize(PLAYER_PARTS_NUM);
	m_ObjPlayer[BODY].LoadModel(L"Resources/body.cmo");
	m_ObjPlayer[L_WING].LoadModel(L"Resources/LeftWing.cmo");
	m_ObjPlayer[R_WING].LoadModel(L"Resources/RightWing.cmo");
	m_ObjPlayer[L_ENGINE].LoadModel(L"Resources/Engine.cmo");
	m_ObjPlayer[R_ENGINE].LoadModel(L"Resources/Engine.cmo");
	m_ObjPlayer[L_WEAPON].LoadModel(L"Resources/weapon.cmo");
	m_ObjPlayer[R_WEAPON].LoadModel(L"Resources/weapon.cmo");

	//�p�[�c��e�q�֌W���Z�b�g
	m_ObjPlayer[L_WING].SetParent(&m_ObjPlayer[BODY]);
	m_ObjPlayer[R_WING].SetParent(&m_ObjPlayer[BODY]);
	m_ObjPlayer[L_ENGINE].SetParent(&m_ObjPlayer[BODY]);
	m_ObjPlayer[R_ENGINE].SetParent(&m_ObjPlayer[BODY]);
	m_ObjPlayer[L_WEAPON].SetParent(&m_ObjPlayer[L_WING]);
	m_ObjPlayer[R_WEAPON].SetParent(&m_ObjPlayer[R_WING]);

	//�e����̃I�t�Z�b�g(���W�̂��炵��)���Z�b�g
	m_ObjPlayer[BODY].SetTranslation(Vector3(0.0f, 1.0f, 0.0f));
	m_ObjPlayer[L_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));
	m_ObjPlayer[R_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));
	m_ObjPlayer[L_ENGINE].SetTranslation(Vector3(-0.1f, 0.0f, 0.5f));
	m_ObjPlayer[L_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));
	m_ObjPlayer[R_ENGINE].SetTranslation(Vector3(0.1f, 0.0f, 0.5f));
	m_ObjPlayer[R_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));
	m_ObjPlayer[L_WEAPON].SetTranslation(Vector3(-0.5f, -0.05f, -0.025f));
	m_ObjPlayer[L_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));
	m_ObjPlayer[R_WEAPON].SetTranslation(Vector3(0.5f, -0.05f, -0.025f));
	m_ObjPlayer[R_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));


	//�����蔻���ݒ肷��(�T�[�N���Ƃ̔r�ˏ����p)
	m_CollisionNodeBody.Initialize();
	m_CollisionNodeBody.SetParent(&m_ObjPlayer[BODY]);
	m_CollisionNodeBody.SetTrans(Vector3(0, 0, 0.15f));
	m_CollisionNodeBody.SetLocalRadius(Vector3(1.0f, 0.15f, 0.4f));
}

//�X�V����
void Player::Update()
{
	//���{�b�g�̋����̍X�V
	this->Action();

	//�s��X�V
	Calc();
}

//�s��X�V
void Player::Calc()
{
	//�v���C���[�̍X�V����
	for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	{
		it->Update();
	}

	//�����蔻��̍X�V����
	m_CollisionNodeBody.Update();
}

//�`��
void Player::Draw()
{
	//�e�p�[�c�̕`��
	for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	{
		it->Draw();
	}
}

//���{�b�g�̋���
void Player::Action()
{
	//���G���W���̉�]
	Vector3 l_angle = m_ObjPlayer[L_ENGINE].GetRotation();
	m_ObjPlayer[L_ENGINE].SetRotation(l_angle + Vector3(0, 0, -0.05f));

	//�E�G���W���̉�]
	Vector3 r_angle = m_ObjPlayer[R_ENGINE].GetRotation();
	m_ObjPlayer[R_ENGINE].SetRotation(r_angle + Vector3(0, 0, 0.05f));

	// �L�[�{�[�h�̍X�V
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	//���݂̍��W�E��]�p���擾
	Vector3 trans = this->GetTrans();

	//�ړ��x�N�g��
	m_moveV = Vector3(0.0f, 0.0f, 0.0f);

	//�ړ��x�N�g������]����
	m_moveV = Vector3::TransformNormal(m_moveV, m_ObjPlayer[BODY].GetWorld());

	//�ړ�
	trans += m_moveV;

	//�ړ��������W�𔽉f
	this->SetTrans(trans);

	//�O�i
	if (keystate.W || keystate.Up)
	{
		//���݂̍��W�E��]�p���擾
		Vector3 trans = m_ObjPlayer[BODY].GetTranslation();

		//�ړ��x�N�g��(Z���W�̑O�i)
		m_moveV = Vector3(0.0f, 0.0f, -0.1f);

		//�ړ��x�N�g������]����
		m_moveV = Vector3::TransformNormal(m_moveV, m_ObjPlayer[BODY].GetWorld());

		//�ړ�
		trans += m_moveV;

		//�ړ��������W�𔽉f
		m_ObjPlayer[BODY].SetTranslation(trans);
	}

	//���
	if (keystate.S || keystate.Down)
	{
		//���݂̍��W�E��]�p���擾
		Vector3 trans = m_ObjPlayer[BODY].GetTranslation();

		//�ړ��x�N�g��(Z���W�̑O�i)
		SimpleMath::Vector3 moveV(0.0f, 0.0f, 0.1f);

		//�ړ��x�N�g������]����
		moveV = Vector3::TransformNormal(moveV, m_ObjPlayer[BODY].GetWorld());

		//�ړ�
		trans += moveV;

		//�ړ��������W�𔽉f
		m_ObjPlayer[BODY].SetTranslation(trans);
	}

	//������
	if (keystate.A || keystate.Left)
	{
		//���݂̊p�x���擾
		Quaternion rot = m_ObjPlayer[BODY].GetRotationQ();
		Quaternion rotadd = Quaternion::CreateFromAxisAngle(Vector3::UnitY, ROT_SPEED);
		rot = rotadd * rot;

		//��]��̊p�x�𔽉f
		m_ObjPlayer[BODY].SetRotationQ(rot);
	}

	//�E����
	if (keystate.D || keystate.Right)
	{
		//���݂̊p�x���擾
		Quaternion rot = m_ObjPlayer[BODY].GetRotationQ();
		Quaternion rotadd = Quaternion::CreateFromAxisAngle(Vector3::UnitY, -ROT_SPEED);
		rot = rotadd * rot;

		//��]��̊p�x�𔽉f
		m_ObjPlayer[BODY].SetRotationQ(rot);
	}

	//�~�T�C������
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		if (m_weapon_flag)
		{
			ResetWeapon();
		}
		else
		{
			FireWeapon();
		}
	}

	// �e�ۂ�O�i������
	if (m_weapon_flag)
	{
		Vector3 pos_l = m_ObjPlayer[L_WEAPON].GetTranslation();
		Vector3 pos_r = m_ObjPlayer[R_WEAPON].GetTranslation();

		m_ObjPlayer[L_WEAPON].SetTranslation(pos_l + m_weapon_speed);
		m_ObjPlayer[R_WEAPON].SetTranslation(pos_r + m_weapon_speed);
	}
}

//�~�T�C���𔭎˂���֐�
void Player::FireWeapon()
{
	m_weapon_flag = false;

	//���˂���p�[�c�̃��[���h�s����擾
	Matrix worldm_l = m_ObjPlayer[L_WEAPON].GetWorld();
	Matrix worldm_r = m_ObjPlayer[R_WEAPON].GetWorld();

	Vector3 scale_l;		//���[���h�X�P�[�����O(���~�T�C��)
	Vector3 scale_r;		//���[���h�X�P�[�����O(�E�~�T�C��)

	Quaternion rotation_l;	//���[���h��](���~�T�C��)
	Quaternion rotation_r;	//���[���h��](�E�~�T�C��)

	Vector3 translation_l;	//���[���h���W(���~�T�C��)
	Vector3 translation_r;	//���[���h���W(�E�~�T�C��)

	//���[���h�s�񂩂�e�v�f�𒊏o
	worldm_l.Decompose(scale_l, rotation_l, translation_l);
	worldm_r.Decompose(scale_r, rotation_r, translation_r);

	//���˃p�[�c��e���番�����ēƗ�
	m_ObjPlayer[L_WEAPON].SetParent(nullptr);
	m_ObjPlayer[L_WEAPON].SetScale(scale_l);
	m_ObjPlayer[L_WEAPON].SetRotationQ(rotation_l);
	m_ObjPlayer[L_WEAPON].SetTranslation(translation_l);

	m_ObjPlayer[R_WEAPON].SetParent(nullptr);
	m_ObjPlayer[R_WEAPON].SetScale(scale_r);
	m_ObjPlayer[R_WEAPON].SetRotationQ(rotation_r);
	m_ObjPlayer[R_WEAPON].SetTranslation(translation_r);

	//�e�ۂ̑��x��ݒ�
	m_weapon_speed = Vector3(0.0f, 0.0f, -0.1f);
	m_weapon_speed = Vector3::Transform(m_weapon_speed, rotation_l);

	m_weapon_flag = true;
}

//�~�T�C�����đ�������֐�
void Player::ResetWeapon()
{
	if (!m_weapon_flag)
	{
		return;
	}

	//�e�q�֌W�ƃI�t�Z�b�g�������l�ɖ߂�
	m_ObjPlayer[L_WEAPON].SetParent(&m_ObjPlayer[L_WING]);
	m_ObjPlayer[L_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));
	m_ObjPlayer[L_WEAPON].SetRotation(Vector3(0, 0, 0));
	m_ObjPlayer[L_WEAPON].SetTranslation(Vector3(-0.5f, -0.05f, -0.025f));

	m_ObjPlayer[R_WEAPON].SetParent(&m_ObjPlayer[R_WING]);
	m_ObjPlayer[R_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));
	m_ObjPlayer[R_WEAPON].SetRotation(Vector3(0, 0, 0));
	m_ObjPlayer[R_WEAPON].SetTranslation(Vector3(0.5f, -0.05f, -0.025f));

	m_weapon_flag = false;
}