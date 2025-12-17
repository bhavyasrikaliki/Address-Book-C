#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

// ------------ Validation Functions ------------

// Validate name (alphabets + spaces only)
int validateName(const char *name) 
{
    //loop through each character
    for (int i = 0; i < strlen(name); i++) 
    {
        // check if character is not alphabet or space
        if (!isalpha(name[i]) && name[i] != ' ')
        {
            return 0;// invalid name
        }
    }
    return 1;// valid name
}

// Validate phone (10 digits)
int validatePhone(const char *phone) 
{
    // check length
    if (strlen(phone) != 10) 
    {
        return 0;// invalid phone
    }
    // loop through each character
    for (int i = 0; i < 10; i++) 
    {
        //check if character is not a digit
        if (!isdigit(phone[i]))
        {
            return 0;// invalid phone
        }
    }
    return 1;//valid phone
}

// Validate email
int validateEmail(const char *email) 
{
    int len = strlen(email); 
    //checking length
    if (len < 5)
    {
        return 0; 
    }    
    // Finding '@'
    char *at = strchr(email, '@');
    if (!at)
    {
        return 0;
    }

    // Name before '@' mandatory
    if (at == email)
    {
        return 0;
    }
    // first part (before '@') and second part (after '@')
    int localLen = at - email;
    if (localLen <= 0)
    {
        return 0;
    }
    // Must contain at least one alphabet before '@'
    int hasAlpha = 0;
    for (int i = 0; i < localLen; i++) 
    {
        if (isalpha(email[i]))
        {
            hasAlpha = 1;
            break;
        }
    }
    if (!hasAlpha) 
    {
        return 0;  // reject if only digits/symbols
    }

    //second part
    char *domain = at + 1;
    if (strlen(domain) < 5)
    {
        return 0; 
    }
    // Must end with ".com"
    if (strcmp(domain + strlen(domain) - 4, ".com") != 0)
    {
        return 0;
    }
    // Character before ".com" must not be empty
    if (strlen(domain) - 4 <= 0)
    {
        return 0;
    }
    // Check each character
    for (int i = 0; i < len; i++) 
    {
        if (!(islower(email[i]) || isdigit(email[i]) || email[i] == '.' || email[i] == '_' || email[i] == '@')) 
        {
            return 0;
        }
    }

    return 1; // valid email
}


