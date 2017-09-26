//----------------------------------------------------------
// ���@�ɒǏ]����J�����N���X
//----------------------------------------------------------
#pragma once
#include "Camera.h"
#include <Keyboard.h>
#include "Player.h"

class FollowCamera : public Camera
{
protected:
	
	DirectX::SimpleMath::Vector3 m_target_pos;							//�Ǐ]�Ώۂ̍��W
	float m_target_angle;												//�Ǐ]�Ώۂ̉�]�p
	DirectX::Keyboard* m_keyboard;										//�L�[�{�[�h
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;			//�L�[�{�[�h�g���b�J�[
	bool m_isFPS;														//FPS�J�����t���O
	Player* m_player;													//�v���C���[

public:
	static const float CAMERA_DISTANCE;									//���@�ƃJ�����̋���
	FollowCamera(int width, int height);								//�R���X�g���N�^
	void Update() override;												//�X�V����
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);	//�Ǐ]�Ώۂ̍��W���Z�b�g
	void SetTargetAngle(float targetangle);								//�Ǐ]�Ώۂ̊p�x���Z�b�g
	void SetKeyboard(DirectX::Keyboard* keyboard);						//�L�[�{�[�h���Z�b�g
	void SetPlayer(Player* player) { m_player = player; }				//�v���C���[���Z�b�g
};