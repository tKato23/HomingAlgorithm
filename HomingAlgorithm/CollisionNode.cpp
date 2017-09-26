
#include "pch.h"
#include "CollisionNode.h"

using namespace DirectX::SimpleMath;

bool CollisionNode::m_DebugVisible = true;

////////////////////////
//CollisionNodeのメンバ関数
////////////////////////

//親をセットする
void CollisionNode::SetParent(Obj3d * parent)
{
	m_obj.SetParent(parent);
}


////////////////////////
//SphereNodeのメンバ関数
////////////////////////

//コンストラクタ
SphereNode::SphereNode()
{
	//デフォルト半径を1メートルにする
	m_LocalRadius = Vector3(1.0f, 1.0f, 1.0f);
}

//初期化
void SphereNode::Initialize()
{
	//デバッグ用モデルを読み込む
	m_obj.LoadModel(L"Resources/SphereNode.cmo");
}

//更新処理
void SphereNode::Update()
{
	m_obj.SetTranslation(m_trans);
	m_obj.SetScale(Vector3(m_LocalRadius));

	//ワールド行列の更新
	m_obj.Update();

	{//ワールド座標での当たり判定球を計算

		//計算済みのワールド行列を取得
		const Matrix& worldm = m_obj.GetWorld();

		//モデル座標系での中心点
		Vector3 center(0, 0, 0);
		
		//モデル座標系での右端
		Vector3 right(1, 0, 0);

		//ワールド座標に変換
		center = Vector3::Transform(center, worldm);
		right = Vector3::Transform(right, worldm);

		//当たり判定球に結果を代入
		Sphere::Center = center;
		Sphere::Radius = Vector3::Distance(center, right);
	}
}

//描画処理
void SphereNode::Draw()
{
	m_obj.Draw();
}