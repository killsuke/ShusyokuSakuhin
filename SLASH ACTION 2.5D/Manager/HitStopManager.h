#pragma once
#include <unordered_set>
#include <string>

class HitStopManager final
{
private:

	static inline std::unordered_set<std::string> m_TargetTags;
	static inline float m_RecordTime = 0.0f;
	static inline float m_HitStopTime = 0.0f;
	static inline bool m_IsHitStopActive = true;	// このマネージャーのアップデートを有効にするかどうか

public:

	// コンストラクタ・デストラクタを削除
	HitStopManager() = delete;
	~HitStopManager() = delete;

	// コピー・ムーブも削除
	HitStopManager(const HitStopManager&) = delete;
	HitStopManager(HitStopManager&&) = delete;
	HitStopManager& operator=(const HitStopManager&) = delete;
	HitStopManager& operator=(HitStopManager&&) = delete;

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