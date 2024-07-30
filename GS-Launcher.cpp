// GS-Launcher.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <string>
#include <Windows.h>
#include <tlhelp32.h>
#include <thread>

void runCommand1(const std::string& command1) {
    system(command1.c_str());
    std::cout << "ran fortnite game \n sleeping for 15 secs gang" << std::endl;
    Sleep(15000);
}

DWORD GetProcessID(const std::wstring& processName) {
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to take process snapshot: " << GetLastError() << std::endl;
        return 0;
    }

    if (Process32First(hSnap, &pe)) {
        do {
            if (processName == pe.szExeFile) {
                CloseHandle(hSnap);
                return pe.th32ProcessID;
            }
        } while (Process32Next(hSnap, &pe));
    }

    CloseHandle(hSnap);
    std::wcerr << L"Process not found: " << processName << std::endl;
    return 0;
}

bool InjectDLL(DWORD processID, const std::wstring& dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (hProcess == NULL) {
        std::wcerr << L"Failed to open process: " << GetLastError() << std::endl;
        return false;
    }

    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, (dllPath.length() + 1) * sizeof(wchar_t), MEM_COMMIT, PAGE_READWRITE);
    if (pRemoteMemory == NULL) {
        std::wcerr << L"Failed to allocate memory in remote process: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return false;
    }

    if (!WriteProcessMemory(hProcess, pRemoteMemory, dllPath.c_str(), (dllPath.length() + 1) * sizeof(wchar_t), NULL)) {
        std::wcerr << L"Failed to write memory in remote process: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, pRemoteMemory, 0, NULL);
    if (hThread == NULL) {
        std::wcerr << L"Failed to create remote thread: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);

    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return true;
}

/*
  TODO:
  ~ Try to use something OTHER than threads
  ~ Work on injection. Uses the same way ExtremeInjector's "Standard Injection" is. It works so why change it.
  ~ Clean up code a bit
  ~ Find the PERFECT time to inject into the game (Or add a check into the DLL to see if Viewport is init.)
*/

int main() {
    std::string command1 = "CMD /c \"C:\\Users\\MAC\\Desktop\\11.31\\FortniteGame\\Binaries\\Win64\\FortniteClient-Win64-Shipping.exe "
        "-epicapp=Fortnite -epicenv=Prod -epiclocale=en-us -epicportal -nobe -fromfl=eac "
        "-fltoken=919348d6add4c4c7c7507e61 -skippatchcheck\"";
    std::string path2 = "C:\\Users\\MAC\\Desktop\\11.31\\FortniteGame\\Binaries\\Win64\\FortniteClient-Win64-Shipping_BE.exe";
    std::string command2 = "start \"\" \"" + path2 + "\"";
    std::string path3 = "C:\\Users\\MAC\\Desktop\\11.31\\FortniteGame\\Binaries\\Win64\\FortniteLauncher.exe";
    std::string command3 = "start \"\" \"" + path3 + "\"";
    
    std::wstring dllPath = L"C:\\Users\\MAC\\Desktop\\GS.dll";

    system(command3.c_str());
    std::cout << "ran fortnitelauncher" << std::endl;
    system(command2.c_str());
    std::cout << "ran the bullshit battleye" << std::endl;

    std::thread t(runCommand1, command1);
    t.detach(); 
    std::cout << "ran fortnite game \n sleeping for 120 secs gang" << std::endl;

    Sleep(10000);
    std::cout << "1" << std::endl; // nothing
    Sleep(10000);
    std::cout << "2" << std::endl; // nothing
    Sleep(10000);
    std::cout << "3" << std::endl; // nothing
    Sleep(10000);
    std::cout << "4" << std::endl; // nothing
    Sleep(10000);
    std::cout << "5" << std::endl; // Game Poster & start of loading
    Sleep(10000);
    std::cout << "6" << std::endl; // Loading (not initilized the Viewport yet)
    Sleep(10000);
    std::cout << "7" << std::endl; // Loading (not initilized the Viewport yet)
    Sleep(10000);
    std::cout << "8" << std::endl; // Loading (not initilized the Viewport yet)
    Sleep(10000);
    std::cout << "9" << std::endl; // Loading (not initilized the Viewport yet)
    Sleep(10000);
    std::cout << "10" << std::endl; // Loading (not initilized the Viewport yet)
    Sleep(10000);
    std::cout << "11" << std::endl; // Loading (not initilized the Viewport yet) ???
    Sleep(10000);
    std::cout << "12" << std::endl; // slow ass i3 fianlly loaded the viewport but the dll crashes because fuck it i guess

    std::cout << "fud injection in progress..." << std::endl;

    DWORD processID = GetProcessID(L"FortniteClient-Win64-Shipping.exe");
    
    Sleep(5000);

    if (processID) {
        if (InjectDLL(processID, dllPath)) {
            std::wcout << L"Successfully injected DLL!" << std::endl;
        }
        else {
            std::wcerr << L"Failed to inject DLL." << std::endl;
        }
    }
    else {
        std::wcerr << L"Process not found." << std::endl;
    }


    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
