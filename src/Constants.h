#pragma once
#include <cstdint>

namespace nightreign
{
	constexpr uint16_t FullDayInSeconds = 840u;		// 14 min

	constexpr uint16_t FirstCircleStarts = 270u;	// 4 mins 30s
	constexpr uint16_t FirstCircleEnds = 438u;		// 7 mins 30s
	constexpr uint16_t SecondCircleStarts = 660u;	// 11 mins
	constexpr uint16_t SecondCircleEnds = FullDayInSeconds; // 14 mins
} // nightreign

namespace core
{
	constexpr uint8_t TargetFPS = 30u;
	constexpr double TargetMS = (1.0 / (double)TargetFPS) * 1000.0;
	constexpr uint8_t NumBackbuffers = 1u;
} // core