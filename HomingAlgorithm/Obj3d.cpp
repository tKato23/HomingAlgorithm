
//�w�b�_�t�@�C���̃C���N���[�h
#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�ÓI�����o�ϐ��̎���
//�f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;

//�f�o�C�X�R���e�L�X�g
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;

//�J����
Camera* Obj3d::m_Camera;

//�ėp�`��X�e�[�g
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;

//�G�t�F�N�g�t�@�N�g��
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_Camera = camera;

	//�ėp�`��X�e�[�g����
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	//�G�t�F�N�g�t�@�N�g���̐���
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());

	//�e�N�X�`���̓ǂݍ��݃p�X�̎w��
	m_factory->SetDirectory(L"Resources");
}

Obj3d::Obj3d()
{
	//�X�P�[����1�{���f�t�H���g
	m_scale = Vector3(1.0f, 1.0f, 1.0f);

	//�f�t�H���g�ł̓I�C���[�p�Ōv�Z
	m_UseQuaternion = false;

	m_parent = nullptr;
}

void Obj3d::LoadModel(const wchar_t * fileName)
{
	//���f���̓ǂݍ��݁A����
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(), fileName, *m_factory);
}

void Obj3d::Update()
{
	//�s��̌v�Z

	//�X�P�[�����O�s��
	Matrix scalemat = Matrix::CreateScale(m_scale);

	Matrix rotmat;

	//��]�s��
	if (m_UseQuaternion)
	{//�N�H�[�^�j�I���ŉ�]���v�Z
		rotmat = Matrix::CreateFromQuaternion(m_rotationQ);
	}
	else
	{//�I�C���[�p�ŉ�]���v�Z(Z��X��Y)
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
		rotmat = rotmatZ * rotmatX * rotmatY;
	}

	//���s�ړ��s��
	Matrix transmat = Matrix::CreateTranslation(m_translation);

	//���[���h�s�������
	m_world = scalemat * rotmat * transmat;

	//�e�̍s���������
	if (m_parent)
	{
		m_world *= m_parent->m_world;
	}
}

//�s��̌v�Z
void Obj3d::Calc()
{
	Matrix scalem;
	Matrix rotm;
	Matrix transm;

	scalem = Matrix::CreateScale(m_scale);

	if (m_UseQuaternion)
	{
		rotm = Matrix::CreateFromQuaternion(m_rotationQ);
	}
	else
	{
		rotm = Matrix::CreateFromYawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);
	}

	transm = Matrix::CreateTranslation(m_translation);

	// ���[���h�s���SRT�̏��ɍ���
	m_world = Matrix::Identity;
	m_world *= scalem;
	m_world *= rotm;
	m_world *= transm;

	// �e�s�񂪂����
	if (m_parent)
	{
		// �e�s����|����
		m_world = m_world * m_parent->GetWorld();
	}
}

//�`��
void Obj3d::Draw()
{
	assert(m_Camera);

	//3D���f���̕`��
	if (m_model)
	{
		m_model->Draw(m_d3dContext.Get(), *m_states, m_world, m_Camera->GetView(), m_Camera->GetProj());
	}
}

//�I�u�W�F�N�g�̍ŏ��̐F��ݒ肷��
void Obj3d::Set_Init_Color(const DirectX::SimpleMath::Vector4& color_code)
{
	if (m_model)
	{
		// ���f�����̑S���b�V������
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();

		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// ���b�V�����̑S���b�V���p�[�c����
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				//���b�V���p�[�c�ɃZ�b�g���ꂽ�G�t�F�N�g��BasicEffect�Ƃ��Ď擾
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());

				if (eff != nullptr)
				{
					eff->SetDiffuseColor(Vector4 (color_code.x, color_code.y, color_code.z, color_code.w));
				}
			}
		}
	}
}

//�I�u�W�F�N�g�̐F��ς���
void Obj3d::Set_Color()
{
	if (m_model)
	{
		// ���f�����̑S���b�V������
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();
		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// ���b�V�����̑S���b�V���p�[�c����
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// ���b�V���p�[�c�ɃZ�b�g���ꂽ�G�t�F�N�g��BasicEffect�Ƃ��Ď擾
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());

				if (eff != nullptr)
				{
					eff->SetDiffuseColor(Vector4(0.0f, 0.0f, 1.0f, 0.0f));
				}
			}
		}
	}
}

//�G�t�F�N�g��ݒ肷��
void Obj3d::Set_Effect()
{
	//�|���S���`��p�̃G�t�F�N�g���쐬
	m_pPolygonEffect.reset(new BasicEffect(m_d3dDevice.Get()));

	if (m_model)
	{
		// ���f�����̑S���b�V������
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();

		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// ���b�V�����̑S���b�V���p�[�c����
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				meshpart->effect = m_pPolygonEffect;
			}
		}
	}
}