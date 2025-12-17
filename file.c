#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "contact.h"

#define FILENAME "contacts.csv"

// Save contacts into CSV file
void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fp = fopen(FILENAME, "w");   // open file in write mode
    if (fp == NULL) 
    {
        printf("Error opening %s for saving!\n", FILENAME);
        return;
    }

    fprintf(fp, "%d\n", addressBook->contactCount); // write total number of contacts

    // write each contact: name, phone, email
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        fprintf(fp, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fp); // close file
    printf("Contacts saved to %s\n", FILENAME);
}

// Load contacts from CSV file
void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fp = fopen(FILENAME, "r");   // open file in read mode
    
    if (fp == NULL) 
    {
        if(addressBook->contactCount == 0)
        {
            printf("No contacts found. Starting new address book.\n");
        }
        addressBook->contactCount = 0;        // start empty
        return;
    }

    // Read number of contacts safely
    if (fscanf(fp, "%d\n", &addressBook->contactCount) != 1) 
    {
        printf("Error reading contact count. Starting empty.\n");
        addressBook->contactCount = 0;
        fclose(fp);
        return;
    }

    // Prevent overflow if file has more than MAX_CONTACTS
    if (addressBook->contactCount > MAX_CONTACTS) 
    {
        printf("File has too many contacts! Truncating to %d.\n", MAX_CONTACTS);
        addressBook->contactCount = MAX_CONTACTS;
    }

    // Read each contact safely
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if (fscanf(fp, "%49[^,],%19[^,],%49[^\n]\n",
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email) != 3) 
        {
            printf("Error reading contact at line %d. Stopping load.\n", i + 2);
            addressBook->contactCount = i; // keep only what was read
            break;
        }
    }

    fclose(fp); // close file
    printf("Loaded %d contacts from %s\n", addressBook->contactCount, FILENAME);
}