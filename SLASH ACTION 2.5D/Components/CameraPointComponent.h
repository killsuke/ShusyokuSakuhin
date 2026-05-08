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

	GameObject* m_BeforeTargetObj = nullptr;	// 前のポイント（から検索し、m_targetObjを見つける）
	GameObject* m_NextTargetPoint = nullptr;	// スクロールしたい位置（目標点直接）

	DirectX::XMFLOAT3 m_BeforeDirection = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);		// プレイヤーが抜けたベクトル
	DirectX::XMFLOAT3 m_Frame2BeforeDirection = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);		// 2フレーム前のプレイヤーが抜けたベクトル

	DirectX::XMFLOAT3 m_ScrollDirection = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);	// 次の部屋にスクロールしたい方向
	DirectX::XMFLOAT3 m_InserDirection = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);		// プレイヤーが入ってきたベクトル
	DirectX::XMFLOAT3 m_ExitDirection = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);		// プレイヤーが抜けたベクトル

	bool m_IsBeforeTouched = false; // 前に触れたかどうかのフラグ
	bool m_IsAfterTouched = false; // 後に触れたかどうかのフラグ

	int m_ScrollDir = 0; // スクロールどっちにスクロールするかのフラグ

public:
	CameraPointComponent(GameObject& obj);
	~CameraPointComponent() = default;

	void Update() override;

	void SetBeforeTargetObj(GameObject& point) { m_BeforeTargetObj = &point; };
	void SetNextTargetObj(GameObject& pos) { m_NextTargetPoint = &pos; };
	void SetBeforeAndNextTargetObj(GameObject& before, GameObject& next) {
		m_BeforeTargetObj = &before;
		m_NextTargetPoint = &next;
	};
	void SetScrollDirection(const DirectX::XMFLOAT3& dir) { m_ScrollDirection = dir; };

	// ヌルポインタであったときのことも考えてGameObject*で返す
	GameObject* GetTargetObj()const { return m_NextTargetPoint; };

	int GetScrollDir() { return m_ScrollDir; };
};