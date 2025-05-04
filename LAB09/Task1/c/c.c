#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handle_sigint(int signum) {
    printf("SIGINT signal received (%d). Exiting the program.\n", signum);
    exit(0);
}

int main() {
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        printf("Failed to register SIGINT signal handler.\n");
        return 1;
    }

    printf("Program running... Press Ctrl+C to send SIGINT. The program will catch it and exit.\n");

    while (1) {
        sleep(1);
    }

    return 0;
}
