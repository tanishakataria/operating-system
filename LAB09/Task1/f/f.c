#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_sigint(int signum) {
    printf("SIGINT signal received (%d). Attempting to terminate with SIGKILL.\n", signum);
    raise(SIGKILL);
}

int main() {
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        printf("Failed to register handler for SIGINT.\n");
        return 1;
    }

    printf("Program is running. Press Ctrl+C to send a SIGINT signal.\n");

    while (1) {
        sleep(1);
    }

    return 0;
}
