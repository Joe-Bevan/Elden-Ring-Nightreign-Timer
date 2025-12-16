#include "NightreignLayer.h"

#include "Constants.h"
#include "Win32App.h"

#include "../ImGui/imgui.h"

NightreignLayer::NightreignLayer(Win32Application* app)
	: m_app(app)
	, m_editMode(false)
	, m_timerRunning(false)
	, m_totalElapsedMins(0u)
	, m_totalElapsedSeconds(0u)
	, m_currentRingState(RingState::kRingOneIdle)
	, m_totalElapsedTimeInSeconds(0.0f)
	, m_remainingTimeTillRingChangeInSeconds(nightreign::FirstCircleStarts)
#if _DEBUG
	, m_countedFrames(0u)
	, m_accumulatedTimeSeconds(0.0f)
	, m_FPS(0u)
#endif
{
	SetCustomImGuiStyle();
}

NightreignLayer::~NightreignLayer()
{
	m_app = nullptr; // non-owning, dont delete
}

void NightreignLayer::OnUpdate(const float dt)
{
	// Update timer vars
	if (m_timerRunning)
	{
		m_totalElapsedTimeInSeconds += dt;
		m_remainingTimeTillRingChangeInSeconds -= dt;

		m_totalElapsedMins = static_cast<uint8_t>(m_totalElapsedTimeInSeconds / 60.f);
		m_totalElapsedSeconds = ((uint32_t)m_totalElapsedTimeInSeconds) % 60u;
	}

	// Update ring state
	if (m_totalElapsedTimeInSeconds >= nightreign::SecondCircleEnds)
	{
		m_currentRingState = RingState::kBossFight;
		m_timerRunning = false;
	}
	else if (m_totalElapsedTimeInSeconds >= nightreign::SecondCircleStarts)
	{
		m_currentRingState = RingState::kRingTwoClosing;
	}
	else if (m_totalElapsedTimeInSeconds >= nightreign::FirstCircleEnds)
	{
		m_currentRingState = RingState::kRingTwoIdle;
	}
	else if (m_totalElapsedTimeInSeconds >= nightreign::FirstCircleStarts)
	{
		m_currentRingState = RingState::kRingOneClosing;
	}

	// Has the ring state changed?
	static RingState prevState = m_currentRingState;
	if (prevState != m_currentRingState)
	{
		switch (m_currentRingState)
		{
		case RingState::kRingOneIdle:
			m_remainingTimeTillRingChangeInSeconds = nightreign::FirstCircleStarts;
			break;
		case RingState::kRingOneClosing:
			m_remainingTimeTillRingChangeInSeconds = nightreign::FirstCircleEnds - nightreign::FirstCircleStarts;
			break;
		case RingState::kRingTwoIdle:
			m_remainingTimeTillRingChangeInSeconds = nightreign::SecondCircleStarts - nightreign::FirstCircleEnds;
			break;
		case RingState::kRingTwoClosing:
			m_remainingTimeTillRingChangeInSeconds = nightreign::SecondCircleEnds - nightreign::SecondCircleStarts;
			break;
		case RingState::kBossFight:
			m_remainingTimeTillRingChangeInSeconds = 0u;
			break;
		}

	}
	prevState = m_currentRingState;

	if (GetAsyncKeyState(VK_F9) & 0x01)
	{
		m_editMode = !m_editMode;
		m_app->SetInputPassthrough(m_editMode);
	}

	if (GetAsyncKeyState(VK_F10) & 0x01)
	{
		m_timerRunning = true;
		ResetTimer();
	}

#if _DEBUG
	++m_countedFrames;
	m_accumulatedTimeSeconds += dt;
	if (m_accumulatedTimeSeconds >= 1.0f)
	{
		m_FPS = m_countedFrames;
		m_countedFrames = 0u;
		m_accumulatedTimeSeconds = 0.0f;
	}
#endif
}

