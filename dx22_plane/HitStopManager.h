#pragma once
#include <unordered_set>
#include <string>

class HitStopManager
{
private:

	static std::unordered_set<std::string> m_TargetTags;
	static float m_RecordTime;
	static float m_HitStopTime;

public:

	HitStopManager() = delete;
	~HitStopManager() = delete;

	static void Update();

	static void AddTargetTag(const std::string& tag) { m_TargetTags.insert(tag); };
	static void RemoveTargetTag(const std::string& tag) { m_TargetTags.erase(tag); };
	static void ClearTargetTags() { m_TargetTags.clear(); };
	static void SetHitStopTime(const float time) { m_HitStopTime = time; };
};