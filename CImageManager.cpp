//---------------------------------------------
// VlaGan: менеджер роботи з зображеннями, 
// рендер завантаженого зображення 
// в головному вікні програми
//---------------------------------------------
#include "CImageManager.h"
#include <fstream>
#include <string>

CImageManager::CImageManager(void(*Msg)(std::string)) {
	Gdiplus::GdiplusStartup(&m_GdiPlusToken, &m_GdiPlusStandartInput, nullptr);
	hdc = nullptr;
	image = nullptr;
	m_OwnerWnd = nullptr;

	Msg_Ptr = Msg;

	//m_vIUndo.clear();
	//m_vIRedo.clear();
}

CImageManager::~CImageManager() {
	delete this->image;
	image = nullptr;
	Gdiplus::GdiplusShutdown(m_GdiPlusToken);
	hdc = nullptr;
}

void CImageManager::Load(HWND& hWnd, const wchar_t* m_wchFilePath) {

#ifdef RESIZE_DBG
	Msg_Ptr("CImageManager::Load has been called!");
#endif

	// перевіряємо наявність файлу за директорією
	std::ifstream iff(m_wchFilePath);
	if (iff.bad()) {
		Msg_Ptr("Invalid filename m_wchFilePath! Load canceled!");
		return;
	}

	m_OwnerWnd = hWnd;

	hdc = BeginPaint(hWnd, &ps);

	Gdiplus::Graphics graphics(hdc);
	image = new Gdiplus::Image(m_wchFilePath);

	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	m_ImgParams.SetupImage(rcClient, image);

	graphics.DrawImage(image, m_ImgParams.x, m_ImgParams.y, m_ImgParams.width, m_ImgParams.hight);

	//ReleaseDC(hWnd, hdc);
	EndPaint(hWnd, &ps);

	m_bImageLoaded = true;
}

// Мб потім піде під ніж, но поки хай буде
void CImageManager::Clear() {

#ifdef RESIZE_DBG
	//Msg_Ptr("CImageManager::Clear() has been called!");
#endif

	m_ImgParams.Reset();
	delete image;
	image = nullptr;
	hdc = nullptr;
	m_OwnerWnd = nullptr;
	m_bImageLoaded = false;


	// при релоаді чистимо всі правки повернути/відновити
	/*for (auto& i : m_vIUndo)
		delete i;
	for (auto& i : m_vIRedo)
		delete i;
	m_vIUndo.clear();
	m_vIRedo.clear();*/
}