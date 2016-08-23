#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    printf("Processo pai principal: %d\n", getpid());
    for(int i = 0; i < 2; i++) {
        int filho_PID = fork();
        if(filho_PID == 0) {
            printf("Processo %d filho de %d \n", getpid(), getppid());
            for(int j = 0; j < 2; j++) {
                int filho_PID2 = fork();
                if(filho_PID2 == 0) {
                    printf("Processo %d filho de %d \n", getpid(), getppid());
                    break;
                } else {
                    wait(NULL);
                }
            }
            break;
        } else {
            wait(NULL);
        }
    }
}
