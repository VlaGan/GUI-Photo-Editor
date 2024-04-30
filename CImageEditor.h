#pragma once


#include <string> 
#include <opencv2\\opencv.hpp>
#include "resource\\base_defines.h"

class CImageEditor {
private:
	std::string m_sCurrentImageFile;

	std::string m_sImageTmpFolder;

	cv::Mat m_cvImage{};

	bool m_bIsImageLoaded{};

	std::vector<cv::Mat> m_vIUndoRedo;
	
	std::wstring m_wchTmpFilePath;

	std::vector<std::string> m_vsTmpImgFilenames;

	int m_iTmpImgIdx;
	int m_iSessionNum{};


	void(*Msg)(std::string);

public:
	CImageEditor(const std::string m_sTmpFolder, void(*msg_ptr)(std::string));
	~CImageEditor();

	IC const bool IsImgLoaded() { return m_bIsImageLoaded; }
	IC void SetImgLoaded(const bool m_bValue) { m_bIsImageLoaded = m_bValue; }

	virtual void Load(const std::string m_sImgName);
	virtual void Save(const std::string m_sImageSavePath);

	virtual void SetImageBrighness(const int m_ipercent, const double m_icontrast = 1);
	virtual void SetGaussianBlur(const int m_istep);
	virtual void SetImageRotation(const int m_iAngle);
	virtual void SetImageErode(const int m_istep);
	virtual void SetGrayScale();

	virtual void Undo();
	virtual void Redo();

	IC const bool IsImageHasChanges() { return m_vIUndoRedo.size() > 1 && m_iTmpImgIdx>0; }

	IC const wchar_t* GetTmpImageWchar_t() { return m_wchTmpFilePath.c_str(); }

	virtual void Clear();

	IC const bool CanUndo() { return m_iTmpImgIdx > 0; }
	IC const bool CanRedo() { return m_iTmpImgIdx < m_vsTmpImgFilenames.size() - 1; }

private:
	virtual void CreateUndoRedo();
};