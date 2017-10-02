#include "Enemy.h"
#include <math.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//	�ÓI�����o�萔�̒�`
const float Enemy::MOVE_SPEED = -0.02f;

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

	{//�e�ۗp�̓����蔻���ݒ肷��
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
	this->SetHomingType(Homing::Type::PURSUIT);

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

	//	�v���C���[�ւ̒ǔ�
	if (m_homingFlag == true)
	{
		//	��ǂ݌^�̃z�[�~���O
		//PrefetchHoming();

		//�ԍ����m�ی^�̎����ǔ�
		//IntervalHoming();

		//	�҂������^�̎����ǔ�
		//AmbushHoming();

		this->HomingExecute();
	}

	
	//IntervalHoming();

	//�e�p�[�c�̍X�V
	Calc();

	//�����蔻��̍X�V
	m_CollisionNodeEnemy.Update();
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

	}
	//�ړ��ʃx�N�g�������@�̊p�x����]������
	//m_moveV = Vector3::TransformNormal(m_moveV, m_ObjEnemy[BODY].GetWorld());
}

////�ǐՌ^�̎����ǔ�
//void Enemy::PursuitHouming()
//{
//	//�ړ��x�N�g��(���x)
//	m_moveV = Vector3(0.07f, 0.07f, 0.07f);
//
//	//�v���C���[�ւ̌���
//	Vector3 direction = m_Player->GetTrans() - this->GetTrans();
//	//���K��
//	direction.Normalize();
//	//���x�x�N�g��
//	direction = direction*m_moveV;
//	//���W���ړ�������
//	Vector3 pos = this->GetTrans();
//	this->SetTrans(pos + direction);
//	this->SetRot(m_Player->GetRot());
//}
//
////	��ǂ݌^�̎����ǔ�
//void Enemy::PrefetchHoming()
//{
//	//	���΋���
//	Vector3 dis;
//	dis.x = m_Player->GetTrans().x - GetTrans().x;
//	dis.y = m_Player->GetTrans().y - GetTrans().y;
//	dis.z = m_Player->GetTrans().z - GetTrans().z;
//
//	//	���Α��x
//	Vector3 vel;
//	vel.x = m_Player->GetMoveV().x - GetMoveV().x;
//	vel.y = m_Player->GetMoveV().y - GetMoveV().y;
//	vel.z = m_Player->GetMoveV().z - GetMoveV().z;
//
//	//	�ڋߎ��ԁ@
//	float time = 0.0f;
//	float distance = sqrtf(dis.x * dis.x + dis.y * dis.y + dis.z * dis.z);
//	float velocity = sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
//
//	//float angle = m_ObjEnemy[BODY].GetRotation().y;
//
//	//�ړ��ʃx�N�g�������@�̊p�x����]������
//	//m_moveV = Vector3::TransformNormal(m_moveV, m_ObjEnemy[BODY].GetWorld());
//
//	//	�v���C���[�̕����ւ̈ړ��x�N�g���̉�]�H
//	//Vector3 EnemyRot = Vector3(0.0f, pos.y, 0.0f);
//	//SetRot(EnemyRot);
//
//
//	//�ړ��x�N�g��(���x)
//	m_moveV = Vector3(0.07f, 0.07f, 0.07f);
//
//	//	�G�ɔ��f������
//	this->SetTrans(this->GetTrans() - pos);
//
//	pos.Normalize();
//	float angle = atan2f(pos.x, pos.z);
//	SetRot(Vector3(0.0f, angle + XM_2PI, 0.0f));
//
//}
//
//
////�҂������^�̎����ǔ�
//void Enemy::AmbushHoming()
//{
//	//�ǔ��Ώ�(�v���C���[)�ւ̃x�N�g��
//	Vector3 TurnVec = m_Player->GetTrans() - this->GetTrans();
//
//	//	��苗���ȓ��Ȃ�ǔ����s��
//	if (TurnVec.x < 10.0f &&  TurnVec.x > -10.0f && TurnVec.y <= 10.0f 
//		&& TurnVec.y > -10.0f && TurnVec.z < 10.0f && TurnVec.z > -10.0f)
//	{
//		//�x�N�g���̐��K��
//		TurnVec.Normalize();
//
//		//�ǔ��Ώۂւ̃x�N�g���Ɉړ��x�N�g������Z����
//		TurnVec = TurnVec * m_moveV;
//
//		//���W���ړ�������
//		Vector3 pos = this->GetTrans();
//		this->SetTrans(pos + TurnVec);
//
//		float angle = atan2f(TurnVec.x, TurnVec.z);
//		SetRot(Vector3(0.0f, angle + XM_PI, 0.0f));
//
//	}
//}
//
////�ԍ����m�ی^�̎����ǔ�
//void Enemy::IntervalHoming()
//{
//	//�ړ��x�N�g��
//	m_moveV = Vector3(MOVE_SPEED, MOVE_SPEED, MOVE_SPEED);
//
//	//�ǔ��Ώ�(�v���C���[)�ւ̃x�N�g��
//	Vector3 Vec = m_Player->GetTrans() - this->GetTrans();
//
//	float angle_X = atan2f(Vec.z, Vec.y);
//	float angle_Y = atan2f(Vec.x, Vec.z);
//	float angle_Z = atan2f(Vec.y, Vec.x);
//
//	this->SetRot(Vector3(angle_X + XM_PI, angle_Y + XM_PI, 0.0f));
//	//this->SetRot(Vector3(0.0f, angle_Y, 0.0f));
//
//	//�x�N�g���̐��K��
//	Vec.Normalize();
//
//	//�ǔ��Ώۂւ̃x�N�g���Ɉړ��x�N�g������Z����
//	Vec = Vec * m_moveV;
//
//	//���W���ړ�������
//	Vector3 pos = this->GetTrans();
//
//	this->SetTrans(pos + TurnVec);
//}

//	�z�[�~���O�̃t���O���Z�b�g����
void Enemy::SetHomingFlag(bool flag)
{
	m_homingFlag = flag;
}

//	dictionary�ɒǉ����s���֐�
void Enemy::addStrategy(Homing::Type type)
{

	switch (type) {
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

