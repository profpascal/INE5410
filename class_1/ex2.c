#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int pai_PID = getpid();
    printf("Pai PID: %d\n", pai_PID);

    for(int i = 0; i < 4; i++) {
        int filho_PID = fork();
        if(filho_PID == 0) {
            printf("Processo pai %d criou %d \n", getppid(), getpid());
            printf("Processo filho %d \n", getpid());
            break;
        } else {
            wait(NULL);
        }
    }
    return 0;
}
