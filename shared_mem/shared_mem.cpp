#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define SHM_NAME "Local\\MySharedMemory"
#define SEM_NAME "Local\\MySemaphore"
#define SHM_SIZE sizeof(int)

int main() {
    std::srand((unsigned)time(NULL)); //generator pentru numere random

    // Creare / deschidere memorie partajată
    HANDLE hMap = CreateFileMappingA(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
        0, SHM_SIZE, SHM_NAME
    );

    if (!hMap) {
        std::cout << "Eroare CreateFileMapping\n";
        return 1;
    }
    //flag pentru primul proces
    bool first = (GetLastError() != ERROR_ALREADY_EXISTS);

    //pointer catre memoria partajata
    int* sharedNumber = (int*)MapViewOfFile(
        hMap, FILE_MAP_ALL_ACCESS, 0, 0, SHM_SIZE
    );

    if (!sharedNumber) {
        std::cout << "Eroare MapViewOfFile\n";
        return 1;
    }
    //initializare facuta de primul proces
    if (first) {
        *sharedNumber = 1;
        std::cout << "\nPrimul proces: initializez counter = 1\n";
    } else {
        std::cout << "\nMemoria partajata existenta, valoare curenta = " << *sharedNumber << "\n";
    }

    // Semafor care asigura activitatea unui singur proces simultan
    HANDLE hSem = CreateSemaphoreA(NULL, 1, 1, SEM_NAME);
    if (!hSem) {
        std::cout << "Eroare CreateSemaphore\n";
        return 1;
    }

    while (true) {
        WaitForSingleObject(hSem, INFINITE);
        //conditie de oprire
        if (*sharedNumber > 1000) {
            ReleaseSemaphore(hSem, 1, NULL);
            break;
        }
        //procesul citeste numarul curent din memorie
        std::cout << "\nPID " << GetCurrentProcessId()
                  << " citeste: " << *sharedNumber << "\n";

        // aruncam moneda: 1 sau 2
        int coin = (rand() % 2) + 1;
        std::cout << "(Se arunca banul pentru urmatorul pas: " << coin << ")\n";

        // cât timp cade 2 scriem în memorie
        while (coin == 2 && *sharedNumber <= 1000) {
            std::cout << "\nPID " << GetCurrentProcessId()
                      << " scrie: " << *sharedNumber;
            //incrementam numarul
            (*sharedNumber)++;
            std:: cout <<" (valoare curenta devine: " << *sharedNumber << " )\n";
            Sleep(10);
           // reluam aruncarea monede
            coin = (rand() % 2) + 1;
            std::cout << "(Se arunca banul pentru urmatorul pas: " << coin << ")\n";

        }

        ReleaseSemaphore(hSem, 1, NULL);   //eliberare semafor
        Sleep(20);
    }

    std::cout << "\nPID " << GetCurrentProcessId() << " a terminat executia.\n";
    //curatam resursele folosite
    UnmapViewOfFile(sharedNumber);
    CloseHandle(hMap);
    CloseHandle(hSem);

    return 0;
}
