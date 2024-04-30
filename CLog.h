#pragma once
#pragma warning(disable : 4996)

#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <direct.h>
#include <windows.h>
#include <vector>
#include "resource/base_functions.h"

class CLog {

private:

	TCHAR m_tchBuffer[CHARSTRSIZE];
	TCHAR m_tcNewLogBuffer[CHARSTRSIZE];
	DWORD m_dwBufferSize{};


	std::string LogDir;
	std::string LogName;
	int lgdir{};
	std::ofstream log;

public:
	CLog();
	~CLog();

	void MkLog();
	template<typename type> void write(type text);

private:
	void write_time();
	void SetupLogDir();
};
