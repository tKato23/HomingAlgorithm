#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�R���X�g���N�^
Enemy::Enemy()
{
	m_sinAngle = 0.0f;
	m_Player = nullptr;

	Initialize();
}

//�f�X�g���N�^
Enemy::~Enemy()
{

}

//����������
void Enemy::Initialize()
{
	//���@�p�[�c�̓ǂݍ���
	m_ObjEnemy.resize(ENEMY_PARTS_NUM);
	m_ObjEnemy[BODY].LoadModel(L"Resources/body.cmo");
	m_ObjEnemy[L_WING].LoadModel(L"Resources/LeftWing.cmo");
	m_ObjEnemy[R_WING].LoadModel(L"Resources/RightWing.cmo");
	m_ObjEnemy[L_ENGINE].LoadModel(L"Resources/Engine.cmo");
	m_ObjEnemy[R_ENGINE].LoadModel(L"Resources/Engine.cmo");
	m_ObjEnemy[L_WEAPON].LoadModel(L"Resources/weapon.cmo");
	m_ObjEnemy[R_WEAPON].LoadModel(L"Resources/weapon.cmo");

	//�p�[�c��e�q�֌W���Z�b�g
	m_ObjEnemy[L_WING].SetParent(&m_ObjEnemy[BODY]);
	m_ObjEnemy[R_WING].SetParent(&m_ObjEnemy[BODY]);
	m_ObjEnemy[L_ENGINE].SetParent(&m_ObjEnemy[BODY]);
	m_ObjEnemy[R_ENGINE].SetParent(&m_ObjEnemy[BODY]);
	m_ObjEnemy[L_WEAPON].SetParent(&m_ObjEnemy[L_WING]);
	m_ObjEnemy[R_WEAPON].SetParent(&m_ObjEnemy[R_WING]);

	//�e����̃I�t�Z�b�g(���W�̂��炵��)���Z�b�g
	m_ObjEnemy[BODY].SetTranslation(Vector3(0.0f, 2.0f, 0.0f));
	m_ObjEnemy[L_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));
	m_ObjEnemy[R_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));
	m_ObjEnemy[L_ENGINE].SetTranslation(Vector3(-0.1f, 0.0f, 0.5f));
	m_ObjEnemy[L_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));
	m_ObjEnemy[R_ENGINE].SetTranslation(Vector3(0.1f, 0.0f, 0.5f));
	m_ObjEnemy[R_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));
	m_ObjEnemy[L_WEAPON].SetTranslation(Vector3(-0.5f, -0.05f, -0.025f));
	m_ObjEnemy[L_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));
	m_ObjEnemy[R_WEAPON].SetTranslation(Vector3(0.5f, -0.05f, -0.025f));
	m_ObjEnemy[R_WEAPON].SetScale(Vector3(0.75f, 0.75f, 0.6f));

	//�^�C�}�[�̏�����
	m_Timer = 0;

	//�ڕW�p�x�̏�����
	m_DistAngle = 0.0f;

	//�e�ۗp�̓����蔻���ݒ肷��
	m_CollisionNodeEnemy.Initialize();
	m_CollisionNodeEnemy.SetParent(&m_ObjEnemy[BODY]);
	m_CollisionNodeEnemy.SetTrans(Vector3(0, 0, 0));
	m_CollisionNodeEnemy.SetLocalRadius(Vector3(0.8f, 0.8f, 0.8f));
}

//�X�V����
void Enemy::Update()
{
	//���{�b�g�̋����̍X�V
	this->Action();


	//�����蔻��̍X�V
	m_CollisionNodeEnemy.Update();

	IntervalHoming();

	//�e�p�[�c�̍X�V
	Calc();
}

//�s��X�V
void Enemy::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_ObjEnemy.begin(); it != m_ObjEnemy.end(); it++)
	{
		it->Update();
	}
}

//�`��
void Enemy::Draw()
{
	//�e�p�[�c�̕`��
	for (std::vector<Obj3d>::iterator it = m_ObjEnemy.begin(); it != m_ObjEnemy.end(); it++)
	{
		it->Draw();
	}

	//�����蔻��̕`��
	//m_CollisionNodeEnemy.Draw();
}

