#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//creates a global head pointer for the structures
struct AccountNode *Accounthead = NULL; 

struct ContactNode *Contacthead = NULL;

// to call the functions
void login(char *name, char *password);
void freeAccountList(); 
void freeContactList(); 
struct AccountNode *createAccountNode(const char *name, const char *password);     
struct ContactNode *createContactNode(const char *name, const char *number);    
void appendAccountNode(struct AccountNode **head, const char *name, const char *password); 
void printAccountList();   
void loadAccountsCSV(); 
void unloadAccountCSV(); 
void freeContactList();  
struct ContactNode *createContactNode(const char *name, const char *number);
void appendContactNode(struct ContactNode **head, const char *name, const char *number); 
void printContactList();  
void loadContactCSV(char *filename); 
void unloadContactCSV(char *filename); 
void newAccount();  
void loggingIn(); 
void deleteAccount(); 
void forgetPassword(); 
void newcontact(char *name, char *password);  
void deletecontact(char *name, char *password); 
void editName(char *name, char *password); 
void editNumber(char *name, char *password); 
char *searchContact(char *name, char *password);                                           
void login(char *name, char *password);                                                    

// creates a structure to store the accounts
struct AccountNode
{
    char name[100];
    char password[100];
    struct AccountNode *prev; 
    struct AccountNode *next;
};

// deletes an account
void freeAccountList()
{
    struct AccountNode *current = Accounthead;
    while (current != NULL)
    {
        struct AccountNode *temp = current;
        current = current->next;
        free(temp);
    }
    Accounthead = NULL;
}

// creates new account nodes
struct AccountNode *createAccountNode(const char *name, const char *password)
{
    struct AccountNode *newNode = (struct AccountNode *)malloc(sizeof(struct AccountNode));
    if (newNode == NULL)
    {
        perror("\t\t\t Memory allocation failed");
        exit(1);
    }
    strncpy(newNode->name, name, sizeof(newNode->name));
    strncpy(newNode->password, password, sizeof(newNode->password));
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// adds account nodes to account structure in the begining
void appendAccountNode(struct AccountNode **head, const char *name, const char *password)
{
    struct AccountNode *newNode = createAccountNode(name, password);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        struct AccountNode *current = *head;
        struct AccountNode *temp = current->next;
        current->next = newNode;
        newNode->prev = current;
        newNode->next = temp;
        if (temp != NULL)
        {
            temp->prev = newNode;
        }
    }
}

// prints the list of accounts 
void printAccountList()
{
    struct AccountNode *current = Accounthead;
    while (current != NULL)
    {
        printf("\t\t\t Name: %s, Password: %s\n", current->name, current->password);
        current = current->next;
    }
}

// reads information from accounts csv file
void loadAccountsCSV()
{
    freeAccountList();
    FILE *file = fopen("accounts.csv", "r");
    if (file == NULL)
    {
        perror("\t\t\t Error opening the file");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char name[100];
        char password[100];
        if (sscanf(line, " %[^,],%s", name, password) == 2)
        {
            appendAccountNode(&Accounthead, name, password);
        }
    }
    fclose(file);
}

// uploads the account information to 'accounts.csv'
void unloadAccountCSV()
{
    FILE *fptr = fopen("accounts.csv", "w");
    if (fptr == NULL)
    {
        perror("\t\t\t Error opening the file");
        return;
    }
    struct AccountNode *current = Accounthead;
    int numAccount = 0;
    while (current != NULL)
    {
        numAccount++;
        current = current->next;
    }
    struct AccountNode *accountArray = (struct AccountNode *)malloc(numAccount * sizeof(struct AccountNode));
    current = Accounthead;
    for (int i = 0; i < numAccount; i++)
    {
        strcpy(accountArray[i].name, current->name);
        strcpy(accountArray[i].password, current->password);
        current = current->next;
    }
    for (int i = 0; i < numAccount - 1; i++)
    {
        for (int j = i + 1; j < numAccount; j++)
        {
            if (strcmp(accountArray[i].name, accountArray[j].name) > 0)
            {
                struct AccountNode temp = accountArray[i];
                accountArray[i] = accountArray[j];
                accountArray[j] = temp;
            }
        }
    }
    for (int i = 0; i < numAccount; i++)
    {
        fprintf(fptr, "%s,%s\n", accountArray[i].name, accountArray[i].password);
    }
    fclose(fptr);
    free(accountArray);
    freeAccountList();
}

// creates a structure to store contact information
struct ContactNode
{
    char name[100];
    char number[100];
    struct ContactNode *prev; 
    struct ContactNode *next;
};

// deletes a contact
void freeContactList()
{
    struct ContactNode *head = Contacthead;
    while (head != NULL)
    {
        struct ContactNode *temp = head;
        head = head->next;
        free(temp);
    }
    Contacthead = NULL;
}

