#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct contact {
    char name[50];
    char phone[20];
    char email[50];
};

void addContact(void);
void viewContacts(void);
void searchContact(void);
void deleteContact(void);
void updateContact(void);

int main(void) {
    int choice;

    while (1) {
        printf("\n----------------------------------\n");
        printf("        CONTACT MANAGEMENT        \n");
        printf("----------------------------------\n");
        printf("1. Add New Contact\n");
        printf("2. View All Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Update Contact\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            /* clear invalid input */
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("Please enter a number between 1 and 6.\n");
            continue;
        }
        getchar(); /* clear newline */

        switch (choice) {
            case 1: addContact(); break;
            case 2: viewContacts(); break;
            case 3: searchContact(); break;
            case 4: deleteContact(); break;
            case 5: updateContact(); break;
            case 6: exit(0);
            default: printf("Please enter a valid option.\n");
        }
    }

    return 0;
}

void addContact(void) {
    FILE *fp = fopen("contacts.txt", "a");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    struct contact c;

    printf("\nEnter Name: ");
    if (fgets(c.name, sizeof(c.name), stdin) == NULL) return;
    c.name[strcspn(c.name, "\n")] = '\0';

    /* Phone number must be exactly 10 digits */
    while (1) {
        printf("Enter Phone Number (10 digits): ");
        if (fgets(c.phone, sizeof(c.phone), stdin) == NULL) return;
        c.phone[strcspn(c.phone, "\n")] = '\0';

        /* Check length exactly 10 and all digits */
        int len = strlen(c.phone);
        int all_digits = 1;
        if (len != 10) all_digits = 0;
        else {
            for (int i = 0; i < len; i++) {
                if (c.phone[i] < '0' || c.phone[i] > '9') {
                    all_digits = 0;
                    break;
                }
            }
        }

        if (all_digits) break;
        else printf("Invalid input. Phone number must be exactly 10 digits.\n");
    }

    printf("Enter Email: ");
    if (fgets(c.email, sizeof(c.email), stdin) == NULL) return;
    c.email[strcspn(c.email, "\n")] = '\0';

    fprintf(fp, "%s;%s;%s\n", c.name, c.phone, c.email);
    fclose(fp);

    printf("Contact saved successfully.\n");
}

void viewContacts(void) {
    FILE *fp = fopen("contacts.txt", "r");
    struct contact c;

    if (fp == NULL) {
        printf("\nNo saved contacts found.\n");
        return;
    }

    printf("\n--- Saved Contacts ---\n");

    while (fscanf(fp, "%49[^;];%19[^;];%49[^\n]\n", c.name, c.phone, c.email) == 3) {
        printf("Name  : %s\n", c.name);
        printf("Phone : %s\n", c.phone);
        printf("Email : %s\n\n", c.email);
    }

    fclose(fp);
}

void searchContact(void) {
    FILE *fp = fopen("contacts.txt", "r");
    struct contact c;
    char name[50];
    int found = 0;

    if (fp == NULL) {
        printf("\nNo contacts available to search.\n");
        return;
    }

    printf("\nEnter the name you want to search: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
        fclose(fp);
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    while (fscanf(fp, "%49[^;];%19[^;];%49[^\n]\n", c.name, c.phone, c.email) == 3) {
        if (strcmp(c.name, name) == 0) {
            printf("\nContact Found:\n");
            printf("Name  : %s\n", c.name);
            printf("Phone : %s\n", c.phone);
            printf("Email : %s\n", c.email);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No contact found with that name.\n");
    }

    fclose(fp);
}

void deleteContact(void) {
    FILE *fp = fopen("contacts.txt", "r");
    if (fp == NULL) {
        printf("\nNo contacts available to delete.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        fclose(fp);
        printf("Error creating temporary file.\n");
        return;
    }

    struct contact c;
    char name[50];
    int found = 0;

    printf("\nEnter the name of the contact to delete: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
        fclose(fp);
        fclose(temp);
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    while (fscanf(fp, "%49[^;];%19[^;];%49[^\n]\n", c.name, c.phone, c.email) == 3) {
        if (strcmp(c.name, name) != 0) {
            fprintf(temp, "%s;%s;%s\n", c.name, c.phone, c.email);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    /* Replace original file with temp file */
    remove("contacts.txt");
    rename("temp.txt", "contacts.txt");

    if (found) {
        printf("Contact deleted successfully.\n");
    } else {
        printf("No contact found with that name.\n");
    }
}

void updateContact(void) {
    FILE *fp = fopen("contacts.txt", "r");
    if (fp == NULL) {
        printf("\nNo contacts available to update.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        fclose(fp);
        printf("Error creating temporary file.\n");
        return;
    }

    struct contact c;
    char name[50];
    int found = 0;

    printf("\nEnter the name of the contact to update: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
        fclose(fp);
        fclose(temp);
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    while (fscanf(fp, "%49[^;];%19[^;];%49[^\n]\n", c.name, c.phone, c.email) == 3) {
        if (strcmp(c.name, name) == 0) {
            found = 1;

            printf("Enter New Name: ");
            if (fgets(c.name, sizeof(c.name), stdin) == NULL) {
                /* keep old if input fails */
            } else {
                c.name[strcspn(c.name, "\n")] = '\0';
            }

            /* Phone number validation */
            while (1) {
                printf("Enter New Phone Number (10 digits): ");
                if (fgets(c.phone, sizeof(c.phone), stdin) == NULL) break;
                c.phone[strcspn(c.phone, "\n")] = '\0';

                int len = strlen(c.phone);
                int all_digits = 1;
                if (len != 10) all_digits = 0;
                else {
                    for (int i = 0; i < len; i++) {
                        if (c.phone[i] < '0' || c.phone[i] > '9') {
                            all_digits = 0;
                            break;
                        }
                    }
                }

                if (all_digits) break;
                else printf("Invalid input. Phone number must be exactly 10 digits.\n");
            }

            printf("Enter New Email: ");
            if (fgets(c.email, sizeof(c.email), stdin) == NULL) {
                /* keep old if input fails */
            } else {
                c.email[strcspn(c.email, "\n")] = '\0';
            }
        }

        fprintf(temp, "%s;%s;%s\n", c.name, c.phone, c.email);
    }

    fclose(fp);
    fclose(temp);

    remove("contacts.txt");
    rename("temp.txt", "contacts.txt");

    if (found) {
        printf("Contact updated successfully.\n");
    } else {
        printf("No contact found with that name.\n");
    }
}