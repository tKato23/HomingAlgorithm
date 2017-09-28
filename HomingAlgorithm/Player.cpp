#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�R���X�g���N�^
Player::Player(DirectX::Keyboard * keyboard)
{
	m_pKeyboard = keyboard;

	m_sinAngle = 0.0f;
	m_speed = 0.0f;

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

	//�p�[�c��e�q�֌W���Z�b�g
	m_ObjPlayer[L_WING].SetParent(&m_ObjPlayer[BODY]);

	m_ObjPlayer[R_WING].SetParent(&m_ObjPlayer[BODY]);

	m_ObjPlayer[L_ENGINE].SetParent(&m_ObjPlayer[BODY]);

	m_ObjPlayer[R_ENGINE].SetParent(&m_ObjPlayer[BODY]);

	//�e����̃I�t�Z�b�g(���W�̂��炵��)���Z�b�g
	m_ObjPlayer[BODY].SetTranslation(Vector3(0.0f, 1.0f, 0.0f));

	m_ObjPlayer[L_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));

	m_ObjPlayer[R_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));

	m_ObjPlayer[L_ENGINE].SetTranslation(Vector3(-0.1f, 0.0f, 0.5f));

	m_ObjPlayer[L_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));

	m_ObjPlayer[R_ENGINE].SetTranslation(Vector3(0.1f, 0.0f, 0.5f));

	m_ObjPlayer[R_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));

	//�����蔻���ݒ肷��(�T�[�N���Ƃ̔r�ˏ����p)
	m_CollisionNodeBody.Initialize();
	m_CollisionNodeBody.SetParent(&m_ObjPlayer[BODY]);
	m_CollisionNodeBody.SetTrans(Vector3(0, 0, 0.15f));
	m_CollisionNodeBody.SetLocalRadius(Vector3(1.0f, 0.15f, 0.4f));

	//�����蔻���ݒ肷��(�T�[�N���̒��S�Ƃ̔���p)
	m_CollisionNodeHit.Initialize();
	m_CollisionNodeHit.SetParent(&m_ObjPlayer[L_ENGINE]);
	m_CollisionNodeHit.SetTrans(Vector3(0, 0, 0.0f));
	m_CollisionNodeHit.SetLocalRadius(Vector3(0.1f, 0.1f, 0.1f));
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
	m_CollisionNodeHit.Update();
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
	Vector3 trans = m_ObjPlayer[BODY].GetTranslation();

	//�ړ��x�N�g��(Z���W�̑O�i)
	m_moveV = Vector3(0.0f, 0.0f, 0.0f);

	//�ړ��x�N�g������]����
	m_moveV = Vector3::TransformNormal(m_moveV, m_ObjPlayer[BODY].GetWorld());

	//�ړ�
	trans += m_moveV;

	//�ړ��������W�𔽉f
	m_ObjPlayer[BODY].SetTranslation(trans);

	//W�L�[��������
	if (keystate.W)
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

		////���݂̊p�x���擾
		//Quaternion rot = m_ObjPlayer[BODY].GetRotationQ();
		//Quaternion rotadd = Quaternion::CreateFromAxisAngle(Vector3::UnitX, ROT_SPEED);
		//rot = rotadd * rot;

		////��]��̊p�x�𔽉f
		//m_ObjPlayer[BODY].SetRotationQ(rot);
	}

	//S�L�[��������
	if (keystate.S)
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

		////���݂̊p�x���擾
		//Quaternion rot = m_ObjPlayer[BODY].GetRotationQ();
		//Quaternion rotadd = Quaternion::CreateFromAxisAngle(Vector3::UnitX, -ROT_SPEED);
		//rot = rotadd * rot;

		////��]��̊p�x�𔽉f
		//m_ObjPlayer[BODY].SetRotationQ(rot);
	}

	//A�L�[��������
	if (keystate.A)
	{
		//���݂̊p�x���擾
		Quaternion rot = m_ObjPlayer[BODY].GetRotationQ();
		Quaternion rotadd = Quaternion::CreateFromAxisAngle(Vector3::UnitY, ROT_SPEED);
		rot = rotadd * rot;

		//��]��̊p�x�𔽉f
		m_ObjPlayer[BODY].SetRotationQ(rot);
	}

	//D�L�[��������
	if (keystate.D)
	{
		//���݂̊p�x���擾
		Quaternion rot = m_ObjPlayer[BODY].GetRotationQ();
		Quaternion rotadd = Quaternion::CreateFromAxisAngle(Vector3::UnitY, -ROT_SPEED);
		rot = rotadd * rot;

		//��]��̊p�x�𔽉f
		m_ObjPlayer[BODY].SetRotationQ(rot);
	}

	//�X�y�[�X�L�[�������Ɖ�������
	if (keystate.Space)
	{
		m_speed = -0.4f;
	}
	else
	{
		m_speed = -0.15f;
	}
}

//�v���C���[�̈ړ����擾����
const DirectX::SimpleMath::Vector3& Player::GetRot()
{
	return m_ObjPlayer[BODY].GetRotation();
}

//�v���C���[�̊p�x���擾����
const DirectX::SimpleMath::Vector3& Player::GetTrans()
{
	return m_ObjPlayer[BODY].GetTranslation();
}

//�v���C���[�̃��[���h�s����擾����
const DirectX::SimpleMath::Matrix & Player::GetWor()
{
	return m_ObjPlayer[BODY].GetWorld();
}

//�v���C���[�̈ړ��x�N�g�����擾����
const DirectX::SimpleMath::Vector3& Player::GetMoveV()
{
	return m_moveV;
}

//�v���C���[�̈ړ����Z�b�g����
void Player::SetRot(const Vector3& rotation)
{
	m_ObjPlayer[BODY].SetRotation(rotation);
}

//�v���C���[�̊p�x���Z�b�g����
void Player::SetTrans(const Vector3& translation)
{
	m_ObjPlayer[BODY].SetTranslation(translation);
}