// creates nodes for contact structure
struct ContactNode *createContactNode(const char *name, const char *number)
{
    struct ContactNode *newNode = (struct ContactNode *)malloc(sizeof(struct ContactNode));
    if (newNode == NULL)
    {
        perror("\t\t\t Memory allocation failed");
        exit(1);
    }
    strncpy(newNode->name, name, sizeof(newNode->name));
    strncpy(newNode->number, number, sizeof(newNode->number));
    newNode->next = NULL;
    return newNode;
}

// adds contact nodes to contact structure at the begining
void appendContactNode(struct ContactNode **head, const char *name, const char *number)
{
    struct ContactNode *newNode = createContactNode(name, number);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        struct ContactNode *current = *head;
        struct ContactNode *temp = current->next;
        current->next = newNode;
        newNode->prev = current;
        newNode->next = temp;
        if (temp != NULL)
        {
            temp->prev = newNode;
        }
    }
}

// reads the information from respective account csv file
void loadContactCSV(char *filename)
{
    freeContactList();
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("\t\t\t Error opening the file");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char name[100];
        char number[100];
        if (sscanf(line, "%[^,],%s", name, number) == 2)
        {
            appendContactNode(&Contacthead, name, number);
        }
    }
    fclose(file);
}

// uploads the contact information in a csv file named according to username
void unloadContactCSV(char *filename)
{
    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL)
    {
        perror("\t\t\t Error opening the file");
        return;
    }
    struct ContactNode *current = Contacthead;
    int numContact = 0;
    while (current != NULL)
    {
        numContact++;
        current = current->next;
    }
    struct ContactNode *contactArray = (struct ContactNode *)malloc(numContact * sizeof(struct ContactNode));
    current = Contacthead;
    for (int i = 0; i < numContact; i++)
    {
        strcpy(contactArray[i].name, current->name);
        strcpy(contactArray[i].number, current->number);
        current = current->next;
    }
    for (int i = 0; i < numContact - 1; i++)
    {
        for (int j = i + 1; j < numContact; j++)
        {
            if (strcmp(contactArray[i].name, contactArray[j].name) > 0)
            {
                struct ContactNode temp = contactArray[i];
                contactArray[i] = contactArray[j];
                contactArray[j] = temp;
            }
        }
    }
    for (int i = 0; i < numContact; i++)
    {
        fprintf(fptr, "%s,%s\n", contactArray[i].name, contactArray[i].number);
    }
    fclose(fptr);
    free(contactArray);
    freeContactList();
}

// creates new account
void newAccount()
{

    char name[15];
    int flag = 0;
    char password[15];
    printf("\n\t\t\t Enter your name: ");
    scanf(" %[^\n]", name);
    printf("\n");
    printf("\t\t\t Enter your password: ");
    scanf(" %[^\n]", password);
    printf("\n");
    loadAccountsCSV();
    struct AccountNode *curr = Accounthead;
    while (curr != NULL)
    {
        if (strcmp(curr->name, name) == 0)
        {
            flag = 1;
        }
        curr = curr->next;
    }
    if (flag == 0)
    {
        appendAccountNode(&Accounthead, name, password);
        unloadAccountCSV(Accounthead);
        char filename[50];
        snprintf(filename, sizeof(filename), "%s%s.csv", name, password);
        FILE *file = fopen(filename, "w");
        if (file == NULL)
        {
            perror("\n\t\t\t Failed to create file");
            return;
        }
        fclose(file);
    }
}

// logs into an existing account
void loggingIn()
{
    char name[15];
    char password[15];
    printf("\n\t\t\t Enter your name: ");
    scanf(" %[^\n]", name);
    printf("\n");
    printf("\t\t\t Enter your password: ");
    scanf(" %[^\n]", password);
    printf("\n");
    loadAccountsCSV();
    struct AccountNode *curr = Accounthead;
    while (curr != NULL)
    {
        if (strcmp(curr->name, name) == 0 && strcmp(curr->password, password) == 0)
        {
            login(name, password);
            return;
        }
        curr = curr->next;
    }
    printf("\n\t\t\t Login credentials not found!\n");
}

