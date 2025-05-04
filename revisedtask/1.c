#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define TEMP_FILE_NAME "temp_data_file.tmp"

FILE *temp_file = NULL;

void handle_sigint(int sig){
     printf("\nReceived SIGINT (Ctrl+C). Cleaning up...\n");
     if(temp_file != NULL){
         if(fclose(temp_file) == 0){
             printf("Temporary file closed successfully.\n");
          } else{
              fprintf(stderr, "Error closing file: %s\n", strerror(errno));
          }

          if(remove(TEMP_FILE_NAME) == 0){
               printf("Temporary file '%s'deleted successfully.\n", TEMP_FILE_NAME);
          } else{
               fprintf(stderr, "Error deleting file: %s\n", strerror(errno));
          }
      }

      printf("Exiting program\n");
      exit(0);
}

int main(){
     struct sigaction sa;
     sa.sa_handler = handle_sigint;
     sigemptyset(&sa.sa_mask);
     sa.sa_flags = 0;

    if(sigaction(SIGINT, &sa, NULL) == -1){
       perror("Error setting up signal handler");
       return 1;
    }

    temp_file = fopen(TEMP_FILE_NAME, "w");
    if(temp_file == NULL){
       perror("Error creating temporary file");
       return 1;
    }
    
    printf("Created temporary file '%s'\n", TEMP_FILE_NAME);
    printf("Press Ctrl+C to stop the program and clean up..\n");

    int counter = 0;
    while(1){
         fprintf(temp_file, "Data entry %d - Timestamp: %ld\n", counter++, time(NULL));
         fflush(temp_file);
         sleep(1);
    }

    return 0;
}
