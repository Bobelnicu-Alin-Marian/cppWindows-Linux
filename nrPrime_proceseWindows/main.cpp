#include <windows.h>
#include <iostream>
#include <vector>
#include <sstream>

int main() {
    const int numProcese = 10;
    const int interval = 1000;

    HANDLE hRead[numProcese], hWrite[numProcese];

    for(int i=0; i<numProcese; ++i) {
        SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
        if(!CreatePipe(&hRead[i], &hWrite[i], &sa, 0)) {
            std::cerr << "Eroare la CreatePipe\n";
            return 1;
        }
    }

    PROCESS_INFORMATION pi[numProcese];
    STARTUPINFO si[numProcese];

    for(int i=0; i<numProcese; ++i) {
        ZeroMemory(&si[i], sizeof(STARTUPINFO));
        si[i].cb = sizeof(STARTUPINFO);
        si[i].hStdOutput = hWrite[i]; // copilul va scrie în pipe
        si[i].dwFlags |= STARTF_USESTDHANDLES;

        ZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));

        int start = i * interval;
        int end = start + interval;

        std::stringstream cmd;
        cmd << "child.exe " << start << " " << end;

        if(!CreateProcess(NULL,
                          const_cast<char*>(cmd.str().c_str()),
                          NULL, NULL, TRUE,
                          0, NULL, NULL,
                          &si[i], &pi[i])) {
            std::cerr << "Eroare la CreateProcess\n";
            return 1;
        }

        CloseHandle(hWrite[i]); // procesul principal nu scrie în pipe
    }

    // Citim datele de la copii
    for(int i=0; i<numProcese; ++i) {
        int n;
        DWORD readBytes;
        while(ReadFile(hRead[i], &n, sizeof(int), &readBytes, NULL) && readBytes > 0)
            std::cout << n << " ";
        CloseHandle(hRead[i]);
    }

    std::cout << std::endl;

    // Așteptăm finalizarea copiilor
    for(int i=0; i<numProcese; ++i) {
        WaitForSingleObject(pi[i].hProcess, INFINITE);
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }

    return 0;
}

