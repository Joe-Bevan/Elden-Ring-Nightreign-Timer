#pragma once
#include <cstdint>

#include "../ImGui/imgui.h"

class Win32Application;

class NightreignLayer
{
public:
	NightreignLayer(Win32Application* app);
	~NightreignLayer();

	//! @brief Called on update
	void OnUpdate(const float dt);

	//! @brief Called on render
	void OnRender();

	//! @brief Returns if the layer is in edit mode
	bool IsInEditMode() const { return m_editMode; }

private:

	enum class RingState : uint8_t
	{
		kRingOneIdle = 0u,
		kRingOneClosing,
		kRingTwoIdle,
		kRingTwoClosing,
		kBossFight,

		kCount
	};

	void ResetTimer();
	ImVec4 RingStateToColour(const RingState state);
	const char* RingStateToCStr(const RingState state);
	void SetCustomImGuiStyle();

	Win32Application* m_app;

	bool m_editMode;
	bool m_timerRunning;

	uint8_t m_totalElapsedMins;
	uint8_t m_totalElapsedSeconds;

	RingState m_currentRingState;

	float m_totalElapsedTimeInSeconds;
	float m_remainingTimeTillRingChangeInSeconds;

#if _DEBUG
	uint32_t m_countedFrames;
	float m_accumulatedTimeSeconds;
	uint32_t m_FPS;
#endif
};