//���{�b�g�̋���
void Enemy::Action()
{
	//���@�p�[�c�̃M�~�b�N
	{
		//���G���W���̉�]
		Vector3 l_angle = m_ObjEnemy[L_ENGINE].GetRotation();
		m_ObjEnemy[L_ENGINE].SetRotation(l_angle + Vector3(0, 0, -0.05f));

		//�E�G���W���̉�]
		Vector3 r_angle = m_ObjEnemy[R_ENGINE].GetRotation();
		m_ObjEnemy[R_ENGINE].SetRotation(r_angle + Vector3(0, 0, 0.05f));

		if (m_weapon_flag == true)
		{
			if (m_ObjEnemy[L_WEAPON].GetParent() != nullptr && m_ObjEnemy[R_WEAPON].GetParent() != nullptr)
			{
				Vector3 a =
					m_ObjEnemy[L_WEAPON].GetParent()->GetParent()->GetTranslation()
					+ m_ObjEnemy[L_WEAPON].GetParent()->GetTranslation()
					+ m_ObjEnemy[L_WEAPON].GetTranslation();

				Vector3 b =
					m_ObjEnemy[R_WEAPON].GetParent()->GetParent()->GetTranslation()
					+ m_ObjEnemy[R_WEAPON].GetParent()->GetTranslation()
					+ m_ObjEnemy[R_WEAPON].GetTranslation();

				Vector3 l_angle =
					m_ObjEnemy[L_WEAPON].GetParent()->GetParent()->GetRotation()
					+ m_ObjEnemy[L_WEAPON].GetParent()->GetRotation()
					+ m_ObjEnemy[L_WEAPON].GetRotation();

				Vector3 r_angle =
					m_ObjEnemy[R_WEAPON].GetParent()->GetParent()->GetRotation()
					+ m_ObjEnemy[R_WEAPON].GetParent()->GetRotation()
					+ m_ObjEnemy[R_WEAPON].GetRotation();

				m_ObjEnemy[L_WEAPON].SetRotation(l_angle);
				m_ObjEnemy[R_WEAPON].SetRotation(r_angle);

				m_a_flag = true;

				m_ObjEnemy[L_WEAPON].SetTranslation(a += Vector3(0, 0, -0.075f));
				m_ObjEnemy[R_WEAPON].SetTranslation(b += Vector3(0, 0, -0.075f));

				m_ObjEnemy[L_WEAPON].SetParent(nullptr);
				m_ObjEnemy[R_WEAPON].SetParent(nullptr);
			}
		}
	}

	//float angle = m_ObjEnemy[BODY].GetRotation().y;

	//�ړ��ʃx�N�g�������@�̊p�x����]������
	//m_moveV = Vector3::TransformNormal(m_moveV, m_ObjEnemy[BODY].GetWorld());


	//�ړ��x�N�g��(���x)
	m_moveV = Vector3(0.07f, 0.07f, 0.07f);

}

//	��ǂ݌^�̎����ǔ�
void Enemy::PrefetchHoming(Vector3 targetPos)
{

}

//�ԍ����m�ی^�̎����ǔ�
void Enemy::IntervalHoming()
{
	//�ǔ��Ώ�(�v���C���[)�ւ̃x�N�g��
	Vector3 TurnVec = m_Player->GetTrans() - this->GetTrans();

	//�x�N�g���̐��K��
	TurnVec.Normalize();

	//�ǔ��Ώۂւ̃x�N�g���Ɉړ��x�N�g������Z����
	TurnVec = TurnVec * m_moveV;

	//���W���ړ�������
	Vector3 pos = this->GetTrans();
	this->SetTrans(pos + TurnVec);


	//�ڕW�p�x�Ɍ������āA�@�̂̊p�x���Ԃ���

	//�G�̊p�x����]������
	Vector3 rotv = m_ObjEnemy[BODY].GetRotation();

	float angle = m_DistAngle - rotv.y;

	//180�x�𒴂���ꍇ�A�t���ɂ���
	if (angle > XM_PI)
	{
		angle -= XM_2PI;
	}

	if (angle < -XM_PI)
	{
		angle += XM_2PI;
	}

	//���
	rotv.y += angle * 0.01f;

	rotv.y = 90.0f;

	this->SetRot(rotv);

	//m_ObjEnemy[BODY].SetRotation(rotv);
}