#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_sigint(int signum) {
    printf("Received SIGINT (signal number: %d)\n", signum);
}

int main() {
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        printf("Unable to register SIGINT handler.\n");
        return 1;
    }

    printf("Program is running. Press Ctrl+C to send SIGINT.\n");

    while (1) {
        sleep(1);
    }

    return 0;
}
