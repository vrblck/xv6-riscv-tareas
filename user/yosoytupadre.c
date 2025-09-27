#include "user.h"

int main(int argc, char *argv[])
{
    int pid = getpid();
    int ppid = getppid();
    printf("Mi PID: %d\n", pid);
    printf("PID de mi padre: %d\n", ppid);

    printf("\nProbando getancestor(n):\n");
    for(int i = 0; i < 5; i++) {
        int anc = getancestor(i);
        printf("getancestor(%d) = %d\n", i, anc);
        if(anc == -1) break;
    }

    int f = fork();
    if(f == 0) {
        // Proceso hijo
        printf("\n[Hijo] Mi PID: %d, getppid(): %d\n", getpid(), getppid());
        for(int i = 0; i < 5; i++) {
            int anc = getancestor(i);
            printf("[Hijo] getancestor(%d) = %d\n", i, anc);
            if(anc == -1) break;
        }
        exit(0);
    } else {
        wait(0);
    }
    exit(0);
}
