#include "Collision.h"
using namespace DirectX::SimpleMath;

//���Ƌ��Ƃ̓����蔻��
bool CheakSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	//��A��B�̒��S���W�̍����v�Z����
	Vector3 sub = sphereB.Center - sphereA.Center;

	float distance_Square;
	//�O�����̒藝�ŁA�x�N�g���̒������v�Z����
	distance_Square = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;

	//���a�̘a�̓��
	float radius_Square;

	radius_Square = sphereA.Radius + sphereB.Radius;
	radius_Square = radius_Square * radius_Square;

	//���������a�̘a���傫����Γ������Ă��Ȃ�
	if (distance_Square > radius_Square)
	{
		return false;
	}

	return true;
}

//3�_�̍��W����O�p�`���v�Z����(�����v���ɒ��_���w��)
void ComputeTriangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, Triangle* _triangle)
{
	//���W���R�s�[����
	_triangle->P0 = _p0;
	_triangle->P1 = _p1;
	_triangle->P2 = _p2;

	//�@���x�N�g�����v�Z
	Vector3 P0_P1 = _p1 - _p0;
	Vector3 P1_P2 = _p2 - _p1;

	//�O�ς�2�x�N�g���ɐ����ȃx�N�g�����Z�o
	_triangle->Normal = P0_P1.Cross(P1_P2);
	_triangle->Normal.Normalize();
}

//�_�Ɩ@���t���O�p�`��1�ԋ߂��_ (�_�A�O�p�`�A1�ԋ߂��_(�o��))
void ClosestPtPoint2Triangle(const Vector3& _point, const Triangle& _tri, Vector3* _closest)
{
	//_point��P0�̊O���̒��_�̈�̒��ɂ��邩�`�F�b�N
	Vector3 P0_P1 = _tri.P1 - _tri.P0;
	Vector3 P0_P2 = _tri.P2 - _tri.P0;
	Vector3 P0_PT = _point - _tri.P0;

	float d1 = P0_P1.Dot(P0_PT);
	float d2 = P0_P2.Dot(P0_PT);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		//P0��1�ԋ߂�
		*_closest = _tri.P0;
		return;
	}

	//_point��P1�̊O���̒��_�̈�̒��ɂ��邩�`�F�b�N
	Vector3 P1_PT = _point - _tri.P1;

	float d3 = P0_P1.Dot(P1_PT);
	float d4 = P0_P2.Dot(P1_PT);

	if (d3 >= 0.0f && d4 <= d3)
	{
		*_closest = _tri.P1;
		return;
	}

	//_point��P0_P1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����_point��P0_P1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*_closest = _tri.P0 + v * P0_P1;
		return;
	}

	//_point��P2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 P2_PT = _point - _tri.P2;
	float d5 = P0_P1.Dot(P2_PT);
	float d6 = P0_P2.Dot(P2_PT);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*_closest = _tri.P2;
		return;
	}

	//_point��P0_P2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����_point��P0_P2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*_closest = _tri.P0 + w * P0_P2;
		return;
	}

	//_point��P1_P2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����_point��P1_P2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*_closest = _tri.P1 + w * (_tri.P2 - _tri.P1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*_closest = _tri.P0 + P0_P1 * v + P0_P2 * w;
}

//�O�p�`�Ɠ_�̓����蔻��
bool CheckPoint2Triangle(const Vector3& _point, const Triangle& _triangle)
{
	//�_�ƎO�p�`�͓��ꕽ�ʏ�ɂ�����̂Ƃ��Ă��܂��B���ꕽ�ʏ�ɖ����ꍇ�͐��������ʂɂȂ�܂���
	//����͊O�Ƃ݂Ȃ��܂��B
	//ABC���O�p�`���ǂ����̃`�F�b�N�͏ȗ�...

	//�_0��1�A 1��2�A 2��0 �̃x�N�g�������ꂼ��v�Z
	Vector3 v01 = _triangle.P1 - _triangle.P0;
	Vector3 v12 = _triangle.P2 - _triangle.P1;
	Vector3 v20 = _triangle.P0 - _triangle.P2;

	//�O�p�`�̊e���_����_�ւ̃x�N�g�������ꂼ��v�Z
	Vector3 v0p = _point - _triangle.P0;
	Vector3 v1p = _point - _triangle.P1;
	Vector3 v2p = _point - _triangle.P2;

	//�e�Ӄx�N�g���ƁA�_�ւ̃x�N�g���̊O�ς��v�Z
	Vector3 c0 = v01.Cross(v0p);
	Vector3 c1 = v12.Cross(v1p);
	Vector3 c2 = v20.Cross(v2p);

	//���ςœ����������ǂ������ׂ�
	float dot01 = c0.Dot(c1);
	float dot02 = c0.Dot(c2);

	//�O�σx�N�g�����S�ē��������Ȃ�A�O�p�`�̓����ɓ_������
	if (dot01 > 0 && dot02 > 0)
	{
		return true;
	}

	//�O�p�`�̊O���ɓ_������
	return false;
}

