#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

enum Menu
{
    INITIAL,
    NEW_ACCOUNT,
    LIST_ALL,
    SEARCH_CATEGORY,
    SEARCH,
    CHOOSING_ACCOUNT_OPERATION,
    ACCOUNT_OPERATION,
    EXIT
};

enum SearchCategories
{
    ACCOUNT_NUMBER,
    NAME,
    SURNAME,
    CITY,
    PESEL,
    RETURN_TO_INITIAL_MENU
};

enum AccountOperations
{
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER,
    LOAN,
    PAY_DEBT,
    RETURN_TO_PREVIOUS_MENU
};

const char *INITIAL_MENU_OPTIONS[] = {"Create new account", "List all accounts", "Search for account", "Make transaction", "Quit"};
const enum Menu INITIAL_MENU_ACTIONS[] = {NEW_ACCOUNT, LIST_ALL, SEARCH_CATEGORY, CHOOSING_ACCOUNT_OPERATION, EXIT};
int NUMBER_OF_INITIAL_MENU_OPTIONS = 5;

const char *SEARCH_CATEGORIES[] = {"Account number", "Name", "Surname", "City", "PESEL", "Return to previous menu"};
const enum SearchCategories SEARCH_CATEGORIES_ACTIONS[] = {ACCOUNT_NUMBER, NAME, SURNAME, CITY, PESEL, RETURN_TO_INITIAL_MENU};
int NUMBER_OF_SEARCH_CATEGORIES = 6;

const char *ACCOUNT_OPERATIONS[] = {"Deposit money", "Withdraw money", "Transfer money", "Take loan", "Pay the debt", "Return to previous menu"};
const enum AccountOperations ACCOUNT_OPERATIONS_ACTIONS[] = {DEPOSIT, WITHDRAWAL, TRANSFER, LOAN, PAY_DEBT, RETURN_TO_PREVIOUS_MENU};
int NUMBER_OF_ACCOUNT_OPERATIONS = 6;

FILE *fptr;
FILE *newFptr;
void clearInput()
{
    while (getchar() != '\n')
        ;
}

void cls()
{
    system("clear");
}

void greet()
{
    system("stty sane");
    printf("Welcome to banking system\n");
    printf("\nPress Enter to continue");
    clearInput();
    cls();
}

void goodbye()
{
    printf("Thank you for choosing this service.\n");
}

void handleInput(int *choice, int numberOfOptions, bool *isConfirmed)
{
    system("stty raw");
    char ch = getchar();
    system("stty sane");

    cls();

    if (ch == '\033')
    {
        getchar();
        switch (getchar())
        {
        case 'A': // arrow up
            if (*choice > 0)
                (*choice)--;
            break;
        case 'B': // arrow down
            if (*choice < numberOfOptions - 1)
                (*choice)++;
            break;
        }
    }
    else if (ch == '\r') // enter key
    {
        *isConfirmed = true;
    }
}

void switchMenu(int *choice, const char *options[], int numberOfOptions, bool *isConfirmed, const char *message)
{
    printf("%s\n", message);
    for (int i = 0; i < numberOfOptions; i++)
    {
        printf("[%c] %s\n", *choice == i ? 'x' : ' ', options[i]);
    }
    handleInput(choice, numberOfOptions, isConfirmed);
}

void writeNewAccountToFile(char *accountDetails, int accountNumber)
{
    fptr = fopen("data.txt", "r+");
    if (fptr == NULL)
    {
        printf("ERROR\n");
        exit(3);
    }

    fseek(fptr, 0, SEEK_SET);
    fprintf(fptr, "%d\n", accountNumber);

    fseek(fptr, 0, SEEK_END);
    fprintf(fptr, "%s\n", accountDetails);

    fclose(fptr);
}
void listAccounts()
{
    fptr = fopen("data.txt", "r");
    if (fptr == NULL)
    {
        printf("ERROR: Unable to open data file.\n");
        exit(3);
    }

    int accountNumber;
    fscanf(fptr, "%d", &accountNumber);

    printf("Account_number Name Surname City PESEL Current_balance Loan\n");
    char line[1000];
    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        printf("%s", line);
    }

    fclose(fptr);
    printf("\nPress Enter to return to the main menu...");
    clearInput();
}

