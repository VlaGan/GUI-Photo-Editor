#pragma once
//--------------------------------------
// VlaGan: Допоміжні ф-ї програми
//--------------------------------------

#include <direct.h>
#include <windows.h>
#include "base_defines.h"
#include <vector>

static void GetExeDirection(DWORD& m_dwBufferSize, TCHAR m_tcBuffer[]) {
	m_dwBufferSize = GetCurrentDirectory(CHARSTRSIZE, m_tcBuffer);
}

static const bool DirExist(TCHAR m_tcBuffer[]) {
	DWORD dwFileAttributes = GetFileAttributes(m_tcBuffer);
	if (dwFileAttributes == 0xFFFFFFFF)
		return false;
	return dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}


static const bool DirExist(LPCWSTR m_sBuffer) {
	DWORD dwFileAttributes = GetFileAttributes(m_sBuffer);
	if (dwFileAttributes == 0xFFFFFFFF)
		return false;
	return dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}


static const std::string GetWorkTimeStr(double time) {
	// Hours/ minutes / seconds
	std::vector<float> m_vfCurrTime(3);
	while (time >= TIME_HOUR_UNIT) {
		m_vfCurrTime[0]++;
		time -= TIME_HOUR_UNIT;
	}

	while (time >= TIME_MINUTE_UNIT) {
		m_vfCurrTime[1]++;
		time -= TIME_MINUTE_UNIT;
	}
	m_vfCurrTime[2] = time;

	std::string msg_str = "Programm worktime = " + std::to_string(m_vfCurrTime[0]) + " hours, " 
		+ std::to_string(m_vfCurrTime[1]) + " minutes, " + std::to_string(m_vfCurrTime[2]) + " seconds!";

	return msg_str;
}



static const bool string_find_type(std::string str, std::vector<std::string> types) {
	for (auto& type : types)
		if (str.find(type))
			return true;
	return false;
}


static std::string push_index_str(const std::string str, const std::string what, size_t pos = 0) {
	if (!pos) pos = str.size() - 4;
	std::string push_str;
	size_t posidx{};

	const size_t size = str.size();
	while (posidx < size)
		pos == posidx ? push_str += what : push_str += str[posidx++];
	return push_str;
}