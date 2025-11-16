#include <windows.h>
#include <iostream>
#include <vector>
#include <cstdlib>

bool estePrim(int n) {
    if(n < 2) return false;
    for(int i = 2; i*i <= n; ++i)
        if(n % i == 0) return false;
    return true;
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        std::cerr << "Parametri insuficienti\n";
        return 1;
    }

    int start = std::atoi(argv[1]);
    int end = std::atoi(argv[2]);

    std::vector<int> prime;
    for(int n = start; n < end; ++n)
        if(estePrim(n))
            prime.push_back(n);

    DWORD written;
    for(int n : prime)
        WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), &n, sizeof(int), &written, NULL);

    return 0;
}