bool accountDetailsValid(char *name, char *surname, char *city, char *pesel, unsigned int currentBalance, int currentLoan)
{
    if (name[0] == '\0' || surname[0] == '\0' || city[0] == '\0' || pesel[0] == '\0' || currentBalance < 0 || currentLoan < 0)
    {
        return false;
    }

    if (strlen(pesel) != 11)
    {
        return false;
    }

    for (int i = 0; name[i] != '\0'; i++)
    {
        if (isdigit(name[i]))
        {
            return false;
        }
    }

    for (int i = 0; surname[i] != '\0'; i++)
    {
        if (isdigit(surname[i]))
        {
            return false;
        }
    }

    for (int i = 0; city[i] != '\0'; i++)
    {
        if (isdigit(city[i]))
        {
            return false;
        }
    }

    return true;
}

int calculateAccountNumber()
{
    int accountNumber = 0;
    fptr = fopen("data.txt", "r");
    if (fptr == NULL)
    {
        printf("ERROR: Unable to open data file.\n");
        exit(3);
    }
    fscanf(fptr, "%d", &accountNumber);
    fclose(fptr);
    return accountNumber + 1;
}

void createNewAccount()
{
    char name[100];
    char surname[100];
    char city[100];
    char pesel[12];
    unsigned int currentBalance;
    int currentLoan;

    printf("Welcome to the new account creator\n");
    printf("Enter your name: ");
    scanf("%99s", name);

    printf("Enter your surname: ");
    scanf("%99s", surname);
    clearInput();
    printf("Enter your city: ");
    scanf("%99s", city);
    clearInput();
    printf("Enter your PESEL: ");
    scanf("%11s", pesel);
    clearInput();
    printf("Enter your current balance: ");
    scanf("%u", &currentBalance);
    clearInput();
    printf("Enter your current loan: ");
    scanf("%d", &currentLoan);
    clearInput();

    bool valid = accountDetailsValid(name, surname, city, pesel, currentBalance, currentLoan);
    if (!valid)
    {
        printf("Incorrect data, please try again\n");
        sleep(1);
        return;
    }

    int accountNumber = calculateAccountNumber();
    char accountDetails[500];
    snprintf(accountDetails, sizeof(accountDetails), "%d %s %s %s %s %d %d",
             accountNumber, name, surname, city, pesel, currentBalance, currentLoan);
    writeNewAccountToFile(accountDetails, accountNumber);

    printf("\nAccount created successfully with the following details:\n");
    printf("Account number: %d\n", accountNumber);
    printf("Name: %s\n", name);
    printf("Surname: %s\n", surname);
    printf("City: %s\n", city);
    printf("PESEL: %s\n", pesel);
    printf("Current Balance: %d\n", currentBalance);
    printf("Current Loan: %d\n", currentLoan);
    printf("\nPress Enter to return to previous menu");
    clearInput();
}

void searchRecords(int *choice)
{
    char searchPhrase[100];
    printf("Enter the %s to search for.\n", SEARCH_CATEGORIES[*choice]);
    scanf("%99s", searchPhrase);

    fptr = fopen("data.txt", "r");
    int accountNumber;
    fscanf(fptr, "%d", &accountNumber);
    int ok;
    bool found = false;
    while (1)
    {
        int accountNumber, currentBalance, currentLoan;
        char name[100], surname[100], city[100], pesel[12];
        char *dataToCompare[] = {name, surname, city, pesel};
        ok = fscanf(fptr, "%d %99s %99s %99s %11s %d %d",
                    &accountNumber, name, surname, city, pesel, &currentBalance, &currentLoan);
        if (ok == EOF)
            break;

        if ((*choice == 0 && atoi(searchPhrase) == accountNumber) || (*choice != 0 && strcasecmp(dataToCompare[*choice - 1], searchPhrase) == 0))
        {
            found = true;
            printf("Account number: %d\n", accountNumber);
            printf("Name: %s\n", name);
            printf("Surname: %s\n", surname);
            printf("City: %s\n", city);
            printf("PESEL: %s\n", pesel);
            printf("Current Balance: %d\n", currentBalance);
            printf("Current Loan: %d\n", currentLoan);
        }
    }

    if (!found)
    {
        printf("\nAccount not found\n");
    }
    printf("\nPress Enter to return to the previous menu...");

    getchar();
    clearInput();
    cls();
    fclose(fptr);
}