//list contacts implementation
static int g_sortcriteria;  // 0 = name, 1 = phone, 2 = email
// Compare function for qsort
int compareContacts(const void *a, const void *b) 
{
    const Contact *c1 = (const Contact *)a;
    const Contact *c2 = (const Contact *)b;
    
    if (g_sortcriteria == 0) 
    {
        return strcmp(c1->name, c2->name);   // sort by name
    } 
    else if (g_sortcriteria == 1) 
    {
        return strcmp(c1->phone, c2->phone); // sort by phone
    } 
    else if (g_sortcriteria == 2) 
    {
        return strcmp(c1->email, c2->email); // sort by email
    }
    return 0;
}
void listContacts(AddressBook *addressBook) 
{
    if (addressBook->contactCount == 0) 
    {
        printf("No contacts found!!\n");
        return;
    }

    int sortChoice;
    while (1) 
    {
        printf("Sort by: 0 = Name, 1 = Phone, 2 = Email\n");
        printf("Enter your choice: ");
        if (scanf("%d", &sortChoice) != 1) 
        {
            printf("Invalid input! Please enter a number (0-2).\n");
            while (getchar() != '\n'); // clear leftover input
            continue;
        }
        if (sortChoice < 0 || sortChoice > 2) 
        {
            printf("Choice out of range! Try again.\n");
            continue;
        }
        break; // valid input
    }

    g_sortcriteria = sortChoice; // set global sorting criteria

    // Sort contacts before printing
    qsort(addressBook->contacts, addressBook->contactCount, sizeof(Contact), compareContacts);
    printf("\n===== Contact List =====\n");
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        printf("%d) Name: %s         | Phone: %s           | Email: %s\n",
               i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
    printf("========================\n");
}

void initialize(AddressBook *addressBook) 
{
    addressBook->contactCount = 0;

    // Load existing contacts from file first
    loadContactsFromFile(addressBook);

    // If no contacts loaded, then populate dummy contacts
    if (addressBook->contactCount == 0) 
    {
        populateAddressBook(addressBook);
    }
}

void saveAndExit(AddressBook *addressBook)
{
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

int isDuplicate(AddressBook *addressBook, char *phone, char *email)
{
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if ((strlen(phone) > 0 && strcmp(addressBook->contacts[i].phone, phone) == 0) ||
            (strlen(email) > 0 && strcmp(addressBook->contacts[i].email, email) == 0))
        {
            return 1; // duplicate found
        }
    }
    return 0; // no duplicate
}


void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */
    Contact newContact;

    // ---- NAME ----
    do 
    {
        printf("Enter name: ");
        scanf(" %[^\n]", newContact.name);  // allows spaces

        if (!validateName(newContact.name)) 
        {
            printf("Invalid name! Please try again.\n");
        }
    } while (!validateName(newContact.name));

    // ---- PHONE ----
    do 
    {
        printf("Enter phone: ");
        scanf("%s", newContact.phone);

        if (!validatePhone(newContact.phone)) 
        {
            printf("Invalid phone number! Please try again.\n");
            continue;
        }

        if (isDuplicate(addressBook, newContact.phone, "")) 
        {
            printf("Phone number already exists! Try again.\n");
            continue;
        }

        break;  // valid and unique

    } while (1);

    // ---- EMAIL ----
    do 
    {
        printf("Enter email: ");
        scanf("%s", newContact.email);

        if (!validateEmail(newContact.email)) 
        {
            printf("Invalid email! Please try again.\n");
            continue;
        }

        if (isDuplicate(addressBook, "", newContact.email)) 
        {
            printf("Email already exists! Try again.\n");
            continue;
        }

        break;  // valid and unique

    } while (1);

    // ---- SAVE ----
    addressBook->contacts[addressBook->contactCount++] = newContact;
    printf("Contact created successfully!\n");
}

