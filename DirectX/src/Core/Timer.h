#pragma once

#include <chrono>

class Timer
{
public:
	Timer();

	double Peek();
	double Reset();
private:
	std::chrono::time_point<std::chrono::system_clock> m_StartTime;
};
