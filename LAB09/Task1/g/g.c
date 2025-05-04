#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sigint_handler(int signum){
      fprintf(stdout, "Caugh SIGINT signal (%d). Forcefully terminating using SIGKILL....\n", signum);
      kill(getpid(), SIGKILL);
}

void my_handler(int signum){
      printf("Caught user-defined signal SIGURS1 (%d). Hello from my_handler!\n", signum);
}

int main(){
     if(signal(SIGINT, sigint_handler) == SIG_ERR){
        printf("Error setting up handler for SIGUSR1\n");
        return 1;
      }

      printf("Program started. PID: %d\n", getpid());
      printf("Press Ctrl+C to send SIGINT, or send SIGUSR1 using: kill -USR1 %d\n", getpid());

      printf("Raising SIGUSR1 internally...\n");
      kill(getpid(), SIGUSR1);

      while(1){
          sleep(1);
      }

      return 0;
}
