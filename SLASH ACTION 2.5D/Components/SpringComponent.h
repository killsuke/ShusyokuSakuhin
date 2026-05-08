#include <DirectXMath.h>
#include "Component.h"

// ばね
class SpringComponent final : public Component
{
private:
	GameObject* m_SpringPartner = nullptr;	// バネの相手となるオブジェクト

	float m_Konstante = 1.0f;           // ばね定数
	float m_Damping = 0.0f;     // 減衰係数

	float m_RestLength = 0.0f;         // 初期の長さ

	bool m_IsSpringAction = true;
	bool m_FinSpringAction = false;

	float m_PreviousDistance = 0.0f;

	// Transformによるバネ更新用
	DirectX::XMFLOAT3 m_SpringVelocity = { 0.0f,0.0f,0.0f };

public:

	// コンストラクタ
	SpringComponent(GameObject& obj);

	~SpringComponent() = default;

	void Update() override;

	void SpringAction2D();

	void SpringAction3D();

	void SpringActionTransform();

	// ばね定数をセットする
	void SetK(float k);
	// ばね定数をセットする
	void SetDAMPING(float damping);

	// ばねの初期の長さをセットする
	void Setrestlng(float restLength);

	void SetSpringPartner(GameObject* partner) { m_SpringPartner = partner; }

	void MakeDamping();

	/**
	* @brief 臨界減衰 (ζ=1) となる減衰係数 c を計算する
	*
	* @param mass 質量 m
	* @param springConstant ばね定数 k
	* @return float 減衰係数 c（ζ=1 となる値）
	*/

	float ComputeCriticalDamping(float mass, float springConstant);

	void SetSpringAction(const bool flag) { m_IsSpringAction = flag; };
	void SetFinSpringAction(const bool flag) { m_FinSpringAction = flag; };

	bool GetSpringAction()const { return m_IsSpringAction; };
	bool GetFinSpringAction()const { return m_FinSpringAction; };

	// これで突然前の部屋に戻ってもちゃんとスクロールする
	void PreviousDistanceReset() {
		m_PreviousDistance = 0.0f;
		m_SpringVelocity = { 0.0f,0.0f,0.0f };
	};
};