void searchContact(AddressBook *addressBook) 
{
    if (addressBook->contactCount == 0) 
    {
        printf("No contacts available to search.\n");
        return;
    }

    int choice;
    char input[50];
    int found = 0;

    while (1) 
    {
        printf("\nSearch by:\n");
        printf("1. Name\n");
        printf("2. Phone\n");
        printf("3. Email\n");
        printf("4. Exit Search\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) 
        {
            printf("Invalid input! Please enter a number (1-4).\n");
            while (getchar() != '\n'); // clear buffer
            continue; // back to search menu
        }

        if (choice == 4) 
        {
            printf("Exiting search menu.\n");
            break; // go back to main menu
        }

        if (choice == 1)  //Search by NAME
        {
            do 
            {
                printf("Enter the name: ");
                scanf(" %[^\n]", input);

                if (!validateName(input)) 
                {
                    printf("Invalid name! Try again.\n");
                }
            } while (!validateName(input));

            int matches[100];
            int matchCount = 0;

            for (int i = 0; i < addressBook->contactCount; i++) 
            {
                if (strcmp(addressBook->contacts[i].name, input) == 0) 
                {
                    matches[matchCount++] = i;
                }
            }

            if (matchCount == 0) 
            {
                printf("No contact found with name '%s'.\n", input);
                continue;   // back to search menu
            }

            if (matchCount == 1) 
            {
                int idx = matches[0];
                printf("\nContact found:\n");
                printf("Name : %s\n", addressBook->contacts[idx].name);
                printf("Phone: %s\n", addressBook->contacts[idx].phone);
                printf("Email: %s\n", addressBook->contacts[idx].email);
                continue;   // stay in search menu
            }

            printf("\nMultiple contacts found with name '%s':\n", input);
            for (int j = 0; j < matchCount; j++) 
            {
                int idx = matches[j];
                printf("%d) Phone: %s | Email: %s\n", j + 1, 
                       addressBook->contacts[idx].phone,
                       addressBook->contacts[idx].email);
            }

            printf("Enter phone or email to refine search: ");
            scanf("%s", input);

            int valid = (validatePhone(input) || validateEmail(input));
            if (!valid) 
            {
                printf("Invalid phone/email input!\n");
                continue;   // back to search menu
            }

            for (int j = 0; j < matchCount; j++) 
            {
                int idx = matches[j];
                if (strcmp(addressBook->contacts[idx].phone, input) == 0 ||
                    strcmp(addressBook->contacts[idx].email, input) == 0) 
                {
                    printf("\nContact found:\n");
                    printf("Name : %s\n", addressBook->contacts[idx].name);
                    printf("Phone: %s\n", addressBook->contacts[idx].phone);
                    printf("Email: %s\n", addressBook->contacts[idx].email);
                    break;
                }
            }
        }
        else if (choice == 2)  //Search by PHONE
        {
            do 
            {
                printf("Enter the phone: ");
                scanf("%s", input);
                if (!validatePhone(input)) 
                {
                    printf("Invalid phone number! Try again.\n");
                }
            } while (!validatePhone(input));

            found = 0;
            for (int i = 0; i < addressBook->contactCount; i++) 
            {
                if (strcmp(addressBook->contacts[i].phone, input) == 0) 
                {
                    printf("\nContact found:\n");
                    printf("Name : %s\n", addressBook->contacts[i].name);
                    printf("Phone: %s\n", addressBook->contacts[i].phone);
                    printf("Email: %s\n", addressBook->contacts[i].email);
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                printf("No contact found with phone '%s'.\n", input);
            }
        }
        else if (choice == 3)  //Search by EMAIL
        {
            do 
            {
                printf("Enter the email: ");
                scanf("%s", input);
                if (!validateEmail(input)) 
                {
                    printf("Invalid email! Try again.\n");
                }
            } while (!validateEmail(input));

            found = 0;
            for (int i = 0; i < addressBook->contactCount; i++) 
            {
                if (strcmp(addressBook->contacts[i].email, input) == 0) 
                {
                    printf("\nContact found:\n");
                    printf("Name : %s\n", addressBook->contacts[i].name);
                    printf("Phone: %s\n", addressBook->contacts[i].phone);
                    printf("Email: %s\n", addressBook->contacts[i].email);
                    found = 1;
                    break;
                }
            }
            if (!found)
            { 
                printf("No contact found with email '%s'.\n", input);
            }
        }
        else
        {
            printf("Invalid choice! Please enter 1-4.\n");
        }
    }
}

void editContact(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0)
    {
        printf("No contacts available to edit!\n");
        return;
    }

    int choice;
    do
    {
        printf("\nEdit Menu:\n");
        printf("1. Edit Name\n");
        printf("2. Edit Phone\n");
        printf("3. Edit Email\n");
        printf("4. Exit Edit Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number (1-4).\n");
            while (getchar() != '\n');
            continue;
        }
        getchar(); // consume newline

        if (choice == 4)
        {
            printf("Exiting edit menu.\n");
            break;
        }

        // --- List all contacts ---
        printf("\n===== Contacts =====\n");
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            printf("%d) Name: %s | Phone: %s | Email: %s\n",
                   i + 1,
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);
        }
        printf("====================\n");

        int index;
        printf("Enter index number to edit (0 to cancel): ");
        if (scanf("%d", &index) != 1)
        {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        if (index == 0)
        {
            printf("Cancelled editing.\n");
            continue;
        }
        if (index < 1 || index > addressBook->contactCount)
        {
            printf("Invalid contact number!\n");
            continue;
        }

        Contact *contact = &addressBook->contacts[index - 1];
        getchar(); // consume newline

        switch (choice)
        {
            case 1: // Edit Name
                do
                {
                    printf("Enter new name: ");
                    fgets(contact->name, sizeof(contact->name), stdin);
                    contact->name[strcspn(contact->name, "\n")] = 0; // remove newline

                    if (!validateName(contact->name))
                        printf("Invalid name! Try again.\n");
                    else
                        break;
                } while (1);
                printf("Name updated successfully!\n");
                break;

            case 2: // Edit Phone
                do
                {
                    char newPhone[20];   // temporary buffer
                    printf("Enter new phone: ");
                    scanf("%s", newPhone);

                    if (!validatePhone(newPhone))
                        printf("Invalid phone number! Try again.\n");
                    else if (isDuplicate(addressBook, newPhone, ""))
                        printf("Phone number already exists! Try again.\n");
                    else
                    {
                        strcpy(contact->phone, newPhone); // update only after validation
                        break;
                    }
                } while (1);
                printf("Phone updated successfully!\n");
                getchar(); // consume newline
                break;

            case 3: // Edit Email
                do
                {
                    char newEmail[50];   // temporary buffer
                    printf("Enter new email: ");
                    scanf("%s", newEmail);

                    if (!validateEmail(newEmail))
                        printf("Invalid email! Try again.\n");
                    else if (isDuplicate(addressBook, "", newEmail))
                        printf("Email already exists! Try again.\n");
                    else
                    {
                        strcpy(contact->email, newEmail); // update only after validation
                        break;
                    }
                } while (1);
                printf("Email updated successfully!\n");
                getchar(); // consume newline
                break;
                
            default:
                printf("Invalid choice! Try again.\n");
        }

    } while (choice != 4);
}

