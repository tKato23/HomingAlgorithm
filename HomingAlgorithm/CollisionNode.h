#pragma once
#include "Collision.h"
#include "Obj3d.h"

//当たりノード
class CollisionNode
{
protected:
	//デバッグ表示ON
	static bool m_DebugVisible;

public:
	static void SetDebugVisible(bool flag) { m_DebugVisible = flag; }
	static bool GetDebugVisible() { return m_DebugVisible; }

public:
	//初期化
	virtual void Initialize() = 0;

	//更新処理
	virtual void Update() = 0;
	
	//描画処理
	virtual void Draw() = 0;

	//親をセットする
	void SetParent(Obj3d* parent);

	//親からのオフセットをセット
	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_trans = trans; }

protected:
	//デバッグ表示用オブジェクト
	Obj3d m_obj;

	//親からのオフセット
	DirectX::SimpleMath::Vector3 m_trans;
};

//球の当たりノード
class SphereNode : public CollisionNode, public Sphere
{
public:
	//コンストラクタ
	SphereNode();

	//初期化
	void Initialize();

	//更新処理
	void Update();

	//描画処理
	void Draw();

	//ローカル半径をセット
	//void SetLocalRadius(float radius) { m_LocalRadius = radius; }
	void SetLocalRadius(const DirectX::SimpleMath::Vector3 radius) { m_LocalRadius = radius; }

protected:

	//ローカル半径
	//float m_LocalRadius;
	DirectX::SimpleMath::Vector3 m_LocalRadius;
};