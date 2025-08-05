#pragma once
#include "Component.h"
#include "CameraPattern.h"
#include <SimpleMath.h>

class CameraPointComponent : public Component
{
private:
	CameraPattern cp;
	GameObject* m_beforePoint = nullptr;	// 前のポイント（から検索し、m_targetObjを見つける）
//	GameObject* nextPoint = nullptr;	// 次のポイント
	GameObject* m_targetObj = nullptr;	// スクロールしたい位置（目標点直接）
	
	DirectX::SimpleMath::Vector3 m_scrollDirection = DirectX::SimpleMath::Vector3::Zero;	// 次の部屋にスクロールしたい方向
	DirectX::SimpleMath::Vector3 m_inserDirection = DirectX::SimpleMath::Vector3::Zero;		// プレイヤーが入ってきたベクトル
	DirectX::SimpleMath::Vector3 m_exitDirection = DirectX::SimpleMath::Vector3::Zero;		// プレイヤーが抜けたベクトル

public:
	CameraPointComponent(GameObject& obj);
	~CameraPointComponent() = default;

	void Update() override;

	inline void SetCameraPattern(const CameraPattern& pattern) { cp = pattern; };
	inline void SetBeforePoint(GameObject* point) { m_beforePoint = point; };
	inline void SetTargetObj(GameObject& pos) { m_targetObj = &pos; };
	inline void SetScrollDirection(const DirectX::SimpleMath::Vector3& dir) { m_scrollDirection = dir; };

	inline GameObject GetTargetObj()const { return *m_targetObj; };
//	inline void SetNextPoint(GameObject* point) { nextPoint = point; };
};