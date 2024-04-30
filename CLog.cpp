//--------------------------------------------------------
// VlaGan: CLog класс лога програми
//--------------------------------------------------------

#include "CLog.h"

constexpr auto m_sLogFolderName = "\\__appdata__\\logs";

CLog::CLog() {
	SetupLogDir();

	if (!DirExist(m_tcNewLogBuffer))
		lgdir = _mkdir(LogDir.c_str());

	MkLog();
	log.open(LogName);

	write("Log file has been created!");
	write("Log file path: " + LogDir);
	write("Log path buffer size = " + std::to_string(m_dwBufferSize));
}

CLog::~CLog() {
	log.close();
}

void CLog::MkLog() {
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);

	LogName = LogDir;
	LogName += "\\log_";
	std::vector<int> m_viCurrTime =
	{
		now->tm_wday + 1, now->tm_mon + 1, now->tm_year + 1900,
		now->tm_hour, now->tm_min,  now->tm_sec
	};
	const char* suf = "_";
	for (size_t i = 0; i < m_viCurrTime.size(); i++)
		LogName += (m_viCurrTime[i] >= 10 ? std::to_string(m_viCurrTime[i]) : '0' + std::to_string(m_viCurrTime[i]))
		+ (i < m_viCurrTime.size() - 1 ? suf : ".txt");
}

template<typename type> void CLog::write(type text) {
	write_time();
	log << text << std::endl;
}

void CLog::write_time() {
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);
	const char tp[] = ":";
	std::vector<int> m_viCurrTime =
	{
		now->tm_wday + 1, now->tm_mon + 1, now->tm_year + 1900,
		now->tm_hour, now->tm_min,  now->tm_sec
	};

	log << "[";
	for (size_t i = 0; i < m_viCurrTime.size(); i++)
		log << (m_viCurrTime[i] >= 10 ? std::to_string(m_viCurrTime[i]) : '0' + std::to_string(m_viCurrTime[i]))
		<< (i < m_viCurrTime.size() - 1 ? tp : "");
	log << "] ";
}

void CLog::SetupLogDir() {
	GetExeDirection(m_dwBufferSize, m_tchBuffer);

	size_t buf1pos{}, buf2pos{};
	while (m_tchBuffer[buf1pos] != '\0')
		m_tcNewLogBuffer[buf1pos++] = m_tchBuffer[buf1pos];
	while (m_sLogFolderName[buf2pos] != '\0')
		m_tcNewLogBuffer[buf1pos++] = m_sLogFolderName[buf2pos++];
	m_dwBufferSize = buf1pos;
	buf1pos = 0;

	while (m_tcNewLogBuffer[buf1pos] != '\0')
		LogDir += (char)m_tcNewLogBuffer[buf1pos++];
}