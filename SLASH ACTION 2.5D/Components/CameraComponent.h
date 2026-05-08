#pragma once

#include "GameObject/GameObject.h"
#include "TransformComponent.h"

namespace {
	constexpr float DEFAULT_FOV = 45.0f;
	constexpr float WIDE_FOV = 90.0f;
	constexpr float TELEPHOTO_FOV = 20.0f;
}

enum class FieldOfView {
	DEFAULT,	// ÉfÉtÉHÉãÉg
	WIDE,		// çLäp
	TELEPHOTO,	// ñ]âì

	MAX
};

//-----------------------------------------------------------------------------
//CameraÉNÉâÉX
//-----------------------------------------------------------------------------
class CameraComponent : public Component {
protected:

	DirectX::XMFLOAT3 m_Target = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	DirectX::XMFLOAT3 m_OffsetPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_OffsetTarget = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX m_ViewMatrix2D = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_ProjectionMatrix2D = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX m_ViewMatrix3D = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_ProjectionMatrix3D = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX m_ViewMatrixSky = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_ProjectionMatrixSky = DirectX::XMMatrixIdentity();

	FieldOfView m_FieldOfView = FieldOfView::DEFAULT;

	void Update2D();
	void Update3D();
	void UpdateSky();

public:

	CameraComponent(GameObject& obj);
	~CameraComponent() = default;

	void Update() override;

	void SetTarget(const DirectX::XMFLOAT3& target) { m_Target = target; };
	void AddTarget(const DirectX::XMFLOAT3& target) { m_Target += target; };

	void SetFieldOfView(const FieldOfView fov) { m_FieldOfView = fov; }
	void SetOffsetPosition(const DirectX::XMFLOAT3& offsetPos) { m_OffsetPosition = offsetPos; }
	void SetOffsetTarget(const DirectX::XMFLOAT3& offsetTgt) { m_OffsetTarget = offsetTgt; }

	DirectX::XMFLOAT3 GetTarget() const { return m_Target; }

	// ì]íuÇµÇƒÇ¢Ç»Ç¢èÛë‘ÇÃÉÇÉmÇï‘Ç∑
	DirectX::XMMATRIX GetView2D() const { return m_ViewMatrix2D; };
	DirectX::XMMATRIX GetView3D() const { return m_ViewMatrix3D; };

	DirectX::XMMATRIX GetProj2D() const { return m_ProjectionMatrix2D; };
	DirectX::XMMATRIX GetProj3D() const { return m_ProjectionMatrix3D; };

	DirectX::XMFLOAT3 GetOffsetPosition() const { return m_OffsetPosition; };
	FieldOfView GetFieldOfView() const { return m_FieldOfView; };
};