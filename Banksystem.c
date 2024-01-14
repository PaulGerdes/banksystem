#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ACCOUNTS 100

typedef struct bankKonto {
    char* firstName;
    char* lastName;
    int kontoStand;
    char iBan[30];
} bankKonto;

bankKonto accounts[MAX_ACCOUNTS];
int numAccounts = 0;

void iBanCreator(char* iBan) {
    sprintf(iBan, "IBAN%d", rand());
}

void bankKontoErstellen(bankKonto* account) {
    account->firstName = malloc(50);
    account->lastName = malloc(50);

    printf("Enter first name: ");
    scanf("%s", account->firstName);

    printf("Enter last name: ");
    scanf("%s", account->lastName);

    iBanCreator(account->iBan);
    printf("Generated IBAN: %s\n", account->iBan);

    account->kontoStand = 50;
}

void showAllFirstNames(bankKonto* accounts, int numAccounts) {
    printf("All First Names:\n");
    for (int i = 0; i < numAccounts; i++) {
        printf("%d. %s\n", i + 1, accounts[i].firstName);
    }
}

void showAllIbansAndNames(bankKonto* accounts, int numAccounts) {
    printf("All Ibans and First Names:\n");
    for (int i = 0; i < numAccounts; i++) {
        printf("%d. %s: %s\n", i + 1, accounts[i].firstName, accounts[i].iBan);
    }
}

void freeBankKonto(bankKonto* account) {
    free(account->firstName);
    free(account->lastName);
}

void userMoneyTopUp(bankKonto* accounts, int numberAccount){\
    int topUpValue;
    printf("\nHow much € do you want to top up?\n");
    scanf("%d",&topUpValue);
    accounts[numberAccount].kontoStand += topUpValue;
    printf("\nYou new Bank Balance is %d €\n",accounts[numberAccount].kontoStand);
}

void userPayOut(bankKonto* accounts, int numberAccount){\
    int payOutValue;
    printf("\nHow much € do you want to pay out?\n");
    scanf("%d",&payOutValue);
    accounts[numberAccount].kontoStand -= payOutValue;
    printf("\nYou new Bank Balance is %d €\n",accounts[numberAccount].kontoStand);
}


void sendMoneyFunktion(bankKonto* accounts, int numberAccount, bool haveIban) {
    if (haveIban) {
        char IbanOfReciever[30];
        int numOfReciever = 0;

        printf("Please enter the recipient's IBAN: ");
        scanf("%s", IbanOfReciever);

        char fullIban[35];
        strcpy(fullIban, "IBAN");
        strcat(fullIban, IbanOfReciever);

        while (numOfReciever < numAccounts && strcmp(accounts[numOfReciever].iBan, fullIban) != 0) {
            numOfReciever++;
        }

        if (numOfReciever < numAccounts) {
            printf("Recipient: %s %s\n", accounts[numOfReciever].firstName, accounts[numOfReciever].lastName);
            int transactionValue = 0;
            printf("Please Type how much € you want to send\n");
            scanf("%d",&transactionValue);
            if(transactionValue < accounts[numberAccount - 1].kontoStand){
                accounts[numberAccount - 1].kontoStand -= transactionValue;
                accounts[numOfReciever].kontoStand += transactionValue;

                printf("\n\nTransaction successful!\n You send %s %d€\n\n",accounts[numOfReciever].firstName, transactionValue);

                printf("\nYou new Bank Balance: %d\n", accounts[numberAccount - 1].kontoStand);
            } else { 
                printf("\nYou got not enough Money you only got %d\n",accounts[numberAccount - 1].kontoStand);
            }
            
        } else {
            printf("\nRecipient with the specified IBAN not found.\n");
        }
    } else {
        // Handle case when IBAN is not available
        printf("\nIBAN not available.\n");
    }
}

void sendMoneyDashboard() {
    printf("Do you have the IBAN of the person, or do you want to search by the name?\n");
    printf("1. I have the IBAN\n");
    printf("2. I want to do it by the name\n");
    printf("3. Back to Main menu\n");
}

void userLoggedinDashboard(bankKonto* accounts, int numberAccount) {
    int userInput;
    printf("Hello %s your Bank Balance is %d€\n",accounts[numberAccount].firstName, accounts[numberAccount].kontoStand);
    printf("What do you want to do?\n");
    printf("1. Send Money to Someone\n");
    printf("2. Top up Money\n");
    printf("3. Pay out Money\n");
    printf("4. Logout\n");
}

void displayDashboard() {
    printf("\nDashboard\n");
    printf("1. Add a new account\n");
    printf("2. Show all account names\n");
    printf("3. Show all IBANs and the First Names\n");
    printf("4. Make a transaction\n");
    printf("5. Exit\n");
    printf("Choose an option: ");
}

void displayTransactionDashboard(bankKonto* accounts, int numAccounts) {
    int choice;
    char finalIbanInput[30];

    printf("What is your IBAN? ");
    scanf("%s", finalIbanInput);

    // Concatenate "IBAN" with the user input
    char fullIban[35];
    strcpy(fullIban, "IBAN");
    strcat(fullIban, finalIbanInput);

    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].iBan, fullIban) == 0) {
            // Do something with the account
            printf("\nFound account with IBAN %s\n", finalIbanInput);
            printf("\nHello %s\n", accounts[i].firstName);

            do {
                userLoggedinDashboard(accounts, i);
                scanf("%d", &choice);
                switch (choice) {
                    case 1:
                        break;
                    case 2:
                        userMoneyTopUp(accounts, i);
                        break;
                    case 3:
                        userPayOut(accounts, i);
                        break;
                    case 4:
                        break;   
                    default:
                        printf("Invalid input\n");
                }
            } while (choice != 1 && choice != 4);

            if (choice != 2 && choice != 4) {
                do {
                    sendMoneyDashboard();
                    scanf("%d", &choice);
                    switch (choice) {
                        case 1:
                            sendMoneyFunktion(accounts, i + 1, true);
                            break;

                        case 2:
                            break;

                        case 3:
                            break;

                        default:
                            printf("Invalid input\n");
                    }
                } while (choice != 3);
            }
        }
    }

    printf("\nInvalid IBAN\n");
}

int main() {
    int choice;

    do {
        displayDashboard();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (numAccounts < MAX_ACCOUNTS) {
                    bankKontoErstellen(&accounts[numAccounts]);
                    numAccounts++;
                } else {
                    printf("Maximum number of accounts reached!\n");
                }
                break;

            case 2:
                showAllFirstNames(accounts, numAccounts);
                break;

            case 3:
                showAllIbansAndNames(accounts, numAccounts);
                break;

            case 4:
                displayTransactionDashboard(accounts, numAccounts);
                break;

            case 5:
                showAllIbansAndNames(accounts, numAccounts);
                break;

            default:
                printf("Invalid option. Please choose a valid option.\n");
        }

    } while (choice != 5);

    // Free allocated memory before exiting
    for (int i = 0; i < numAccounts; i++) {
        freeBankKonto(&accounts[i]);
    }

    printf("Exiting the program. Goodbye!\n");

    return 0;
}