// deletes an account
void deleteAccount()
{
    char name[15];
    char password[15];
    printf("\n\t\t\t Enter your name: ");
    scanf(" %[^\n]", name);
    printf("\033[0m");
    printf("\n\t\t\t Enter your password: ");
    scanf("%s", password);
    printf("\n");
    loadAccountsCSV();
    if (Accounthead == NULL)
    {
        printf("\n\t\t\t No accounts found.\n");
        return;
    }
    struct AccountNode *curr = Accounthead;
    struct AccountNode *prev = NULL;
    while (curr != NULL)
    {
        if (strcmp(curr->name, name) == 0 && strcmp(curr->password, password) == 0)
        {
            if (prev == NULL)
            {
                Accounthead = Accounthead->next;
            }
            else
            {
                prev->next = curr->next;
            }
            free(curr);
            char filename[50];
            snprintf(filename, sizeof(filename), "%s%s.csv", name, password);
            if (remove(filename) == 0)
            {
                printf("\n\t\t\t Account '%s' has been deleted.\n", name);
            }
            else
            {
                printf("\n\t\t\t Failed to delete account '%s'.\n", name);
            }
            unloadAccountCSV(Accounthead);

            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("\n\t\t\t Account with name '%s' and password '%s' not found.\n", name, password);
}

// changes the password incase the user forgets it
// ADMIN PASSWORD IS 123
void forgetPassword()
{
    int adminPass;
    printf("\n\t\t\t Enter admin password : ");
    scanf("%d", &adminPass);
    printf("\n");
    if (adminPass != 123)
    {
        printf("\t\t\t Wrong admin password!!");
        return;
    }
    char name[15], newpass[15];
    printf("\t\t\t Enter your name : ");
    scanf(" %[^\n]", name);
    printf("\033[0m");
    loadAccountsCSV();
    struct AccountNode *current = Accounthead;
    while (current != NULL)
    {
        if ((strcmp(current->name, name) == 0))
        {

            printf("\t\t\t Enter your new password : ");
            scanf(" %[^\n]", newpass);
            printf("\033[0m");
            char oldName[35];
            strcpy(oldName, name);
            strcat(oldName, current->password);
            strcat(oldName, ".csv");
            char newName[35];
            strcpy(newName, name);
            strcat(newName, newpass);
            strcat(newName, ".csv");
            strcpy(current->password, newpass);
            unloadAccountCSV(Accounthead);
            // csvfile
            if (rename(oldName, newName) == 0)
            {
                printf("\t\t\t File renamed successfully.\n");
            }
            else
            {
                perror("\t\t\t Error renaming file\n");
            }
            return;
        }
        current = current->next;
    }
}

// creates a  new contact
void newcontact(char *name, char *password)
{
    char contactname[15];
    char number[15];
    int flag = 0;
    printf("\t\t\t Enter your name : ");
    scanf(" %[^\n]", contactname);
    printf("\033[0m");
    printf("\t\t\t Enter your number : ");
    scanf(" %[^\n]", number);
    printf("\033[0m");
    char filename[50];
    snprintf(filename, sizeof(filename), "%s%s.csv", name, password);
    loadContactCSV(filename);
    struct ContactNode *curr = Contacthead;
    while (curr != NULL)
    {
        if (strcmp(curr->name, contactname) == 0 && strcmp(curr->number, number) == 0)
        {
            flag = 1;
        }
        curr = curr->next;
    }
    if (flag == 0)
    {
        appendContactNode(&Contacthead, contactname, number);
        unloadContactCSV(filename);
    }
}

// deletes a contact
void deletecontact(char *name, char *password)
{
    char contactname[15];
    char number[15];
    printf("\t\t\t Enter your name : ");
    scanf(" %[^\n]", contactname);
    printf("\033[0m");
    printf("\t\t\t Enter your number : ");
    scanf(" %[^\n]", number);
    printf("\033[0m");
    char filename[50];
    snprintf(filename, sizeof(filename), "%s%s.csv", name, password);
    loadContactCSV(filename);
    if (Contacthead == NULL)
    {
        printf("\t\t\t No contacts found.");
        return;
    }
    struct ContactNode *curr = Contacthead;
    struct ContactNode *prev = NULL;
    while (curr != NULL)
    {
        if (strcmp(curr->name, contactname) == 0 && strcmp(curr->number, number) == 0)
        {
            if (prev == NULL)
            {
                Contacthead = Contacthead->next;
            }
            else
            {
                prev->next = curr->next;
            }
            free(curr);
            unloadContactCSV(filename);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("\t\t\t Account with name '%s' and number '%s' not found.\n", contactname, number);
}

// edits a name of an existing contact
void editName(char *name, char *password)
{
    char filename[50];
    snprintf(filename, sizeof(filename), "%s%s.csv", name, password);
    loadContactCSV(filename);
    printf("\t\t\t Enter the number whose name you want to edit : ");
    char number[15];
    scanf(" %[^\n]", number);
    printf("\033[0m");
    char Contactname[15];
    printf("\t\t\t Enter the new name : ");
    scanf(" %[^\n]", Contactname);
    printf("\033[0m");
    struct ContactNode *curr = Contacthead;
    while (curr != NULL)
    {
        if (strcmp(curr->number, number) == 0)
        {
            strcpy(curr->name, Contactname);
            break;
        }
        curr = curr->next;
    }
    unloadContactCSV(filename);
}

// edit the number of an existing contact
void editNumber(char *name, char *password)
{
    char filename[50];
    snprintf(filename, sizeof(filename), "%s%s.csv", name, password);
    loadContactCSV(filename);
    printf("\t\t\t Enter the name whose number you want to edit : ");
    char Contactname[15];
    scanf(" %[^\n]", Contactname);
    printf("\033[0m");
    char number[15];
    printf("\t\t\t Enter the new number : ");
    scanf(" %[^\n]", number);
    printf("\033[0m");

    struct ContactNode *curr = Contacthead;
    while (curr != NULL)
    {
        if (strcmp(curr->name, Contactname) == 0)
        {
            strcpy(curr->number, number);
            break;
        }
        curr = curr->next;
    }
    unloadContactCSV(filename);
}

// searches for a contact
char *searchContact(char *name, char *password)
{
    char filename[50];
    snprintf(filename, sizeof(filename), "%s%s.csv", name, password);
    loadContactCSV(filename);
    char contactName[15];
    char contactNumber[15];
    printf("\t\t\t Enter the name of the contact : ");
    scanf(" %[^\n]", contactName);
    printf("\033[0m");
    struct ContactNode *curr = Contacthead;
    while (curr != NULL)
    {
        if (strcmp(curr->name, contactName) == 0)
        {
            strcpy(contactNumber, curr->number);
            freeContactList();
            printf("\t\t\t This is the contact number : %s\n", contactNumber);
            return contactNumber;
        }
        curr = curr->next;
    }
    freeContactList();
    printf("\t\t\t No account found\n", contactNumber);
    return "-1";
}

// prints all contacts in the account
void printContactList(char *name, char *password)
{
    char filename[50];
    snprintf(filename, sizeof(filename), "%s%s.csv", name, password);
    loadContactCSV(filename);
    struct ContactNode *current = Contacthead;
    while (current != NULL)
    {
        printf("\t\t\t Name: %s \t Phone-number: %s\n", current->name, current->number);
        current = current->next;
    }
}

// shows the options after loging into an existing account
void login(char *name, char *password)
{
    printf("\t\t\t ------------------------------------------\n");
    printf("\t\t\t WELCOME %s!\n", name);

    while (1)
    {
        int choice;
        printf("\t\t\t ------------------------------------------\n");
        printf("\t\t\t 1. CREATE NEW CONTACT\n");
        printf("\t\t\t 2. DELETE A CONTACT\n");
        printf("\t\t\t 3. EDIT NAME\n");
        printf("\t\t\t 4. EDIT NUMBER\n");
        printf("\t\t\t 5. SEARCH A CONTACT\n");
        printf("\t\t\t 6. DISPLAY CONTACT\n");
        printf("\t\t\t 7. EXIT\n");
        printf("\t\t\t ENTER YOUR CHOICE: ");
        scanf("%d", &choice);
        printf("\n");
        switch (choice)
        {
        case 1:
            newcontact(name, password);
            printf("\t\t\t New contact has been added\n");
            break;
        case 2:
            deletecontact(name, password);
            break;
        case 3:
            editName(name, password);
            break;
        case 4:
            editNumber(name, password);
            break;
        case 5:
            searchContact(name, password);
            break;
        case 6:
            printContactList(name, password);
            break;
        case 7:
            return;
        default:
            printf("\t\t\t WRONG CHOICE\n\n");
        }
    }
}

// shows all the available options in the program
int main()
{
    printf("\t\t\t ------------------------------------------\n");
    printf("\t\t\tWELCOME TO OUR PHONE DIRECTORY\n");
    printf("\t\t\t------------------------------------------\n");
    while (1)
    {
        int choice;
        printf("\t\t\t ------------------------------------------\n");
        printf("\t\t\t 1. NEW USER RESIGTRATION\n");
        printf("\t\t\t 2. LOGIN/SIGNIN\n");
        printf("\t\t\t 3. DELETION ACCOUNT\n");
        printf("\t\t\t 4. FORGOT PASSWORD\n");
        printf("\t\t\t 5. EXIT\n");
        printf("\t\t\t ENTER YOUR CHOICE: ");

        scanf("%d", &choice);
        printf("\n");
        switch (choice)
        {
        case 1:
            newAccount();
            printf("\t\t\t Your account has been successfully created!\n\n");
            break;
        case 2:
            loggingIn();
            printf("\t\t\t Your account is not found, please create a new account to login\n");
            break;
        case 3:
            deleteAccount();
            break;
        case 4:
            forgetPassword();
            break;
        case 5:
            exit(0);
        default:
            printf("\t\t\t WRONG CHOICE\n\n");
        }
    }
}