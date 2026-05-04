#pragma once
#include "Component.h"
#include "MoveCarComponent.h"
#include <string>

class TestExtrusionJudgeComponent : public Component
{
private:
	bool m_IsGround = false; // 地面にいるかどうかのフラグ
	bool m_IsCeiling = false; // 天井にいるかどうかのフラグ
	std::string m_MoveCarObjName = ""; // MoveCarComponentがアタッチされているオブジェクトの名前

public:
	TestExtrusionJudgeComponent(GameObject& obj);
	~TestExtrusionJudgeComponent() = default;

	void Update() override;
	inline void SetIsGround(const bool isGround) { m_IsGround = isGround; } // 地面にいるかどうかを設定
	inline bool GetIsGround()const { return m_IsGround; } // 地面にいるかどうかを取得

	inline void SetIsCeiling(const bool isCeiling) { m_IsCeiling = isCeiling; } // 天井にいるかどうかを設定
	inline bool GetIsCeiling() const { return m_IsCeiling; } // 天井にいるかどうかを取得

	void SetMoveCarObjectByName(const std::string& name) { m_MoveCarObjName = name; }
};
