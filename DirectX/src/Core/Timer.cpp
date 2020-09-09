#include "pch.h"
#include "Timer.h"

Timer::Timer()
{
	m_StartTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
}

double Timer::Peek()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - m_StartTime).count();
}

double Timer::Reset()
{
	double ret = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - m_StartTime).count();
	m_StartTime = std::chrono::system_clock::now();
	return ret;
}