void NightreignLayer::OnRender()
{
	// Draw red border if in edit mode
	if (m_editMode)
	{
		const float screenWidth = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
		const float screenHeight = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));
		ImGui::GetBackgroundDrawList()->AddRect({ 0,0 }, ImVec2(screenWidth, screenHeight), ImColor(1.0f, 0.0f, 0.0f), 0.0f, 0, 5u);
	}

	const uint8_t remainingMins = static_cast<uint8_t>(m_remainingTimeTillRingChangeInSeconds / 60.f);
	const uint8_t remainingSecs = (static_cast<int>(m_remainingTimeTillRingChangeInSeconds)) % 60;

	bool running = m_app->IsRunning();
	ImGui::Begin("Nightreign Timer - By Joe B.", &running);
	m_app->SetRunning(running);

	ImGui::Text("Ring state:");
	ImGui::SameLine();
	ImGui::TextColored(RingStateToColour(m_currentRingState), "%s (%um %us)", RingStateToCStr(m_currentRingState), remainingMins, remainingSecs);
	ImGui::Text("Night progress (%um %us):", m_totalElapsedMins, m_totalElapsedSeconds);
	const float progress = m_totalElapsedTimeInSeconds / nightreign::FullDayInSeconds;
	ImGui::ProgressBar(progress);

	ImGui::Separator();

	ImGui::BeginDisabled(!m_editMode);
	if (ImGui::Button(m_timerRunning ? "Pause" : "Start [F10]"))
	{
		m_timerRunning = !m_timerRunning;
	}
	ImGui::SameLine();
	if (ImGui::Button("Restart [F10]"))
	{
		m_timerRunning = false;
		ResetTimer();
	}
	ImGui::EndDisabled();
	ImGui::TextDisabled("[ F9 ] - Toggle Edit mode");

#if _DEBUG
	ImGui::Separator();
	ImGui::Text("Counted Frames: %u", m_countedFrames);
	ImGui::Text("Acc time in sec: %.2f", m_accumulatedTimeSeconds);
	ImGui::Text("FPS: %u", m_FPS);
#endif
	ImGui::End();
}

void NightreignLayer::ResetTimer()
{
	m_totalElapsedTimeInSeconds = 0u;
	m_totalElapsedSeconds = 0u;
	m_totalElapsedMins = 0u;
	m_currentRingState = RingState::kRingOneIdle;
	m_remainingTimeTillRingChangeInSeconds = nightreign::FirstCircleStarts;
}

ImVec4 NightreignLayer::RingStateToColour(const RingState state)
{
	switch (state)
	{
	case RingState::kRingOneIdle: return ImVec4(0, 1, 0, 1);
	case RingState::kRingOneClosing: return ImVec4(1, 0, 0, 1);

	case RingState::kRingTwoIdle: return ImVec4(0, 1, 0, 1);
	case RingState::kRingTwoClosing: return ImVec4(1, 0, 0, 1);

	case RingState::kBossFight: return ImVec4(0, 1, 0, 1);
	}
	return ImVec4(1, 1, 1, 1);
}

const char* NightreignLayer::RingStateToCStr(const RingState state)
{
	switch (state)
	{
	case RingState::kRingOneIdle: return "Ring 1 idle";
	case RingState::kRingOneClosing: return "Ring 1 closing";

	case RingState::kRingTwoIdle: return "Ring 2 idle";
	case RingState::kRingTwoClosing: return "Ring 2 closing";

	case RingState::kBossFight: return "Boss fight (gl!)";
	}
	return "Unknown ring state";
}

void NightreignLayer::SetCustomImGuiStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.04f, 0.04f, 0.04f, 0.99f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.51f, 0.51f, 0.80f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.03f, 0.03f, 0.04f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.01f, 0.01f, 0.01f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.17f, 0.17f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.53f, 0.25f, 0.77f, 1.0f);
	colors[ImGuiCol_SliderGrab] = ImVec4{ 0.4f, 0.12f, 0.611f, 1.0f };
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.53f, 0.25f, 0.77f, 1.0f);
	colors[ImGuiCol_Button] = ImVec4(0.21f, 0.22f, 0.23f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.51f, 0.51f, 0.80f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.54f, 0.55f, 0.55f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.51f, 0.51f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.16f, 0.16f, 0.16f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.23f, 0.23f, 0.24f, 0.80f);
	colors[ImGuiCol_Tab] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
	colors[ImGuiCol_TabSelected] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
	colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.53f, 0.25f, 0.77f, 1.0f);
	colors[ImGuiCol_TabDimmed] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
	colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
	colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4{ 0.4f, 0.12f, 0.611f, 1.0f };
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.14f, 0.87f, 0.05f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.30f, 0.60f, 0.10f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.23f, 0.78f, 0.02f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.46f, 0.47f, 0.46f, 0.06f);
	colors[ImGuiCol_TextLink] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavCursor] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.78f, 0.69f, 0.69f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	style.WindowRounding = 4.0f;
	style.FrameRounding = 4.0f;
	style.GrabRounding = 3.0f;
	style.PopupRounding = 4.0f;
	style.TabRounding = 4.0f;
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.ScrollbarSize = 10.0f;
	style.GrabMinSize = 10.0f;
	style.SeparatorTextBorderSize = 2.0f;
}

