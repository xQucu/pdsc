#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

enum Menu
{
    INITIAL,
    NEW_ACCOUNT,
    LIST_ALL,
    SEARCH,
    SEARCH_CATEGORY,
    ACCOUNT_OPERATION,
    EXIT
};

const char *INITIAL_MENU_OPTIONS[] = {"Create a new account", "List all accounts", "Search for account", "Choose account to make transaction.", "Quit"};
const enum Menu INITIAL_MENU_ACTIONS[] = {NEW_ACCOUNT, LIST_ALL, SEARCH, ACCOUNT_OPERATION, EXIT};
int NUMBER_OF_INITIAL_MENU_OPTIONS = 5;

FILE *fptr;
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
    else if (ch == '\r') // enter key to confirm selection
    {
        *isConfirmed = true;
    }
}

void switchMenu(int *choice, const char *options[], int numberOfOptions, bool *isConfirmed)
{
    printf("Choose what operation do you want to make\n");
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
    fscanf(fptr, "%d", &accountNumber); // Skip the first line containing the last account number

    printf("List of all accounts:\n");
    char line[512];
    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        printf("%s", line);
    }

    fclose(fptr);
    printf("\nPress Enter to return to the main menu...");
    clearInput();
}

bool accountDetailsValid(char *name, char *surname, char *city, char *street, int houseNumber, char *pesel, int currentBalance, int currentLoan)
{
    if (name[0] == '\0' || surname[0] == '\0' || city[0] == '\0' || street[0] == '\0' || houseNumber <= 0 || pesel[0] == '\0' || currentBalance < 0 || currentLoan < 0)
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

    for (int i = 0; street[i] != '\0'; i++)
    {
        if (isdigit(street[i]))
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
    char street[100];
    int houseNumber;
    char pesel[12];
    int currentBalance;
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
    printf("Enter your street: ");
    scanf("%99s", street);
    printf("Enter your house number: ");
    scanf("%d", &houseNumber);
    clearInput();
    printf("Enter your PESEL: ");
    scanf("%11s", pesel);
    clearInput();
    printf("Enter your current balance: ");
    scanf("%d", &currentBalance);
    clearInput();
    printf("Enter your current loan: ");
    scanf("%d", &currentLoan);
    clearInput();

    bool valid = accountDetailsValid(name, surname, city, street, houseNumber, pesel, currentBalance, currentLoan);
    if (!valid)
    {
        printf("Incorrect data, please try again\n");
        sleep(1);
        return;
    }

    int accountNumber = calculateAccountNumber();
    char accountDetails[500];
    snprintf(accountDetails, sizeof(accountDetails), "%d %s %s %s %s %d %s %d %d",
             accountNumber, name, surname, city, street, houseNumber, pesel, currentBalance, currentLoan);
    writeNewAccountToFile(accountDetails, accountNumber);

    printf("\nAccount created successfully with the following details:\n");
    printf("Account number: %d\n", accountNumber);
    printf("Name: %s\n", name);
    printf("Surname: %s\n", surname);
    printf("City: %s\n", city);
    printf("Street: %s\n", street);
    printf("House Number: %d\n", houseNumber);
    printf("PESEL: %s\n", pesel);
    printf("Current Balance: %d\n", currentBalance);
    printf("Current Loan: %d\n", currentLoan);
    printf("\nPress Enter to return to previous menu");
    clearInput();
}

void handleMenu(enum Menu *menu, int *choice, bool *isConfirmed)
{
    switch (*menu)
    {
    case INITIAL:
        switchMenu(choice, INITIAL_MENU_OPTIONS, NUMBER_OF_INITIAL_MENU_OPTIONS, isConfirmed);
        if (*isConfirmed)
        {
            *isConfirmed = false;
            if (INITIAL_MENU_ACTIONS[*choice] == EXIT)
            {
                *choice = -1;
                return;
            }
            *menu = INITIAL_MENU_ACTIONS[*choice];
        }

        break;
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
        break;
    case SEARCH_CATEGORY:
        break;
    case ACCOUNT_OPERATION:
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

    checkDataFile();
    greet();

    while (choice != -1)
    {
        handleMenu(&menu, &choice, &isConfirmed);
    }

    goodbye();

    // create new account
    // step by step
    // list all accounts
    // scrolling
    // search for account
    // category
    // search
    // choose account
    // deposit
    // withdrawal
    // transfer to other account
    // take loan
    // pay debt

    return 0;
}