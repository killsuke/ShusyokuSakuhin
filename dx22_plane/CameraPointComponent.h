#pragma once
#include "Component.h"
//#include "CameraPattern.h"
#include <SimpleMath.h>
#include <memory>

// スクロールインの方向
#define SCROLL_IN_RIGHT (DirectX::SimpleMath::Vector3( 1.0f, 0.0f, 0.0f))
#define SCROLL_IN_LEFT  (DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f))
#define SCROLL_IN_UP    (DirectX::SimpleMath::Vector3( 0.0f, 1.0f, 0.0f))
#define SCROLL_IN_DOWN  (DirectX::SimpleMath::Vector3( 0.0f,-1.0f, 0.0f))

class CameraPointComponent : public Component
{
private:

	GameObject* m_beforeTargetObj = nullptr;	// 前のポイント（から検索し、m_targetObjを見つける）
	GameObject* m_nextTargetPoint = nullptr;	// スクロールしたい位置（目標点直接）

	DirectX::SimpleMath::Vector3 m_beforeDirection = DirectX::SimpleMath::Vector3::Zero;		// プレイヤーが抜けたベクトル

	DirectX::SimpleMath::Vector3 m_scrollDirection = DirectX::SimpleMath::Vector3::Zero;	// 次の部屋にスクロールしたい方向
	DirectX::SimpleMath::Vector3 m_inserDirection = DirectX::SimpleMath::Vector3::Zero;		// プレイヤーが入ってきたベクトル
	DirectX::SimpleMath::Vector3 m_exitDirection = DirectX::SimpleMath::Vector3::Zero;		// プレイヤーが抜けたベクトル

	bool beforeTouched = false; // 前に触れたかどうかのフラグ
	bool afterTouched = false; // 後に触れたかどうかのフラグ

	int m_isScrollDir = 0; // スクロールどっちにスクロールするかのフラグ

public:
	CameraPointComponent(GameObject& obj);
	~CameraPointComponent() = default;

	void Update() override;

	inline void SetBeforeTargetObj(GameObject& point) { m_beforeTargetObj = &point; };
	inline void SetNextTargetObj(GameObject& pos) { m_nextTargetPoint = &pos; };
	inline void SetScrollDirection(const DirectX::SimpleMath::Vector3& dir) { m_scrollDirection = dir; };

	// ヌルポインタであったときのことも考えてGameObject*で返す
	inline GameObject* GetTargetObj()const { return m_nextTargetPoint; };
};