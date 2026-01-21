#pragma once
#include <unordered_set>
#include <string>

class HitStopManager
{
private:

	static std::unordered_set<std::string> m_TargetTags;
	static float m_RecordTime;
	static float m_HitStopTime;
	static bool m_IsHitStopActive;	// このマネージャーのアップデートを有効にするかどうか

public:

	HitStopManager() = delete;
	~HitStopManager() = delete;

	static void Update();

	static void AddTargetTag(const std::string& tag) { m_TargetTags.insert(tag); };
	static void AddTargetTags(const std::unordered_set<std::string>& tags) {
		m_TargetTags.insert(tags.begin(), tags.end());
	};
	static void RemoveTargetTag(const std::string& tag) { m_TargetTags.erase(tag); };
	static void ClearTargetTags() { m_TargetTags.clear(); };
	static void SetHitStopTime(const float time) { m_HitStopTime = time; };
	static void SetIsHitStopActive(const bool isActive) { m_IsHitStopActive = isActive; };
};