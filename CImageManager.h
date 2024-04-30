#pragma once


#include <iostream>
#include <windows.h>
#include <gdiplus.h>
#include "resource\\base_defines.h"
#include <vector>


class CImageManager {


	// ��������� ��������� ����������
	struct EImageParams {
		int x{}, y{}, hight{}, width{};

		void Reset() {
			x = NULL; y = NULL; hight = NULL; width = NULL;
		}

		//-- VlaGan: ��� ������ ������������� ���������
		//-- � ������-������� ������ ������� ����
		void SetupImage(RECT hWndRect, Gdiplus::Image* img) {
			hight = img->GetHeight();
			width = img->GetWidth();

			const double ImgRel = width / hight;

			if (hWndRect.right - hWndRect.left > width)
				x = ((hWndRect.right - hWndRect.left) - width) / 2;
			else {
				while (hWndRect.right - hWndRect.left < width)
					width -= (double)(5.f * ImgRel);
				x = ((hWndRect.right - hWndRect.left) - width) / 2;
			}

			if (hWndRect.bottom - hWndRect.top > hight)
				y = CW_TOOLTIPBTN_HIGHT + ((hWndRect.bottom - hWndRect.top- CW_TOOLTIPBTN_HIGHT) - hight) / 2;
			else {
				while (hWndRect.bottom - hWndRect.top < hight)
					hight -= (double)(5.f * ImgRel);
				y = CW_TOOLTIPBTN_HIGHT + ((hWndRect.bottom - hWndRect.top- CW_TOOLTIPBTN_HIGHT) - hight) / 2;
			}
		}
	};

private:
	Gdiplus::GdiplusStartupInput m_GdiPlusStandartInput;
	ULONG_PTR m_GdiPlusToken;
	Gdiplus::Image* image;
	EImageParams m_ImgParams;

	HDC hdc;
	PAINTSTRUCT ps;
	HWND m_OwnerWnd;

	bool m_bImageLoaded{};

	// �������� �� ���� � ������������ �-� ����
	void(*Msg_Ptr)(std::string);


	// ����� �� � ������ ������
	//std::vector<Gdiplus::Image*> m_vIUndo{};

	// ����� �� � ���������� ������
	//std::vector<Gdiplus::Image*> m_vIRedo{};

public:
	CImageManager(void(*Msg)(std::string));
	~CImageManager();

	virtual void Load(HWND &hWnd, const wchar_t* m_wchFilePath);
	IC bool IsLoaded() {return m_bImageLoaded;}

	virtual void Clear();

	IC HDC* GetHDC() { return &hdc; }
	IC PAINTSTRUCT* GetPS() { return &ps; }

};

