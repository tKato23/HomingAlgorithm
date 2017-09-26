
#include "pch.h"
#include "CollisionNode.h"

using namespace DirectX::SimpleMath;

bool CollisionNode::m_DebugVisible = true;

////////////////////////
//CollisionNode�̃����o�֐�
////////////////////////

//�e���Z�b�g����
void CollisionNode::SetParent(Obj3d * parent)
{
	m_obj.SetParent(parent);
}


////////////////////////
//SphereNode�̃����o�֐�
////////////////////////

//�R���X�g���N�^
SphereNode::SphereNode()
{
	//�f�t�H���g���a��1���[�g���ɂ���
	m_LocalRadius = Vector3(1.0f, 1.0f, 1.0f);
}

//������
void SphereNode::Initialize()
{
	//�f�o�b�O�p���f����ǂݍ���
	m_obj.LoadModel(L"Resources/SphereNode.cmo");
}

//�X�V����
void SphereNode::Update()
{
	m_obj.SetTranslation(m_trans);
	m_obj.SetScale(Vector3(m_LocalRadius));

	//���[���h�s��̍X�V
	m_obj.Update();

	{//���[���h���W�ł̓����蔻�苅���v�Z

		//�v�Z�ς݂̃��[���h�s����擾
		const Matrix& worldm = m_obj.GetWorld();

		//���f�����W�n�ł̒��S�_
		Vector3 center(0, 0, 0);
		
		//���f�����W�n�ł̉E�[
		Vector3 right(1, 0, 0);

		//���[���h���W�ɕϊ�
		center = Vector3::Transform(center, worldm);
		right = Vector3::Transform(right, worldm);

		//�����蔻�苅�Ɍ��ʂ���
		Sphere::Center = center;
		Sphere::Radius = Vector3::Distance(center, right);
	}
}

//�`�揈��
void SphereNode::Draw()
{
	m_obj.Draw();
}