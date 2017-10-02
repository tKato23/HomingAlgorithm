//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include "FollowCamera.h"
#include "DebugText.h"

#include <SimpleMath.h>
#include <vector>
#include "Obj3d.h"
#include "Player.h"
#include "Enemy.h"

//定数宣言
const int Ball_Number = 20;
const int Teapot_Number = 20;
const int Max_Graund = 200;
const int Enemy_Num = 5;
const int Circle_Num = 15;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:
	Game();
	~Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

	//	デバッグのため敵をプレイヤーから一定距離離す
	void separateEnemy();

private:
    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	std::unique_ptr<DirectX::BasicEffect> m_effect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;	

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	std::unique_ptr<DebugText> m_debugText;

	//エフェクトファクトリ
	std::unique_ptr<DirectX::EffectFactory> m_factory;

	//プレイヤー
	std::unique_ptr<Player> m_Player;

	//敵
	std::unique_ptr<Enemy> m_Enemy;

	//キーボード
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	DirectX::Keyboard::KeyboardStateTracker m_tracker;

	////地形データのポインタ配列
	//std::vector<LandShape*> m_landShapeArray;

	//モデル(天球)
	Obj3d m_objSkydome;

	//モデル(地面)
	Obj3d m_objGround;

	//地面のワールド行列
	DirectX::SimpleMath::Matrix m_worldGraund;

	//カメラ
	std::unique_ptr<FollowCamera> m_Camera;

	////地形(地面)
	//LandShape m_landshape_ground;

	////地形(空)
	//LandShape m_landshape_sky;
};