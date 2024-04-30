// GUI Photo Editor.cpp : Определяет точку входа для приложения.
//

#include "CFileManager.h"
#include "CLog.h"
#include "resource\\framework.h"
#include "resource\\resource.h"
#include "CImageManager.h"
#include "CImageEditor.h"

#include <CommCtrl.h>
//#include <gdiplus.h>

#ifdef TIME_DBG
#include "..\\Multithtreading Test\timer.h"
Timer g_timer;
#endif

//---------------------------- Filemanager ------------------------
CFileManager g_filemanager;
//-----------------------------------------------------------------
// 
//------------------------------- LOG -----------------------------
CLog g_log;
template <typename type> static void Msg(type text, CLog* g_log = &g_log) {
    g_log->write(text);
}

static void Msg(std::string text) {
    g_log.write(text);
}
//-----------------------------------------------------------------
// 
//-------------------------- CImageManager ------------------------
CImageManager g_imagemanager(Msg);
//-----------------------------------------------------------------
// 
//-------------------------- CImageManager ------------------------
CImageEditor g_imageditor(g_filemanager.GetImgTmpPath(), Msg);
//-----------------------------------------------------------------
//void DoSomethingOpenCV();



//-- VlaGan: різні ф-ї
const bool ShowNotLoadedMsgBox(HWND hWnd);
void MakeLoad(HWND hWnd);



HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                 
WCHAR szWindowClass[MAX_LOADSTRING];            


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ChooseBr(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    GaussianBlur(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ImageRotation(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ImageErode(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Gdiplus::GdiplusStartupInput m_GdiPlusStandartInput;
    ULONG_PTR m_GdiPlusToken;
    Gdiplus::GdiplusStartup(&m_GdiPlusToken, &m_GdiPlusStandartInput, nullptr);
    Msg("Programm started succesfully!");

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GUIPHOTOEDITOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUIPHOTOEDITOR));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    Gdiplus::GdiplusShutdown(m_GdiPlusToken);

    return (int) msg.wParam;
}

WNDCLASSEXW wcex;
ATOM MyRegisterClass(HINSTANCE hInstance)
{


    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUIPHOTOEDITOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GUIPHOTOEDITOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


HWND hWndGeneral;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   hWndGeneral = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      //CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
      CW_USEDEFAULT, 0, CW_BASEWNDWIDTH, CW_BASEWNDHIGHT, nullptr, nullptr, hInstance, nullptr);

   const int nButtons = 6;

   TBBUTTON tbb[nButtons];
   
   //-- LOAD TOOLTIP
   tbb[0].iBitmap = STD_FILENEW;
   tbb[0].idCommand = ID_TOOLTIP_LOAD;     
   tbb[0].fsState = TBSTATE_ENABLED;  
   tbb[0].fsStyle = TBSTYLE_BUTTON; 
   tbb[0].iString = 0;                
   
   //-- UNDO TOOLTIP
   tbb[1].iBitmap = STD_UNDO;
   tbb[1].idCommand = ID_TOOLTIP_UNDO;
   tbb[1].fsState = TBSTATE_ENABLED;
   tbb[1].fsStyle = TBSTYLE_BUTTON;
   tbb[1].iString = 0;
   
   //-- REDO TOOLTIP
   tbb[2].iBitmap = STD_REDOW;
   tbb[2].idCommand = ID_TOOLTIP_REDO;
   tbb[2].fsState = TBSTATE_ENABLED;
   tbb[2].fsStyle = TBSTYLE_BUTTON;
   tbb[2].iString = 0;

   tbb[3].iBitmap = STD_FILESAVE;
   tbb[3].idCommand = ID_TOOLTIP_SAVE;
   tbb[3].fsState = TBSTATE_ENABLED;
   tbb[3].fsStyle = TBSTYLE_BUTTON;
   tbb[3].iString = 0;

   tbb[4].iBitmap = STD_DELETE;
   tbb[4].idCommand = ID_TOOLTIP_DELETE;
   tbb[4].fsState = TBSTATE_ENABLED;
   tbb[4].fsStyle = TBSTYLE_BUTTON;
   tbb[4].iString = 0;


   tbb[5].fsStyle = TBSTYLE_SEP;
   HWND htb = CreateToolbarEx(hWndGeneral,(DWORD)WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS, (UINT)ID_TOOLBAR,              
       (int)15,(HINSTANCE)HINST_COMMCTRL, (UINT)IDB_STD_SMALL_COLOR, (TBBUTTON*)tbb, (int)nButtons,
       48, 48,30, 30,sizeof(TBBUTTON));

   if (!hWndGeneral)
      return FALSE;

   ShowWindow(hWndGeneral, nCmdShow);
   UpdateWindow(hWndGeneral);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


    switch (message)
    {

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;

        case IDM_EXIT: {
            if (g_imageditor.IsImageHasChanges()) {
                int msgboxID = MessageBox(hWnd, L"Існують не збережені зміни!\nВийти з програми без їх збереження?",
                    L"Не збережені зміни!", MB_ICONEXCLAMATION | MB_YESNO);

                if (msgboxID != IDYES)
                    break;
            }
            DestroyWindow(hWnd);}
            break;

        case ID_FILECHOOSE_DIR: {
            if (g_imageditor.IsImageHasChanges() && g_imageditor.IsImgLoaded()) {
                int msgboxID = MessageBox(hWnd, L"Попереднє фото має не збережені зміни!\nВідмінити їх і завантажити нове?",
                    L"Відмінити зміни?", MB_ICONEXCLAMATION | MB_YESNO);

                if (msgboxID != IDYES)
                    break;
            }
            MakeLoad(hWnd);
        }
        break;

        case ID_SAVE_FILE_AS: {
            if (ShowNotLoadedMsgBox(hWnd))
                break;

            g_filemanager.ChooseSaveFilePath(hWnd);
            if (g_filemanager.IsImageSaveFilePath() && g_filemanager.GetImgSavePath().size()) {
                Msg("Save file path/name = " + g_filemanager.GetImgSavePath());

                if (g_filemanager.GetFilePath() != g_filemanager.GetImgSavePath() && g_filemanager.IsImageSaveFilePath()) {

                    std::ifstream file_exist(g_filemanager.GetImgSavePath());
                    if (file_exist.good()) {
                        int msgboxID = MessageBox(hWnd, L"Замінити файл?\nОбраний вами файл буде замінено!",
                            L"Замінити файл?", MB_ICONEXCLAMATION | MB_YESNO);

                        if (msgboxID == IDYES)
                            g_imageditor.Save(g_filemanager.GetImgSavePath());
                    }
                    else
                        g_imageditor.Save(g_filemanager.GetImgSavePath());
                }
            }
           

        }
        break;

        case ID_SAVE_FILE: {
            if (ShowNotLoadedMsgBox(hWnd))
                break;

            std::string save = g_filemanager.GetFilePath();
            if (save.size()) {
                save.insert(save.size() - 4, "_redacted");
                g_imageditor.Save(save);
            }
        }
        break;


        case ID_CLEAR_FOTO: {
            if (ShowNotLoadedMsgBox(hWnd))
                break;
            if (g_imageditor.IsImageHasChanges()) {
                int msgboxID = MessageBox(hWnd, L"Попереднє фото має не збережені зміни!\nВсе одно видалити фото?",
                    L"Відмінити зміни?", MB_ICONEXCLAMATION | MB_YESNO);

                if (msgboxID != IDYES)
                    break;
            }
            g_imagemanager.Clear();
            g_filemanager.Clear();
            g_imageditor.Clear();
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        //-- UNDO в розділі правки
        case ID_UNDO: {
            if (ShowNotLoadedMsgBox(hWnd))
                break;
            if (!g_imageditor.CanUndo()) {
                Msg("CANT UNDO!");
                break;
            }
            g_imageditor.Undo();
            g_imagemanager.Load(hWnd, g_imageditor.GetTmpImageWchar_t());
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        //-- REDO в розділі правки
        case ID_REDO:
        {
            if (ShowNotLoadedMsgBox(hWnd))
                break;
            if (!g_imageditor.CanRedo()) {
                Msg("CANT REDO!");
                break;
            }
            g_imageditor.Redo();
            g_imagemanager.Load(hWnd, g_imageditor.GetTmpImageWchar_t());
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        //-- UNDO на панелі
        case ID_TOOLTIP_UNDO: {
            if (ShowNotLoadedMsgBox(hWnd))
                break;
            if (!g_imageditor.CanUndo()) {
                Msg("CANT UNDO!");
                break;
            }
            g_imageditor.Undo();
            g_imagemanager.Load(hWnd, g_imageditor.GetTmpImageWchar_t());
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        //-- REDO на панелі
        case ID_TOOLTIP_REDO: {
            if (ShowNotLoadedMsgBox(hWnd))
                break;
            if (!g_imageditor.CanRedo()) {
                Msg("CANT REDO!");
                break;
            }
            g_imageditor.Redo();
            g_imagemanager.Load(hWnd, g_imageditor.GetTmpImageWchar_t());
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        case ID_TOOLTIP_LOAD: {
            if (g_imageditor.IsImageHasChanges() && g_imageditor.IsImgLoaded()) {
                int msgboxID = MessageBox(hWnd, L"Попереднє фото має не збережені зміни!\nВідмінити їх і завантажити нове?",
                    L"Відмінити зміни?", MB_ICONEXCLAMATION | MB_YESNO);

                if (msgboxID != IDYES)
                    break;
            }
            MakeLoad(hWnd);
        }
        break;

        case ID_TOOLTIP_SAVE: {
            if (ShowNotLoadedMsgBox(hWnd))
                break;
           
                std::string save = g_filemanager.GetFilePath();
                if (save.size()) {
                    save.insert(save.size() - 4, "_redacted");
                    g_imageditor.Save(save);
                }
        }
        break;

        case ID_TOOLTIP_DELETE: {
            if (ShowNotLoadedMsgBox(hWnd))
                break;
            if (g_imageditor.IsImageHasChanges()) {
                int msgboxID = MessageBox(hWnd, L"Попереднє фото має не збережені зміни!\nВсе одно видалити фото?",
                    L"Відмінити зміни?", MB_ICONEXCLAMATION | MB_YESNO);

                if (msgboxID != IDYES)
                    break;
            }
            g_imagemanager.Clear();
            g_filemanager.Clear();
            g_imageditor.Clear();
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        case ID_IMAGEMOD_BRIGHNESS:
        {
            if (ShowNotLoadedMsgBox(hWnd))
                break;
            return DialogBox(hInst, MAKEINTRESOURCE(IDD_CHOOSE_BR), hWnd, ChooseBr);
        }
        break;


        case ID_IMAGEMOD_GAUSSBLUR: {
            if (ShowNotLoadedMsgBox(hWnd))
                break;
            return DialogBox(hInst, MAKEINTRESOURCE(IDD_CHOOSE_GAUSSIAN_BLUR), hWnd, GaussianBlur);
        }
        break;

        case ID_IMAGEMOD_ROTATION: {
            if (ShowNotLoadedMsgBox(hWnd))
                break;
            return DialogBox(hInst, MAKEINTRESOURCE(IDD_CHOOSE_ROTATION), hWnd, ImageRotation);
        }
        break;

        case ID_IMAGEMOD_ERODE: {
            if (ShowNotLoadedMsgBox(hWnd))
                break;
            return DialogBox(hInst, MAKEINTRESOURCE(IDD_CHOOSE_ERODE), hWnd, ImageErode);
        }
        break;

        case ID_IMAGEMOD_GRAYSCALE: {
            if (ShowNotLoadedMsgBox(hWnd))
                break;
            g_imageditor.SetGrayScale();
            g_imagemanager.Load(hWnd, g_imageditor.GetTmpImageWchar_t());
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;


    case WM_PAINT:{
        if (g_filemanager.GetFilePath().size())
            g_imagemanager.Load(hWnd, !g_imageditor.IsImageHasChanges() ? g_filemanager.GetImgPathWchar_t() : g_imageditor.GetTmpImageWchar_t());

        else {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
    }
    break;

    case WM_SIZE:
    {        
        if (g_filemanager.GetFilePath().size())
            g_imagemanager.Load(hWnd, !g_imageditor.IsImageHasChanges() ? g_filemanager.GetImgPathWchar_t() : g_imageditor.GetTmpImageWchar_t());
    }
    break;


    // вихід з програми
    case WM_DESTROY:{
        Msg("Destroying window!");

#ifdef TIME_DBG
        Msg(GetWorkTimeStr(g_timer.elapsed()));
#endif // TIME_DBG

        PostQuitMessage(0);
        break;
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}


//-- каллбек на about
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}



//-- Каллбек вікна яскравості і контсрасту
INT_PTR CALLBACK ChooseBr(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hWndtrack = GetDlgItem(hDlg, IDC_SCROLLBARV);
    HWND hWndtrackC = GetDlgItem(hDlg, IDC_CONTRAST);

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG: {
        SendMessage(hWndtrack, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(NULL, 100));  
        SendMessage(hWndtrack, TBM_SETPAGESIZE, 0, (LPARAM)4);                  
        SendMessage(hWndtrack, TBM_SETSEL, (WPARAM)FALSE, (LPARAM)MAKELONG(NULL, NULL));
        SendMessage(hWndtrack, TBM_SETPOS,(WPARAM)TRUE,(LPARAM)NULL);

        SendMessage(hWndtrackC, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 200));
        SendMessage(hWndtrackC, TBM_SETPAGESIZE, 0, (LPARAM)4);
        SendMessage(hWndtrackC, TBM_SETSEL, (WPARAM)FALSE, (LPARAM)MAKELONG(0, 100));
        SendMessage(hWndtrackC, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)100);

        return (INT_PTR)TRUE;
    }
        
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDOK)
        {
            //InvalidateRect(hWndGeneral, NULL, TRUE);
            const int br = SendMessage(hWndtrack, TBM_GETPOS, 0, 0);
            Msg("BRIGHNESS = " + std::to_string(br));
            const double cont = (double)(SendMessage(hWndtrackC, TBM_GETPOS, 0, 0) / 100.f);
            Msg("CONTRAST = " + std::to_string(cont));
            g_imageditor.SetImageBrighness(br, cont);
            g_imagemanager.Load(hWndGeneral, g_imageditor.GetTmpImageWchar_t());
            InvalidateRect(hWndGeneral, NULL, TRUE);

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
    }
    break;  

    }
    return (INT_PTR)FALSE;
}


INT_PTR CALLBACK GaussianBlur(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hWndtrack = GetDlgItem(hDlg, IDC_TASKBAR_BLUR);

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG: {
        SendMessage(hWndtrack, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(3, 5));
        SendMessage(hWndtrack, TBM_SETPAGESIZE, 0, (LPARAM)4);
        SendMessage(hWndtrack, TBM_SETSEL, (WPARAM)FALSE, (LPARAM)MAKELONG(NULL, NULL));
        SendMessage(hWndtrack, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)3);

        return (INT_PTR)TRUE;
    }

    case WM_COMMAND: {
        if (LOWORD(wParam) == IDOK)
        {
            const int step = SendMessage(hWndtrack, TBM_GETPOS, 0, 0);
            Msg("GAUSSIAN BLUR STEP = " + std::to_string(step));
            g_imageditor.SetGaussianBlur(step == 4 ? 3: step);
            g_imagemanager.Load(hWndGeneral, g_imageditor.GetTmpImageWchar_t());
            InvalidateRect(hWndGeneral, NULL, TRUE);

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
    }
                   break;

    }
    return (INT_PTR)FALSE;
}


//-- VlaGan: поворот проти годинникової стрілки, вважається, що стартовий кут - 180 град
INT_PTR CALLBACK ImageRotation(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hWndtrack = GetDlgItem(hDlg, ID_IMAGEMOD_ROTATION);

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG: {
        SendMessage(hWndtrack, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 360));
        SendMessage(hWndtrack, TBM_SETPAGESIZE, 0, (LPARAM)4);
        SendMessage(hWndtrack, TBM_SETSEL, (WPARAM)FALSE, (LPARAM)MAKELONG(NULL, NULL));
        SendMessage(hWndtrack, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)180);

        return (INT_PTR)TRUE;
    }

    case WM_COMMAND: {
        if (LOWORD(wParam) == IDOK)
        {
            const int m_iAngle = SendMessage(hWndtrack, TBM_GETPOS, 0, 0);
            Msg("ROTATION ANGLE = " + std::to_string(m_iAngle));
            g_imageditor.SetImageRotation(m_iAngle);
            g_imagemanager.Load(hWndGeneral, g_imageditor.GetTmpImageWchar_t());
            InvalidateRect(hWndGeneral, NULL, TRUE);

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
    }
                   break;

    }
    return (INT_PTR)FALSE;
}


INT_PTR CALLBACK ImageErode(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hWndtrack = GetDlgItem(hDlg, ID_IMAGEMOD_ERODE);

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG: {
        SendMessage(hWndtrack, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(3, 5));
        SendMessage(hWndtrack, TBM_SETPAGESIZE, 0, (LPARAM)4);
        SendMessage(hWndtrack, TBM_SETSEL, (WPARAM)FALSE, (LPARAM)MAKELONG(NULL, NULL));
        SendMessage(hWndtrack, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)3);

        return (INT_PTR)TRUE;
    }

    case WM_COMMAND: {
        if (LOWORD(wParam) == IDOK)
        {
            const int step = SendMessage(hWndtrack, TBM_GETPOS, 0, 0);
            Msg("IMAGE ERODE STEP = " + std::to_string(step));
            g_imageditor.SetImageErode(step);
            g_imagemanager.Load(hWndGeneral, g_imageditor.GetTmpImageWchar_t());
            InvalidateRect(hWndGeneral, NULL, TRUE);

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
    }
                   break;

    }
    return (INT_PTR)FALSE;
}


const bool ShowNotLoadedMsgBox(HWND hWnd) {
    if (!g_imageditor.IsImgLoaded()) {
        MessageBox(hWnd, L"Помилка! Ви не завантажили зображення!",
            L"Помилка!", MB_ICONEXCLAMATION | MB_OK);
        return true;
    }
    return false;
}

void MakeLoad(HWND hWnd) {
    g_imagemanager.Clear();
    g_filemanager.Clear();
    g_imageditor.Clear();
    g_filemanager.ChooseFilePath(hWnd);
    Msg("Selected file = " + g_filemanager.GetFilePath());

    //
    if (g_filemanager.GetFilePath().size()) {
        if (!g_imagemanager.IsLoaded()) {
            g_imagemanager.Load(hWnd, g_filemanager.GetImgPathWchar_t());

            //if(!g_imageditor.IsImageHasChanges())
            g_imageditor.Load(g_filemanager.GetFilePath());

            InvalidateRect(hWnd, NULL, TRUE);

        }
        //-- Тут мініхак, інвалідим вікно, чим викликаємо перезапуск цього
        //-- коллбека і сам по собі WM_PAINT
        else
            InvalidateRect(hWnd, NULL, TRUE);

    }
    // якщо ми нічого не виберемо, то і не загрузимо вже
    else
        InvalidateRect(hWnd, NULL, TRUE);
};