//���Ɩ@���t���O�p�`�̓�����`�F�b�N (���A�O�p�`�A��_���W(�o��))
bool CheckSphere2Triangle(const Sphere& _sphere, const Triangle& _tri, Vector3 *_inter)
{
	Vector3 p;

	// ���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(_sphere.Center, _tri, &p);
	Vector3 v = p - _sphere.Center;

	// ���ƎO�p�`����������̂́A���̒��S����_p�܂ł̋��������̔��a�����������ꍇ
	if (v.Dot(v) <= _sphere.Radius * _sphere.Radius)
	{
		if (_inter)
		{
			*_inter = p;
		}
		return true;
	}

	return false;

	// ���ƕ��ʁi�O�p�`������Ă��镽�ʁj�̓����蔻��
	// ���ƕ��ʂ̋������v�Z
	float ds = _sphere.Center.Dot(_tri.Normal);
	float dt = _tri.P0.Dot(_tri.Normal);
	float dist = ds - dt;

	// ���������a�ȏ�Ȃ�A������Ȃ�
	if (fabsf(dist) > _sphere.Radius)	return false;

	// ���S�_�𕽖ʂɎˉe�����Ƃ��A�O�p�`�̓����ɂ���΁A�������Ă���
	// �ˉe�������W
	Vector3 Center = -dist * _tri.Normal + _sphere.Center;

	// �O�p�`�̊O���ɂȂ���΁A�������Ă��Ȃ�
	if (!CheckPoint2Triangle(Center, _tri))	return false;

	if (_inter)
	{
		*_inter = Center;	// ��_���R�s�[
	}
	return true;
}

bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, Vector3 *_inter)
{
	const float epsilon = -1.0e-5f;	// �덷�z���p�̔����Ȓl
	Vector3 	LayV;		// �����̏I�_���n�_
	Vector3 	tls;		// �O�p�`�̒��_0�������̎n�_
	Vector3 	tle;		// �O�p�`�̒��_0�������̏I�_
	float 	distl0;
	float 	distl1;
	float 	dp;
	float 	denom;
	float 	t;
	Vector3	s;			// �����ƕ��ʂƂ̌�_
	Vector3 	st0;		// ��_���O�p�`�̒��_0
	Vector3 	st1;		// ��_���O�p�`�̒��_1
	Vector3 	st2;		// ��_���O�p�`�̒��_2
	Vector3 	t01;		// �O�p�`�̒��_0�����_1
	Vector3 	t12;		// �O�p�`�̒��_1�����_2
	Vector3 	t20;		// �O�p�`�̒��_2�����_0
	Vector3	m;

	//�����̎n�_���O�p�n�̗����ɂ���΁A������Ȃ�
	tls = _segment.start - _triangle.P0;
	distl0 = tls.Dot(_triangle.Normal);	// �����̎n�_�ƕ��ʂ̋���

	if (distl0 <= epsilon) return false;

	//�����̏I�_���O�p�n�̕\���ɂ���΁A������Ȃ�
	tle = _segment.end - _triangle.P0;
	distl1 = tle.Dot(_triangle.Normal);	// �����̏I�_�ƕ��ʂ̋���

	if (distl1 >= -epsilon) return false;

	//�����ƕ��ʂƂ̌�_s�����
	denom = distl0 - distl1;
	t = distl0 / denom;
	LayV = _segment.end - _segment.start;	// �����̕����x�N�g�����擾
	s = t * LayV + _segment.start;

	//��_���O�p�`�̓����ɂ��邩�ǂ����𒲂ׂ�B

	//�O�p�`�̓����ɂ���ꍇ�A��_����e���_�ւ̃x�N�g���Ɗe�Ӄx�N�g���̊O�ρi�O�g�j���A�S�Ė@���Ɠ�������������

	//��ł���������v���Ȃ���΁A������Ȃ��B
	st0 = _triangle.P0 - s;
	t01 = _triangle.P1 - _triangle.P0;
	m = st0.Cross(t01);
	dp = m.Dot(_triangle.Normal);

	if (dp <= epsilon) return false;

	st1 = _triangle.P1 - s;
	t12 = _triangle.P2 - _triangle.P1;
	m = st1.Cross(t12);
	dp = m.Dot(_triangle.Normal);

	if (dp <= epsilon) return false;


	st2 = _triangle.P2 - s;
	t20 = _triangle.P0 - _triangle.P2;
	m = st2.Cross(t20);
	dp = m.Dot(_triangle.Normal);

	if (dp <= epsilon) return false;

	if (_inter)
	{
		*_inter = s;	// ��_���R�s�[
	}
	return true;
}