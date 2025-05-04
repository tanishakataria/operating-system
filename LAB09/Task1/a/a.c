#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int signum) {
    fprintf(stdout, "SIGINT signal caught! Signal number: %d\n", signum);
}

int main() {
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        printf("Failed to set up signal handler for SIGINT\n");
        return 1;
    }

    printf("Running... Press Ctrl+C to trigger SIGINT\n");

    while (1) {
        sleep(1);
    }

    return 0;
}
