#pragma once



constexpr auto m_sLogFolderName = "\\__appdata__\\logs";

#pragma warning(disable : 4996)

static class Log {

private:

	TCHAR m_tchBuffer[CHARSTRSIZE];
	TCHAR m_tcNewLogBuffer[CHARSTRSIZE];
	DWORD m_dwBufferSize{};


	std::string LogDir;
	std::string LogName;
	int lgdir{};
	std::ofstream log;

public:
	Log();
	~Log();

	void MkLog();
	template<typename type> void write(type text);

private:
	void write_time();
	void SetupLogDir();
};

Log::Log() {
	SetupLogDir();
	
	if (!DirExist(m_tcNewLogBuffer))
		lgdir = _mkdir(LogDir.c_str());

	MkLog();
	log.open(LogName);

	write("Log file has been created!");
	write("Log file path: " + LogDir);
	write("Buff size = " + std::to_string(m_dwBufferSize));
	//for (size_t i = 0;i < m_dwBufferSize;i++)
	//	log << (char)m_tchBuffer[i];
	//log << std::endl;
}

Log::~Log() {
	log.close();
}

void Log::MkLog() {
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

template<typename type> void Log::write(type text) {
	write_time();
	log << text << std::endl;
}

void Log::write_time() {
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
		<< (i < m_viCurrTime.size()-1 ? tp: "");
	log << "] ";
}

void Log::SetupLogDir() {
	//m_dwBufferSize = GetCurrentDirectory(CHARSTRSIZE, m_tchBuffer);

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

