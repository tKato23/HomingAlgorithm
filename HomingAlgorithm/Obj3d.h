
//���d�C���N���[�h�̖h�~
#pragma once

//�w�b�_�t�@�C���̃C���N���[�h
#include <Windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

#include "Camera.h"

class Obj3d
{
public:
	//�ÓI�����o�ϐ��̏�����
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>            d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     d3dContext,
		Camera* camera);

private:
	//�f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;

	//�f�o�C�X�R���e�L�X�g
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;

	//�J����
	static Camera* m_Camera;

	//�ėp�`��X�e�[�g
	static std::unique_ptr<DirectX::CommonStates> m_states;

	//�G�t�F�N�g�t�@�N�g��
	static std::unique_ptr<DirectX::EffectFactory> m_factory;

public:
	//�R���X�g���N�^
	Obj3d();

	//���f�������[�h����
	void LoadModel(const wchar_t* fileName);

	//�X�V
	void Update();

	//�s��̌v�Z
	void Calc();

	//�`��
	void Draw();

	//�I�u�W�F�N�g�̍ŏ��̐F��ݒ肷��
	void Set_Init_Color(const DirectX::SimpleMath::Vector4& color_code);

	//�I�u�W�F�N�g�̐F��ς���
	void Set_Color();

	//�G�t�F�N�g��ݒ肷��
	void Set_Effect();

	//setter
	//�X�P�[�����O(X Y Z)
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }

	//��]�p(X Y Z)
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { m_rotation = rotation; m_UseQuaternion = false; }

	//��]�p(�N�H�[�^�j�I��)
	void SetRotationQ(const DirectX::SimpleMath::Quaternion& rotation) { m_rotationQ = rotation; m_UseQuaternion = true; }

	//���s�ړ�(X Y Z)
	void SetTranslation(const DirectX::SimpleMath::Vector3& translation) { m_translation = translation; }

	//���[���h�s��p
	void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_world = world; }

	//�e��3D�I�u�W�F�N�g�̃|�C���^
	void SetParent(Obj3d* parent) { m_parent = parent; }

	//getter
	//�X�P�[�����O(X Y Z)
	const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }

	//��]�p(X Y Z)
	const DirectX::SimpleMath::Vector3& GetRotation() { return m_rotation; }

	//��]�p(�N�H�[�^�j�I��)
	const DirectX::SimpleMath::Quaternion& GetRotationQ() { return m_rotationQ; }

	//���s�ړ�(X Y Z)
	const DirectX::SimpleMath::Vector3& GetTranslation() { return m_translation; }

	//���[���h�s��
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_world; }

	//�e��3D�I�u�W�F�N�g�̃|�C���^
	Obj3d* GetParent() { return m_parent; }

private:
	//�����o�ϐ�
	//3D���f��

	//���f��
	std::unique_ptr<DirectX::Model> m_model;

	//�X�P�[�����O(X Y Z)
	DirectX::SimpleMath::Vector3	m_scale;

	//��]�p(X Y Z �I�C���[�p)
	DirectX::SimpleMath::Vector3	m_rotation;

	//��]�p(�N�H�[�^�j�I��)
	DirectX::SimpleMath::Quaternion	m_rotationQ;

	//���s�ړ�(X Y Z)
	DirectX::SimpleMath::Vector3	m_translation;

	//���[���h�s��
	DirectX::SimpleMath::Matrix		m_world;

	//�e��3D�I�u�W�F�N�g�̃|�C���^
	Obj3d* m_parent;

	//�N�H�[�^�j�I���g�p�t���O
	bool m_UseQuaternion;

	//�|���S���\���p�G�t�F�N�g
	std::shared_ptr<DirectX::BasicEffect> m_pPolygonEffect;
};