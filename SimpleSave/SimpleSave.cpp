// RaoufiTech on github, still in development will add more functionality later on. hope u enjoy this.

#include <windows.h>
#include <shlobj.h>
#include <gdiplus.h>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <thread>
#include <atomic>

//#pragma comment(lib, "gdiplus.lib")  uncomment this if you don't link with gdiplus during building, I am linking so I can remove this.

using namespace Gdiplus;

std::atomic<bool> running(true);

std::wstring getCurrentTimeAsWString() {
    time_t now = time(0);
    struct tm tstruct;
    wchar_t buf[80];
    localtime_s(&tstruct, &now);
    wcsftime(buf, sizeof(buf) / sizeof(wchar_t), L"%H-%M-%S", &tstruct);
    return std::wstring(buf);
}

std::wstring getDownloadPath() {
    PWSTR path = nullptr;
    if (SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &path) == S_OK) {
        std::wstring downloadPath = std::wstring(path) + L"\\ClipBoardImages";
        CoTaskMemFree(path);
        return downloadPath;
    }
    return L"";
}

void saveBitmapAsImage(HBITMAP hBitmap, const std::wstring& filename, const CLSID& format) {
    Bitmap bitmap(hBitmap, NULL);
    bitmap.Save(filename.c_str(), &format, NULL);
}

void createFolderIfNotExists(const std::wstring& path) {
    SHCreateDirectoryExW(NULL, path.c_str(), NULL);
}

bool addToStartup() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);

    HKEY hKey;
    if (RegOpenKeyW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey) == ERROR_SUCCESS) {
        RegSetValueExW(hKey, L"ClipboardImageSaver", 0, REG_SZ, (BYTE*)exePath, (DWORD)(wcslen(exePath) + 1) * sizeof(wchar_t));
        RegCloseKey(hKey);
        return true;
    }
    return false;
}

std::string hashBitmap(HBITMAP hBitmap) {
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    int bufferSize = bmp.bmWidthBytes * bmp.bmHeight;
    std::vector<BYTE> buffer(bufferSize);

    HDC hdc = GetDC(NULL);
    GetBitmapBits(hBitmap, bufferSize, buffer.data());
    ReleaseDC(NULL, hdc);

    // Simple hash of pixels so that we dont save the same image every second
    std::hash<std::string> hasher;
    return std::to_string(hasher(std::string(buffer.begin(), buffer.end())));
}

void monitorClipboard() {
    std::wstring downloadPath = getDownloadPath();
    createFolderIfNotExists(downloadPath);

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    CLSID pngClsid;
    HRESULT hr = CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &pngClsid);
    if (FAILED(hr)) {
        MessageBoxW(NULL, L"Failed to retrieve CLSID for PNG format!", L"Error", MB_ICONERROR);
        running = false;
        return;
    }

    std::string lastImageHash;

    while (running) {
        if (OpenClipboard(NULL)) {
            if (IsClipboardFormatAvailable(CF_BITMAP)) {
                HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
                if (hBitmap) {
                    std::string currentHash = hashBitmap(hBitmap);
                    if (currentHash != lastImageHash) {
                        std::wstring filename = downloadPath + L"\\" + getCurrentTimeAsWString() + L".png";
                        saveBitmapAsImage(hBitmap, filename, pngClsid);
                        lastImageHash = currentHash;
                    }
                }
            }
            CloseClipboard();
        }
        Sleep(1000);
    }

    GdiplusShutdown(gdiplusToken);
}

void listenForHotkey() {
    if (!RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT, 0x53)) { // CTRL+ALT+S shuts down the app until you rerun the .exe again.
        if (GetLastError() == ERROR_HOTKEY_ALREADY_REGISTERED) {
            MessageBoxW(NULL, L"Hotkey already registered by another instance of EasySave or another application.", L"Info", MB_ICONINFORMATION);
        }
        return;
    }

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY) {
            running = false;
            break;
        }
    }

    UnregisterHotKey(NULL, 1);
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    HANDLE mutex = CreateMutex(NULL, TRUE, L"EasySaveMutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBoxW(NULL, L"Another instance of EasySave is already running.", L"Error", MB_ICONERROR);
        return 1;
    }

    FreeConsole();
    if (!addToStartup()) {
        MessageBoxW(NULL, L"Failed to add to startup!", L"Error", MB_ICONERROR);
        return 1;
    }

    std::thread hotkeyThread(listenForHotkey);
    monitorClipboard();

    hotkeyThread.join();

    ReleaseMutex(mutex);
    return 0;
}
