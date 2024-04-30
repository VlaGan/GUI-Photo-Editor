
//-------------------------------------------
//-- VlaGan: CImageEditor - клас-обробник
//-- зображень
//-------------------------------------------

#include "CImageEditor.h"
#include <iostream>
#include <algorithm>


//-- Ну, шо... Треба завезти вже підтримку формату для tmp файлів...

constexpr auto m_sTpmImgName = "\\tmp_file.jpg";

CImageEditor::CImageEditor(const std::string m_sTmpFolder, void(*msg_ptr)(std::string)) {
	m_sCurrentImageFile.clear();
	m_sImageTmpFolder = m_sTmpFolder;

	m_vIUndoRedo.clear();
	m_vsTmpImgFilenames.clear();
	m_wchTmpFilePath.clear();

	size_t pos{};
	while(pos < m_sImageTmpFolder.size())
		m_wchTmpFilePath += m_sImageTmpFolder[pos++]; 
	pos = 0;
	while (pos < strlen(m_sTpmImgName))
		m_wchTmpFilePath += m_sTpmImgName[pos++];

	Msg = msg_ptr;
	m_iTmpImgIdx = 0;
}

CImageEditor::~CImageEditor() {
	m_cvImage.release();// = NULL;
}


void CImageEditor::Load(const std::string m_sImgName) {

	m_sCurrentImageFile = m_sImgName;
	Msg("Current imageditor file = " + m_sCurrentImageFile);

	m_cvImage = cv::imread(m_sCurrentImageFile);

	m_bIsImageLoaded = true;

	if(!(std::find(m_vsTmpImgFilenames.begin(), m_vsTmpImgFilenames.end(), m_sCurrentImageFile) != m_vsTmpImgFilenames.end()))
		m_iSessionNum++;

	if (m_vsTmpImgFilenames.empty())
		m_vsTmpImgFilenames.push_back(m_sCurrentImageFile);

	if (m_vIUndoRedo.empty()) {
		cv::Mat tmp;
		m_cvImage.copyTo(tmp);
		m_vIUndoRedo.push_back(tmp);
	}
}

void CImageEditor::Save(const std::string m_sImageSavePath) {
	cv::imwrite(m_sImageSavePath, m_cvImage);
}

void CImageEditor::Clear() {	
	for (auto& i : m_vIUndoRedo)
		i.release();
	m_vIUndoRedo.clear();
	m_sCurrentImageFile.clear();
	m_bIsImageLoaded = false;
	m_cvImage.release();
	m_cvImage = NULL;
	//m_cvImage.r;
	//m_iTmpImgIdx = NULL;

	m_vsTmpImgFilenames.clear();
	m_iTmpImgIdx = 0;

	//Msg("try to remove tmp file  = " + std::to_string(remove((m_sImageTmpFolder + m_sTpmImgName).c_str())));
}

// тут треба буде ресайз зробити
void CImageEditor::CreateUndoRedo() {
	//if (m_vsTmpImgFilenames.empty())
	//	m_vsTmpImgFilenames.push_back(m_sCurrentImageFile);

	if (m_iTmpImgIdx != m_vsTmpImgFilenames.size() - 1) {
		m_iSessionNum++;
		/*size_t pos = m_vsTmpImgFilenames.size() - 1;
		while (pos != m_iTmpImgIdx + 1) {
			m_vsTmpImgFilenames.pop_back();
			m_vIUndoRedo[pos].release();
			m_vIUndoRedo.pop_back();
			pos--;
		}*/
		m_vsTmpImgFilenames.resize(!m_iTmpImgIdx ? 1 : m_iTmpImgIdx);

		for (int i = m_iTmpImgIdx; i < m_vIUndoRedo.size(); i++)
			m_vIUndoRedo[i].release();
		m_vIUndoRedo.resize(!m_iTmpImgIdx ? 1: m_iTmpImgIdx);
	}

	std::string fname = m_sImageTmpFolder + m_sTpmImgName;
	fname.insert(fname.size() - 4, "_" + std::to_string(m_iSessionNum) + "_" + std::to_string(++m_iTmpImgIdx));
	Msg("Pushed current filename = " + fname);

	m_vsTmpImgFilenames.push_back(fname);

	m_wchTmpFilePath.clear();
	size_t i{};
	while (i < m_vsTmpImgFilenames[m_iTmpImgIdx].size())
		m_wchTmpFilePath += m_vsTmpImgFilenames[m_iTmpImgIdx][i++];
}


