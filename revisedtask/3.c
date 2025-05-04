#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_TELLERS 3
#define NUM_CUSTOMERS 10
#define MAX_TRANSACTION 100

// Shared account balance
double account_balance = 1000.00;
pthread_mutex_t balance_mutex = PTHREAD_MUTEX_INITIALIZER;

// Structure to hold teller information
typedef struct {
    int teller_id;
    int customers_served;
} TellerData;

// Function to simulate a transaction (deposit or withdrawal)
void process_transaction(int teller_id, int customer_id) {
    // Generate a random transaction amount between -MAX_TRANSACTION and +MAX_TRANSACTION
    double amount = (rand() % (2 * MAX_TRANSACTION + 1)) - MAX_TRANSACTION;

    pthread_mutex_lock(&balance_mutex);

    printf("Teller %d serving customer %d: ", teller_id, customer_id);
    if (amount >= 0) {
        printf("Depositing $%.2f\n", amount);
    } else {
        printf("Withdrawing $%.2f\n", -amount);
    }

    if (amount < 0 && account_balance < -amount) {
        printf("Teller %d: Insufficient funds for withdrawal of $%.2f (Balance: $%.2f)\n",
               teller_id, -amount, account_balance);
    } else {
        account_balance += amount;
        printf("New balance: $%.2f\n", account_balance);
    }

    pthread_mutex_unlock(&balance_mutex);

    // Simulate transaction processing delay (100ms to 300ms)
    usleep(100000 + rand() % 200000);
}

// Function run by each teller thread
void* teller_function(void* arg) {
    TellerData* data = (TellerData*)arg;

    printf("Teller %d started working\n", data->teller_id);

    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        process_transaction(data->teller_id, i + 1);
        data->customers_served++;
    }

    printf("Teller %d finished serving %d customers\n", 
           data->teller_id, data->customers_served);

    return NULL;
}

int main() {
    pthread_t tellers[NUM_TELLERS];
    TellerData teller_data[NUM_TELLERS];

    srand(time(NULL)); // Seed random number generator

    printf("Bank simulation starting with initial balance: $%.2f\n\n", account_balance);

    // Create teller threads
    for (int i = 0; i < NUM_TELLERS; i++) {
        teller_data[i].teller_id = i + 1;
        teller_data[i].customers_served = 0;
        pthread_create(&tellers[i], NULL, teller_function, &teller_data[i]);
    }

    // Wait for all tellers to complete
    for (int i = 0; i < NUM_TELLERS; i++) {
        pthread_join(tellers[i], NULL);
    }

    printf("\nAll tellers finished working\n");
    printf("Final account balance: $%.2f\n", account_balance);

    pthread_mutex_destroy(&balance_mutex);

    return 0;
}
