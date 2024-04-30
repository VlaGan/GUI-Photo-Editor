#pragma once

#include <wtypes.h>
#include <commdlg.h>
#include <string>
#include "resource\\base_functions.h"

class CFileManager {
private:
	std::string m_sCurrFilePath;
	std::wstring m_wsCurrFilePath;
	WCHAR m_wchCurrFilePath;
	OPENFILENAME ofn;

	//-- директорія до .ехе
	TCHAR m_tchBuffer[CHARSTRSIZE];
	DWORD m_dwBufferSize{};
	std::string m_sExePath;

	//-- директорія до папки з проміжними фото
	TCHAR m_tchBufferImgTmp[CHARSTRSIZE];
	DWORD m_dwBufferSizeImgTmp{};
	std::string m_sImgTmpPath;

	int m_iImgTmpDir{};


	OPENFILENAME sfn;
	std::string m_sImgSavePath;

public:
	CFileManager();

	virtual void ChooseFilePath(HWND owner = NULL);
	virtual void ChooseSaveFilePath(HWND owner = NULL);

	IC std::string GetFilePath() { return m_sCurrFilePath; }
	IC std::string GetImgTmpPath() { return m_sImgTmpPath;}
	IC std::string GetExePath() { return m_sExePath; }

	IC LPCWSTR *GetImgPathLPCWSTR() { return &ofn.lpstrFilter; }
	IC WCHAR *GetImgPathWCHAR() { return &m_wchCurrFilePath; }

	IC const wchar_t* GetImgPathWchar_t() { return m_wsCurrFilePath.c_str(); }

	IC std::wstring GetImgPathWString() { return m_wsCurrFilePath; }

	IC std::string GetImgSavePath() { return m_sImgSavePath; }

	const bool IsImageSaveFilePath() const;

	virtual void Clear();
private:
	virtual void FileOpenWriteLPCWSTRtoString();
	virtual void FileOpenWriteLPCWSTRtoWCHAR();

	virtual void FileSaveWrite();
};