void CImageEditor::Undo() {
	if (m_iTmpImgIdx <= 0) {
		Msg("Cant get previous image stage, because m_iTmpImgIdx == 0!");
		return;
	}
	m_iTmpImgIdx--;

	m_wchTmpFilePath.clear();
	size_t i{};
	while (i < m_vsTmpImgFilenames[m_iTmpImgIdx].size())
		m_wchTmpFilePath += m_vsTmpImgFilenames[m_iTmpImgIdx][i++];

	m_cvImage = m_vIUndoRedo[m_iTmpImgIdx];
	Load(m_vsTmpImgFilenames[m_iTmpImgIdx]);
	//m_vIUndoRedo[m_iTmpImgIdx].copyTo(m_cvImage);

}

void CImageEditor::Redo() {
	if (m_iTmpImgIdx == m_vsTmpImgFilenames.size() - 1) {
		Msg("Next image stage does not exist!");
		return;
	}
	m_iTmpImgIdx++;
	m_wchTmpFilePath.clear();
	size_t i{};
	while (i < m_vsTmpImgFilenames[m_iTmpImgIdx].size())
		m_wchTmpFilePath += m_vsTmpImgFilenames[m_iTmpImgIdx][i++];

	m_cvImage = m_vIUndoRedo[m_iTmpImgIdx];
	//m_cvImage.release();
	Load(m_vsTmpImgFilenames[m_iTmpImgIdx]);
}



void CImageEditor::SetImageBrighness(const int m_ipercent, const double m_icontrast) {
	Msg("CImageEditor::SetImageBrighness() has been called!");

	CreateUndoRedo();
	m_cvImage.convertTo(m_cvImage, -1, m_icontrast, m_ipercent);
	cv::Mat undo;
	m_cvImage.copyTo(undo);
	m_vIUndoRedo.push_back(undo);

	cv::imwrite(m_vsTmpImgFilenames[m_iTmpImgIdx], m_cvImage);
}

void CImageEditor::SetGaussianBlur(const int m_istep) {
	Msg("CImageEditor::SetGaussianBlur() has been called!");

	CreateUndoRedo();
	cv::GaussianBlur(m_cvImage, m_cvImage, cv::Size(m_istep, m_istep), 0);
	cv::Mat undo;
	m_cvImage.copyTo(undo);
	m_vIUndoRedo.push_back(undo);

	cv::imwrite(m_vsTmpImgFilenames[m_iTmpImgIdx], m_cvImage);
}

void CImageEditor::SetImageRotation(const int m_iAngle) {
	Msg("CImageEditor::SetImageRotation() has been called!");

	CreateUndoRedo();


	cv::Mat matRotation = cv::getRotationMatrix2D(cv::Point(m_cvImage.rows / 2, m_cvImage.cols / 2), (m_iAngle - 180), 1);
	cv::warpAffine(m_cvImage, m_cvImage, matRotation, m_cvImage.size());
	//undo.release();
	cv::Mat undo;
	m_cvImage.copyTo(undo);
	m_vIUndoRedo.push_back(undo);

	cv::imwrite(m_vsTmpImgFilenames[m_iTmpImgIdx], m_cvImage);

}

void CImageEditor::SetImageErode(const int m_istep) {
	Msg("CImageEditor::SetImageInvert() has been called!");

	CreateUndoRedo();
	cv::erode(m_cvImage, m_cvImage, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(m_istep, m_istep)));

	cv::Mat undo;
	m_cvImage.copyTo(undo);
	m_vIUndoRedo.push_back(undo);

	cv::imwrite(m_vsTmpImgFilenames[m_iTmpImgIdx], m_cvImage);
}


void CImageEditor::SetGrayScale() {
	Msg("CImageEditor::SetGrayScale() has been called!");
	CreateUndoRedo();
	cv::cvtColor(m_cvImage, m_cvImage, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(m_cvImage, m_cvImage);

	cv::Mat undo;
	m_cvImage.copyTo(undo);
	m_vIUndoRedo.push_back(undo);

	cv::imwrite(m_vsTmpImgFilenames[m_iTmpImgIdx], m_cvImage);
}