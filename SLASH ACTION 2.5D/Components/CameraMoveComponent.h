#pragma once
#include "Component.h"
#include "Structs/CameraPattern.h"
#include <DirectXMath.h>

class CameraMoveComponent : public Component
{
private:
	GameObject* m_moveTarget = nullptr; // カメラの移動ターゲット
	float m_chaseHeight = 0.0f;	// この値の高さにプレイヤー達すると追従を始める
	CameraPattern m_CameraPattern = CameraPattern::CAMERA_NONE;

public:
	CameraMoveComponent(GameObject& obj);
	~CameraMoveComponent() = default;

	void Update() override;

	void ChaseCamera(GameObject& cameraObj, GameObject& player);
	void ChaseXAndYCamera(GameObject& cameraObj, GameObject& player);
	void Chase_XCamera(GameObject& cameraObj, GameObject& player);
	void Chase_YCamera(GameObject& cameraObj, GameObject& player);
	void SpringCamera(GameObject& cameraObj);
	void AdjustmentHeight(GameObject& cameraObj,GameObject& player);

	inline void SetChaseHeight(const float height) { m_chaseHeight = height; };
	// カメラの移動ターゲットをセット
	// スプリング目的であれば、差分位置のリセット処理を行う
	void SetMoveTarget(GameObject& target);

	CameraPattern GetCameraPattern() const { return m_CameraPattern; };
};

// カメラのスクロールを制御
// カメラのスクロールするポイントをセットして、
// そのポイントにコライダーをセット、
// プレイヤーがそのポイントに当たったら
// カメラをバネの挙動でスクロールさせる
// 常時横スクロールパターンと
// スクロールしないパターンを用意する
// バネの挙動でスクロールする場合、
// それ用のコンポーネントをポイントごとにセット
// ポイントに前ポイントと次ポイントを持っておく（リスト的な）
// これは前の部屋と次の部屋を移動するため
// そのポイントごとに横スクロールがアリかナシかを確認して切替
// プレイヤーが上下に移動した場合、思った以上に移動したら追いかける？
// 地面に当たったら、カメラをプレイヤーの高さに合わせる