void deleteContact(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0)
    {
        printf("No contacts available to delete!\n");
        return;
    }

    int choice;
    char input[50];

    do
    {
        printf("\nDelete by:\n");
        printf("1. Name\n");
        printf("2. Phone\n");
        printf("3. Email\n");
        printf("4. Exit Delete Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        if (choice == 4)
        {
            printf("Exiting delete menu.\n");
            break;
        }

        if (choice < 1 || choice > 3)
        {
            printf("Invalid choice! Try again.\n");
            continue;
        }

        // --- Keep asking until value is valid ---
        while (1)
        {
            printf("Enter input: ");
            scanf(" %[^\n]", input);

            if (choice == 1 && !validateName(input))
            {
                printf("Invalid name! Try again.\n");
                continue;
            }
            if (choice == 2 && !validatePhone(input))
            {
                printf("Invalid phone! Try again.\n");
                continue;
            }
            if (choice == 3 && !validateEmail(input))
            {
                printf("Invalid email! Try again.\n");
                continue;
            }
            break;
        }

        // --- Find matches ---
        int matches[100], matchCount = 0;
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if ((choice == 1 && strcmp(addressBook->contacts[i].name, input) == 0) ||
                (choice == 2 && strcmp(addressBook->contacts[i].phone, input) == 0) ||
                (choice == 3 && strcmp(addressBook->contacts[i].email, input) == 0))
            {
                matches[matchCount++] = i;
            }
        }

        if (matchCount == 0)
        {
            printf("No matching contact found!\n");
            continue; // instead of return, stay in delete menu
        }

        int idx;
        if (matchCount == 1)
        {
            idx = matches[0];
        }
        else
        {
            printf("\nMultiple contacts found:\n");
            for (int i = 0; i < matchCount; i++)
            {
                int k = matches[i];
                printf("%d) Name: %s | Phone: %s | Email: %s\n",
                       i + 1,
                       addressBook->contacts[k].name,
                       addressBook->contacts[k].phone,
                       addressBook->contacts[k].email);
            }

            int opt;
            while (1)
            {
                printf("Enter which contact number to delete: ");
                if (scanf("%d", &opt) != 1)
                {
                    printf("Invalid input! Please enter a number.\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (opt >= 1 && opt <= matchCount)
                {
                    idx = matches[opt - 1];
                    break;
                }
                else
                {
                    printf("Invalid selection! Try again.\n");
                }
            }
        }

        // --- Delete by shifting ---
        for (int i = idx; i < addressBook->contactCount - 1; i++)
        {
            addressBook->contacts[i] = addressBook->contacts[i + 1];
        }
        addressBook->contactCount--;

        printf("Contact deleted successfully!\n");

    } while (choice != 4);
}
