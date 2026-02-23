#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <memory>

// スクロールインの方向
#define SCROLL_IN_RIGHT (DirectX::XMFLOAT3( 1.0f, 0.0f, 0.0f))
#define SCROLL_IN_LEFT  (DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f))
#define SCROLL_IN_UP    (DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f))
#define SCROLL_IN_DOWN  (DirectX::XMFLOAT3( 0.0f,-1.0f, 0.0f))

class CameraPointComponent : public Component
{
private:

	GameObject* m_beforeTargetObj = nullptr;	// 前のポイント（から検索し、m_targetObjを見つける）
	GameObject* m_nextTargetPoint = nullptr;	// スクロールしたい位置（目標点直接）

	DirectX::XMFLOAT3 m_beforeDirection = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);		// プレイヤーが抜けたベクトル
	DirectX::XMFLOAT3 m_frame2BeforeDirection = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);		// 2フレーム前のプレイヤーが抜けたベクトル

	DirectX::XMFLOAT3 m_scrollDirection = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);	// 次の部屋にスクロールしたい方向
	DirectX::XMFLOAT3 m_inserDirection = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);		// プレイヤーが入ってきたベクトル
	DirectX::XMFLOAT3 m_exitDirection = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);		// プレイヤーが抜けたベクトル

	bool beforeTouched = false; // 前に触れたかどうかのフラグ
	bool afterTouched = false; // 後に触れたかどうかのフラグ

	int m_isScrollDir = 0; // スクロールどっちにスクロールするかのフラグ

public:
	CameraPointComponent(GameObject& obj);
	~CameraPointComponent() = default;

	void Update() override;

	inline void SetBeforeTargetObj(GameObject& point) { m_beforeTargetObj = &point; };
	inline void SetNextTargetObj(GameObject& pos) { m_nextTargetPoint = &pos; };
	inline void SetBeforeAndNextTargetObj(GameObject& before, GameObject& next) {
		m_beforeTargetObj = &before;
		m_nextTargetPoint = &next;
	};
	inline void SetScrollDirection(const DirectX::XMFLOAT3& dir) { m_scrollDirection = dir; };

	// ヌルポインタであったときのことも考えてGameObject*で返す
	inline GameObject* GetTargetObj()const { return m_nextTargetPoint; };

	inline int GetScrollDir() { return m_isScrollDir; };
};