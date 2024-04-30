//--------------------------------------
//-- VlaGan: Менеджер вибору файлів
//--------------------------------------

#include "CFileManager.h"

constexpr auto m_vchAppDataFolder = "\\__appdata__";
constexpr auto m_fTmpFolder = "\\__appdata__\\tmp";

const std::vector<std::string> m_vsFileTypes = {".png", ".jpg"};

CFileManager::CFileManager() {

    GetExeDirection(m_dwBufferSize, m_tchBuffer);
    m_sExePath.clear();
    m_sImgTmpPath.clear();

    size_t buff1pos{}, buff2pos{};
    while (m_tchBuffer[buff1pos] != '\0') {
        m_tchBufferImgTmp[buff1pos] = m_tchBuffer[buff1pos];
        m_sExePath += m_tchBuffer[buff1pos];
        m_sImgTmpPath += m_tchBuffer[buff1pos++];
    }
    while (m_fTmpFolder[buff2pos] != '\0') {
        m_tchBufferImgTmp[buff1pos++] = m_fTmpFolder[buff2pos];
        m_sImgTmpPath += m_fTmpFolder[buff2pos];
        if(m_fTmpFolder[buff2pos++] == '\\')
            if (!DirExist(m_tchBufferImgTmp))
                m_iImgTmpDir = _mkdir(m_sImgTmpPath.c_str());
    }
    m_dwBufferSizeImgTmp = buff1pos;
    buff1pos = 0;

    //if ( !DirExist((LPCWSTR)(m_sExePath + m_vchAppDataFolder).c_str()) )
    if(!DirExist(m_tchBufferImgTmp))
        m_iImgTmpDir = _mkdir(m_sImgTmpPath.c_str());

}

void CFileManager::FileOpenWriteLPCWSTRtoString() {
    size_t pos{};
    m_sCurrFilePath.clear();
    while (ofn.lpstrFile[pos] != '\0')
        m_sCurrFilePath += ofn.lpstrFile[pos++];
}

void CFileManager::FileOpenWriteLPCWSTRtoWCHAR() {
    size_t pos{};
    m_wsCurrFilePath.clear();
    while (ofn.lpstrFile[pos] != '\0') {
        m_wchCurrFilePath += ofn.lpstrFile[pos];
        m_wsCurrFilePath += ofn.lpstrFile[pos++];
    }
}


void CFileManager::ChooseFilePath(HWND owner) {
    WCHAR szFileName[MAX_PATH]{};

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = (LPCWSTR)L"Image Files (*.jpg)\0*.jpg\0Png files (*.png)\0*.png\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; //OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = (LPCWSTR)L"*.png";

    GetOpenFileName(&ofn);
    
    FileOpenWriteLPCWSTRtoString();
    FileOpenWriteLPCWSTRtoWCHAR();
}



void CFileManager::FileSaveWrite() {
    size_t pos{};
    m_sImgSavePath.clear();
    while (sfn.lpstrFile[pos] != '\0')
        m_sImgSavePath += sfn.lpstrFile[pos++];
}

void CFileManager::ChooseSaveFilePath(HWND owner) {
    WCHAR szFileName[MAX_PATH]{};

    ZeroMemory(&sfn, sizeof(sfn));

    sfn.lStructSize = sizeof(sfn);
    sfn.hwndOwner = owner;
    sfn.lpstrFilter = (LPCWSTR)L"Image Files (*.jpg)\0*.jpg\0Png files (*.png)\0*.png\0All Files (*.*)\0*.*\0";
    sfn.lpstrFile = szFileName;
    sfn.nMaxFile = MAX_PATH;
    //sfn.lpstrFileTitle = NULL;
    //sfn.nMaxFileTitle = 0;
    //sfn.lpstrInitialDir = NULL;
    sfn.Flags = OFN_EXPLORER;// | OFN_HIDEREADONLY; //OFN_FILEMUSTEXIST
    sfn.lpstrDefExt = NULL; //(LPCWSTR)L"*.png";

    GetSaveFileName(&sfn);

    FileSaveWrite();
}

const bool CFileManager::IsImageSaveFilePath() const {
    return string_find_type(m_sImgSavePath, m_vsFileTypes);
}


void CFileManager::Clear() {

    ZeroMemory(&sfn, sizeof(sfn));
    ZeroMemory(&ofn, sizeof(ofn));

    m_sImgSavePath.clear();
    m_sCurrFilePath.clear();
    m_wsCurrFilePath.clear();
    m_wchCurrFilePath = '\0';
    m_dwBufferSize = NULL;
    m_dwBufferSizeImgTmp = NULL;
    m_sImgSavePath.clear();
}
