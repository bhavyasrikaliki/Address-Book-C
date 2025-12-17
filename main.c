/*
Documentation:
Name : K. Bhavya
Description : This is a simple address book application that allows users to create, search, edit, delete, and list contacts
            It is a mini project in C language that stores and manages personal contact information such as name, phone number, and email.
            The main objective of the Address Book project is to create, search, edit, delete, and list contact details efficiently using file handling in C.
Date : 23-09-2025
sample I/O : 
Address Book Menu:
1. Create contact
2. Search contact
3. Edit contact
4. Delete contact
5. List all contacts
6. Save contacts
7. Exit
Enter your choice: 1
By using this menu user can perform various operations on address book

*/
#include<stdio.h>
#include<ctype.h>
#include "contact.h"
#include "file.h"   
#include "populate.h" 
int main() 
{
    int choice;
    AddressBook addressBook;
    initialize(&addressBook); // Initialize the address book
    //loadContactsFromFile(&addressBook); // Load existing contacts from file

    do 
    {
        printf("\nAddress Book Menu:\n");
        printf("1. Create contact\n");
        
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
    	printf("6. Save contacts\n");	
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) 
        {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:          
                listContacts(&addressBook);
                break;
            case 6:
                printf("Saving......\n");
                saveContactsToFile(&addressBook);
                break;
            case 7:
                printf("Exit...\n");
                saveContactsToFile(&addressBook);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);
    
       return 0;
}
