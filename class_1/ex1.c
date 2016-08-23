#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
    fork();
    printf("Novo processo criado!\n");
    return 0;
}
