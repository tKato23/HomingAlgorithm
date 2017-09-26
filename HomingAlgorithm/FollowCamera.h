//----------------------------------------------------------
// 自機に追従するカメラクラス
//----------------------------------------------------------
#pragma once
#include "Camera.h"
#include <Keyboard.h>
#include "Player.h"

class FollowCamera : public Camera
{
protected:
	
	DirectX::SimpleMath::Vector3 m_target_pos;							//追従対象の座標
	float m_target_angle;												//追従対象の回転角
	DirectX::SimpleMath::Matrix m_targetWorld;
	DirectX::Keyboard* m_keyboard;										//キーボード
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;			//キーボードトラッカー
	bool m_isFPS;														//FPSカメラフラグ
	Player* m_player;													//プレイヤー

public:
	static const float CAMERA_DISTANCE;									//自機とカメラの距離
	FollowCamera(int width, int height);								//コンストラクタ
	void Update() override;												//更新処理
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);	//追従対象の座標をセット
	void SetTargetAngle(float targetangle);								//追従対象の角度をセット
	void SetKeyboard(DirectX::Keyboard* keyboard);						//キーボードをセット
	void SetPlayer(Player* player) { m_player = player; }				//プレイヤーをセット
};