bool getAccountDetails(int id, int *currentBalance, int *currentLoan)
{
    int ok;
    fptr = fopen("data.txt", "r");
    fscanf(fptr, "%*s");
    while (1)
    {
        int accountNumber;

        ok = fscanf(fptr, "%d %*s %*s %*s %*s %d %d", &accountNumber, currentBalance, currentLoan);
        if (ok == EOF)
            break;

        if (id == accountNumber)
        {
            fclose(fptr);
            return true;
        }
    }
    fclose(fptr);
    return false;
}

bool confirmTransaction()
{
    bool isConfirmed = false;
    int decision = 0;
    const char *confirmationOptions[] = {"Yes", "No"};
    int numberOfOptions = 2;
    printf("Are you sure you want to make this transaction? (y/n)\n");
    const char *message = "Are you sure you want to make that operation";
    while (!isConfirmed)
    {
        switchMenu(&decision, confirmationOptions, numberOfOptions, &isConfirmed, message);
    }
    return decision == 0;
}

void updateOneRecord(int id, int newBalance, int newLoan)
{
    fptr = fopen("data.txt", "r");
    remove("data.tmp");
    newFptr = fopen("data.tmp", "w");
    int accountNumber, balance, loan;
    char name[100], surname[100], city[100], pesel[12];
    int tBalance, tLoan;
    int args;
    int temp;
    fscanf(fptr, "%d", &temp);
    while (1)
    {

        args = fscanf(fptr, "%d %99s %99s %99s %11s %d %d", &accountNumber, name, surname, city, pesel, &balance, &loan);

        if (args == EOF)
        {
            break;
        }
        if (id == accountNumber)
        {
            tBalance = newBalance;
            tLoan = newLoan;
        }
        else
        {
            tBalance = balance;
            tLoan = loan;
        }
        if (fprintf(newFptr, "%d %s %s %s %s %d %d\n", accountNumber, name, surname, city, pesel, tBalance, tLoan) < 0)
        {
            perror("Error\n");
            exit(3);
        }
    }
    fclose(fptr);
    fclose(newFptr);
    rename("data.tmp", "data.txt");
    remove("data.tmp");
}

void updateTwoRecords(int firstId, int firstNewBalance, int firstNewLoan, int secondId, int secondNewBalance, int secondNewLoan)
{
    fptr = fopen("data.txt", "r");
    remove("data.tmp");
    newFptr = fopen("data.tmp", "w");
    int accountNumber, balance, loan;
    char name[100], surname[100], city[100], pesel[12];
    int tBalance, tLoan;
    int args;
    int temp;
    fscanf(fptr, "%d", &temp);
    while (1)
    {

        args = fscanf(fptr, "%d %99s %99s %99s %11s %d %d", &accountNumber, name, surname, city, pesel, &balance, &loan);

        if (args == EOF)
        {
            break;
        }
        if (firstId == accountNumber)
        {
            tBalance = firstNewBalance;
            tLoan = firstNewLoan;
        }
        else if (secondId == accountNumber)
        {
            tBalance = secondNewBalance;
            tLoan = secondNewLoan;
        }
        else
        {
            tBalance = balance;
            tLoan = loan;
        }
        if (fprintf(newFptr, "%d %s %s %s %s %d %d\n", accountNumber, name, surname, city, pesel, tBalance, tLoan) < 0)
        {
            perror("Error\n");
            exit(3);
        }
    }
    fclose(fptr);
    fclose(newFptr);
    rename("data.tmp", "data.txt");
    remove("data.tmp");
}

