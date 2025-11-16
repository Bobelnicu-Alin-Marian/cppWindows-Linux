#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <cstring>
#include <cstdlib>

bool estePrim(int n) {
    if(n < 2) return false;
    for(int i = 2; i*i <= n; ++i)
        if(n % i == 0) return false;
    return true;
}

int main() {
    const int numProcese = 10;
    const int interval = 1000;
    int fd[numProcese][2];

    for(int i=0; i<numProcese; ++i)
        pipe(fd[i]);

    for(int i=0; i<numProcese; ++i) {
        pid_t pid = fork();
        if(pid == 0) { // copil
            close(fd[i][0]);
            int start = i*interval;
            int end = start + interval;
            std::vector<int> prime;
            for(int n=start; n<end; ++n)
                if(estePrim(n)) prime.push_back(n);
            for(int n : prime)
                write(fd[i][1], &n, sizeof(int));
            close(fd[i][1]);
            exit(0);
        } else {
            close(fd[i][1]);
        }
    }

    for(int i=0; i<numProcese; ++i) {
        int n;
        while(read(fd[i][0], &n, sizeof(int)) > 0)
            std::cout << n << " ";
        close(fd[i][0]);
    }

    std::cout << std::endl;
    return 0;
}
