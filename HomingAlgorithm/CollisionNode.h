#pragma once
#include "Collision.h"
#include "Obj3d.h"

//������m�[�h
class CollisionNode
{
protected:
	//�f�o�b�O�\��ON
	static bool m_DebugVisible;

public:
	static void SetDebugVisible(bool flag) { m_DebugVisible = flag; }
	static bool GetDebugVisible() { return m_DebugVisible; }

public:
	//������
	virtual void Initialize() = 0;

	//�X�V����
	virtual void Update() = 0;
	
	//�`�揈��
	virtual void Draw() = 0;

	//�e���Z�b�g����
	void SetParent(Obj3d* parent);

	//�e����̃I�t�Z�b�g���Z�b�g
	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_trans = trans; }

protected:
	//�f�o�b�O�\���p�I�u�W�F�N�g
	Obj3d m_obj;

	//�e����̃I�t�Z�b�g
	DirectX::SimpleMath::Vector3 m_trans;
};

//���̓�����m�[�h
class SphereNode : public CollisionNode, public Sphere
{
public:
	//�R���X�g���N�^
	SphereNode();

	//������
	void Initialize();

	//�X�V����
	void Update();

	//�`�揈��
	void Draw();

	//���[�J�����a���Z�b�g
	//void SetLocalRadius(float radius) { m_LocalRadius = radius; }
	void SetLocalRadius(const DirectX::SimpleMath::Vector3 radius) { m_LocalRadius = radius; }

protected:

	//���[�J�����a
	//float m_LocalRadius;
	DirectX::SimpleMath::Vector3 m_LocalRadius;
};