//----------------------------------------------------------
// �J�����𐧌䂷��N���X
//----------------------------------------------------------
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

class Camera
{
public:
	Camera(int width, int height);
	virtual ~Camera();

	//�X�V
	virtual void Update();

	//�r���[�s����擾����֐�
	const DirectX::SimpleMath::Matrix& GetView();

	//�ˉe�s����擾����֐�
	const DirectX::SimpleMath::Matrix& GetProj();

	//���_���Z�b�g
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos);

	//�Q�Ɠ_���Z�b�g
	void SetRefPos(const DirectX::SimpleMath::Vector3& refpos);

	//������x�N�g�����Z�b�g
	void SetUpVec(const DirectX::SimpleMath::Vector3& upvec);

	//������������p���Z�b�g
	void SetFovY(float fovY);

	//���_���Z�b�g
	void SetAspect(float aspect);

	//���_���Z�b�g
	void SetNearClip(float nearclip);

	//���_���Z�b�g
	void SetFarClip(float farclip);

protected:
	//�r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	//�ǂ����猩��̂��H�i���_�j
	DirectX::SimpleMath::Vector3 m_eyepos;

	//�ǂ�������̂��H�i�����_/�Q�Ɠ_�j
	DirectX::SimpleMath::Vector3 m_refpos;

	//�ǂ��炪��ʏ�������i������x�N�g���j
	DirectX::SimpleMath::Vector3 m_upvec;

	//�v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;

	//������������p
	float m_fovY;

	//�A�X�y�N�g��i���E�c�j
	float m_aspect;

	//��O�̕\�����E����
	float m_nearclip;

	//���̕\�����E����
	float m_farclip;
};