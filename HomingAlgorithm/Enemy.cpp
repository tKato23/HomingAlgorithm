
//�w�b�_�t�@�C���̃C���N���[�h
#include "Enemy.h"
#include <math.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�R���X�g���N�^
Enemy::Enemy()
{
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
	m_ObjEnemy[BODY].LoadModel(L"Resources/E_body.cmo");
	m_ObjEnemy[L_WING].LoadModel(L"Resources/E_leftwing.cmo");
	m_ObjEnemy[R_WING].LoadModel(L"Resources/E_rightwing.cmo");
	m_ObjEnemy[L_ENGINE].LoadModel(L"Resources/E_engine.cmo");
	m_ObjEnemy[R_ENGINE].LoadModel(L"Resources/E_engine.cmo");

	//�p�[�c��e�q�֌W���Z�b�g
	m_ObjEnemy[L_WING].SetParent(&m_ObjEnemy[BODY]);
	m_ObjEnemy[R_WING].SetParent(&m_ObjEnemy[BODY]);
	m_ObjEnemy[L_ENGINE].SetParent(&m_ObjEnemy[BODY]);
	m_ObjEnemy[R_ENGINE].SetParent(&m_ObjEnemy[BODY]);

	//�e����̃I�t�Z�b�g(���W�̂��炵��)���Z�b�g
	m_ObjEnemy[BODY].SetTranslation(Vector3(0.0f, 2.0f, 0.0f));
	m_ObjEnemy[L_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));
	m_ObjEnemy[R_WING].SetTranslation(Vector3(0.0f, 0.0f, 0.0f));
	m_ObjEnemy[L_ENGINE].SetTranslation(Vector3(-0.1f, 0.0f, 0.5f));
	m_ObjEnemy[L_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));
	m_ObjEnemy[R_ENGINE].SetTranslation(Vector3(0.1f, 0.0f, 0.5f));
	m_ObjEnemy[R_ENGINE].SetScale(Vector3(0.75f, 0.75f, 0.75f));

	{//�G�l�~�[�̓����蔻���ݒ肷��
		m_CollisionNodeEnemy.Initialize();

		m_CollisionNodeEnemy.SetParent(&m_ObjEnemy[BODY]);
		m_CollisionNodeEnemy.SetTrans(Vector3(0, 0, 0));
		m_CollisionNodeEnemy.SetLocalRadius(Vector3(0.8f, 0.8f, 0.8f));
	}

	m_homingFlag = false;

	//	�^�C�v�̔z��
	Homing::Type strategyTypearray[] = {
		Homing::PREFETCH,
		Homing::INTERVAL,
		Homing::AMBUSH,
		Homing::PURSUIT
	};

	//	���ꂼ��̃^�C�v��ǉ�
	for (int i = 0; i < MAX_STRATEGY_NUM; i++)
	{
		addStrategy(strategyTypearray[i]);
	}

	//	�������Ƃ��Đ�ǂ݌^���Z�b�g
	this->SetHomingType(Homing::Type::PREFETCH);
}

//�X�V����
void Enemy::Update()
{
	//���{�b�g�̋����̍X�V
	this->Action();

	//�e�p�[�c�̍X�V
	Calc();

	//�����蔻��̍X�V
	m_CollisionNodeEnemy.Update();

	//�v���C���[�ւ̒ǔ�
	if (m_homingFlag == true)
	{
		this->HomingExecute();
	}

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
	//���G���W���̉�]
	Vector3 l_angle = m_ObjEnemy[L_ENGINE].GetRotation();
	m_ObjEnemy[L_ENGINE].SetRotation(l_angle + Vector3(0, 0, -0.05f));

	//�E�G���W���̉�]
	Vector3 r_angle = m_ObjEnemy[R_ENGINE].GetRotation();
	m_ObjEnemy[R_ENGINE].SetRotation(r_angle + Vector3(0, 0, 0.05f));

	//�ړ��x�N�g��
	m_moveV = Vector3(0.0f, 0.0f, 0.0f);

	//�ړ��ʃx�N�g�������@�̊p�x����]������
	m_moveV = Vector3::TransformNormal(m_moveV, m_ObjEnemy[BODY].GetWorld());
}

//	�z�[�~���O�̃t���O���Z�b�g����
void Enemy::SetHomingFlag(bool flag)
{
	m_homingFlag = flag;
}

//	dictionary�ɒǉ����s���֐�
void Enemy::addStrategy(Homing::Type type)
{

	switch (type)
	{
	case Homing::PREFETCH:
		m_homingDictionary[type] = new Prefetch();
		break;

	case Homing::INTERVAL:
		m_homingDictionary[type] = new Interval();
		break;

	case Homing::AMBUSH:
		m_homingDictionary[type] = new Ambush();
		break;

	case Homing::PURSUIT:
		m_homingDictionary[type] = new Pursuit();
		break;

	default:
		break;
	}
}

//	���s����
void Enemy::HomingExecute()
{
	m_homingDictionary[m_currentType]->homing(*m_Player, *this);
}