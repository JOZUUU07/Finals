#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// File structure (Linked List node)
struct File {
    char name[50];
    int size;              // file size in MB
    char date[20];         // automatically generated date
    struct File* next;
};

// Main linked list
struct File* head = NULL;

// Stack for undo delete
struct File* deletedStack = NULL;

// Queue for recent files
struct File* front = NULL;
struct File* rear = NULL;

// Get current date automatically
void getCurrentDate(char date[]) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    sprintf(date, "%04d-%02d-%02d",
            tm.tm_year + 1900,
            tm.tm_mon + 1,
            tm.tm_mday);
}

// Check if file already exists (Linear Search)
int fileExists(char name[]) {

    struct File* temp = head;

    while (temp != NULL) {

        if (strcmp(temp->name, name) == 0) {
            return 1;
        }

        temp = temp->next;
    }

    return 0;
}

// Add file to recent queue
void addRecent(struct File* file) {

    struct File* temp = (struct File*)malloc(sizeof(struct File));

    *temp = *file;
    temp->next = NULL;

    if (rear == NULL) {
        front = rear = temp;
    } else {
        rear->next = temp;
        rear = temp;
    }
}

// Display recent files
void viewRecent() {

    struct File* temp = front;

    if (temp == NULL) {
        printf("No recent files.\n");
        return;
    }

    printf("\nRecent Files:\n");

    while (temp != NULL) {

        printf("Name: %s | Size: %d MB | Date: %s\n",
               temp->name,
               temp->size,
               temp->date);

        temp = temp->next;
    }
}

// Add file to linked list
void addFile(char name[], int size) {

    // Validation
    if (strlen(name) == 0) {
        printf("Error: File name cannot be empty.\n");
        return;
    }

    if (size <= 0) {
        printf("Error: Size must be greater than 0.\n");
        return;
    }

    if (fileExists(name)) {
        printf("Error: File already exists.\n");
        return;
    }

    struct File* newFile = (struct File*)malloc(sizeof(struct File));

    strcpy(newFile->name, name);
    newFile->size = size;

    // Automatically get current date
    getCurrentDate(newFile->date);

    // Insert at head
    newFile->next = head;
    head = newFile;

    printf("File added successfully!\n");
}

// Display all files
void viewFiles() {

    struct File* temp = head;

    if (temp == NULL) {
        printf("No files.\n");
        return;
    }

    printf("\nAll Files:\n");

    while (temp != NULL) {

        printf("Name: %s | Size: %d MB | Date: %s\n",
               temp->name,
               temp->size,
               temp->date);

        temp = temp->next;
    }
}

// Search file using Linear Search
void searchFile(char name[]) {

    struct File* temp = head;

    while (temp != NULL) {

        if (strcmp(temp->name, name) == 0) {

            printf("Found: %s | Size: %d MB | Date: %s\n",
                   temp->name,
                   temp->size,
                   temp->date);

            // Add to recent queue
            addRecent(temp);

            return;
        }

        temp = temp->next;
    }

    printf("File not found.\n");
}

// Delete file
void deleteFile(char name[]) {

    struct File *temp = head, *prev = NULL;

    while (temp != NULL) {

        if (strcmp(temp->name, name) == 0) {

            // Remove from linked list
            if (prev == NULL)
                head = temp->next;
            else
                prev->next = temp->next;

            // Push deleted file to stack
            temp->next = deletedStack;
            deletedStack = temp;

            printf("File deleted.\n");
            return;
        }

        prev = temp;
        temp = temp->next;
    }

    printf("File not found.\n");
}

// Undo delete using stack
void undoDelete() {

    if (deletedStack == NULL) {
        printf("Nothing to undo.\n");
        return;
    }

    // Pop from stack
    struct File* temp = deletedStack;
    deletedStack = deletedStack->next;

    // Return to linked list
    temp->next = head;
    head = temp;

    printf("Undo successful.\n");
}

// Sort files using Bubble Sort
void sortFiles(int choice) {

    int count = 0, i, j;
    struct File* temp = head;

    // Count files
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    if (count < 2) {
        printf("Not enough files to sort.\n");
        return;
    }

    // Store linked list into array
    struct File* arr[count];

    temp = head;

    for (i = 0; i < count; i++) {
        arr[i] = temp;
        temp = temp->next;
    }

    // Bubble Sort
    for (i = 0; i < count - 1; i++) {

        for (j = 0; j < count - i - 1; j++) {

            int swap = 0;

            // Sort by name
            if (choice == 1) {

                if (strcmp(arr[j]->name, arr[j + 1]->name) > 0)
                    swap = 1;
            }

            // Sort by size
            else if (choice == 2) {

                if (arr[j]->size > arr[j + 1]->size)
                    swap = 1;
            }

            // Sort by date
            else if (choice == 3) {

                if (strcmp(arr[j]->date, arr[j + 1]->date) > 0)
                    swap = 1;
            }

            // Swap elements
            if (swap) {

                struct File* t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
        }
    }

    // Display sorted files
    printf("\nSorted Files:\n");

    for (i = 0; i < count; i++) {

        printf("Name: %s | Size: %d MB | Date: %s\n",
               arr[i]->name,
               arr[i]->size,
               arr[i]->date);
    }
}

// Main Menu
int main() {

    int choice, size, sortChoice;
    char name[50];

    while (1) {

        printf("\n===== FILE ORGANIZER SYSTEM =====\n");
        printf("1. Add File\n");
        printf("2. View Files\n");
        printf("3. Search File\n");
        printf("4. Delete File\n");
        printf("5. Undo Delete\n");
        printf("6. Sort Files\n");
        printf("7. View Recent Files\n");
        printf("0. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:

                printf("Enter file name: ");
                scanf("%s", name);

                printf("Enter file size (MB): ");
                scanf("%d", &size);

                addFile(name, size);
                break;

            case 2:

                viewFiles();
                break;

            case 3:

                printf("Enter file name to search: ");
                scanf("%s", name);

                searchFile(name);
                break;

            case 4:

                printf("Enter file name to delete: ");
                scanf("%s", name);

                deleteFile(name);
                break;

            case 5:

                undoDelete();
                break;

            case 6:

                printf("\nSort By:\n");
                printf("1. Name\n");
                printf("2. Size\n");
                printf("3. Date\n");

                printf("Enter choice: ");
                scanf("%d", &sortChoice);

                sortFiles(sortChoice);
                break;

            case 7:

                viewRecent();
                break;

            case 0:

                printf("Exiting program...\n");
                return 0;

            default:

                printf("Invalid choice.\n");
        }
    }
}