void handleAccountOperation(int *choice, int *chosenID)
{
    printf("Please enter your account number.\n");
    scanf("%d", chosenID);
    clearInput();
    int currentBalance, currentLoan;
    bool ok = getAccountDetails(*chosenID, &currentBalance, &currentLoan);
    if (!ok)
    {
        printf("Operation aborted. Account doesn't exists.\n");
        return;
    }

    int amount = 0;

    switch (ACCOUNT_OPERATIONS_ACTIONS[*choice])
    {
    case DEPOSIT:
        printf("How much money do you want to deposit?\n");
        scanf("%d", &amount);
        printf("You want to deposit: %u\n", amount);
        if (amount <= 0 || amount > currentBalance)
        {
            printf("Operation can't be finished.\n");
            return;
        }
        bool isConfirmed = confirmTransaction();
        if (!isConfirmed)
        {
            printf("Operation aborted.\n");
            return;
        }

        int newBalance = currentBalance + amount;
        updateOneRecord(*chosenID, newBalance, currentLoan);
        printf("Operation completed successfully\n");

        break;
    case WITHDRAWAL:
        break;
    case TRANSFER:
        break;
    case LOAN:
        break;
    case PAY_DEBT:
        break;

    default:
        printf("ERROR\n");
        break;
    }
}

void handleMenu(enum Menu *menu, int *choice, bool *isConfirmed, int *chosenID)
{

    switch (*menu)
    {
    case INITIAL:
    {
        char *message = "Choose what do you want to do";

        switchMenu(choice, INITIAL_MENU_OPTIONS, NUMBER_OF_INITIAL_MENU_OPTIONS, isConfirmed, message);
        if (*isConfirmed)
        {
            *isConfirmed = false;
            if (INITIAL_MENU_ACTIONS[*choice] == EXIT)
            {
                *choice = -1;
                return;
            }
            *menu = INITIAL_MENU_ACTIONS[*choice];
            *choice = 0;
        }

        break;
    }
    case NEW_ACCOUNT:
        createNewAccount();
        cls();
        *menu = INITIAL;
        break;
    case LIST_ALL:
        listAccounts();
        cls();
        *menu = INITIAL;
        break;
    case SEARCH:
        searchRecords(choice);
        *menu = SEARCH_CATEGORY;
        break;
    case SEARCH_CATEGORY:
    {
        const char *message = "Choose category to search by";
        switchMenu(choice, SEARCH_CATEGORIES, NUMBER_OF_SEARCH_CATEGORIES, isConfirmed, message);
        if (*isConfirmed)
        {
            *isConfirmed = false;
            if (SEARCH_CATEGORIES_ACTIONS[*choice] == RETURN_TO_INITIAL_MENU)
            {
                *choice = 0;
                *menu = INITIAL;
                return;
            }
            *menu = SEARCH;
        }
        break;
    }
    case CHOOSING_ACCOUNT_OPERATION:
    {
        const char *message = "Choose what operation do you want to make";
        switchMenu(choice, ACCOUNT_OPERATIONS, NUMBER_OF_ACCOUNT_OPERATIONS, isConfirmed, message);
        if (*isConfirmed)
        {
            *isConfirmed = false;
            if (ACCOUNT_OPERATIONS_ACTIONS[*choice] == RETURN_TO_PREVIOUS_MENU)
            {
                *choice = 0;
                *menu = INITIAL;
                return;
            }
            *menu = ACCOUNT_OPERATION;
        }
        break;
    }
    case ACCOUNT_OPERATION:
        handleAccountOperation(choice, chosenID);

        printf("\nPress Enter to continue");
        clearInput();
        cls();
        *menu = CHOOSING_ACCOUNT_OPERATION;
        break;

    default:
        printf("BREAKS\n");
        exit(3);
        break;
    }
}

void checkDataFile()
{
    fptr = fopen("data.txt", "r");
    if (fptr == NULL)
    {
        fptr = fopen("data.txt", "w");
        if (fptr == NULL)
        {
            printf("ERROR: Unable to create data file.\n");
            exit(3);
        }
        fprintf(fptr, "0\n");
    }
    fclose(fptr);
}

int main()
{
    enum Menu menu = INITIAL;
    int choice = 0;
    bool isConfirmed = false;
    int chosenID = -1;

    checkDataFile();
    greet();

    while (choice != -1)
    {
        handleMenu(&menu, &choice, &isConfirmed, &chosenID);
    }

    goodbye();

    // create new account v
    // step by step
    // list all accounts v
    // search for account v
    // category v
    // search v
    // choose account
    // deposit
    // withdrawal
    // transfer to other account
    // take loan
    // pay debt

    return 0;
}