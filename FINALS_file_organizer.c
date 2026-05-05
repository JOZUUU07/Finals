#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// File structure (Linked List node)
struct File {
    char name[50];
    char type[20];   // Document, Image, Video, Audio
    int size;        // always in MB
    struct File* next;
};

// Main list
struct File* head = NULL;

// Stack for undo delete
struct File* deletedStack = NULL;

// Queue for recent files
struct File* front = NULL;
struct File* rear = NULL;

// ? Check if file already exists
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

// ? Validate file type
int isValidType(char type[]) {
    return strcmp(type, "Document") == 0 ||
           strcmp(type, "Image") == 0 ||
           strcmp(type, "Video") == 0 ||
           strcmp(type, "Audio") == 0;
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

    while (temp != NULL) {
        printf("%s (%s)\n", temp->name, temp->type);
        temp = temp->next;
    }
}

// Add file to linked list
void addFile(char name[], char type[], int size) {

    // ? Validation checks
    if (strlen(name) == 0) {
        printf("Error: File name cannot be empty.\n");
        return;
    }

    if (!isValidType(type)) {
        printf("Error: Invalid file type. Use Document/Image/Video/Audio only.\n");
        return;
    }

    if (size <= 0) {
        printf("Error: Size must be greater than 0.\n");
        return;
    }

    if (fileExists(name)) {
        printf("Error: File with this name already exists.\n");
        return;
    }

    struct File* newFile = (struct File*)malloc(sizeof(struct File));

    strcpy(newFile->name, name);
    strcpy(newFile->type, type);
    newFile->size = size;

    newFile->next = head;
    head = newFile;

    // ? Success message
    printf("File added successfully!\n");
}

// Display all files
void viewFiles() {
    struct File* temp = head;

    if (temp == NULL) {
        printf("No files.\n");
        return;
    }

    while (temp != NULL) {
        printf("Name: %s | Type: %s | Size: %d MB\n",
               temp->name, temp->type, temp->size);
        temp = temp->next;
    }
}

// Search file
void searchFile(char name[]) {
    struct File* temp = head;

    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("Found: %s (%s) - %d MB\n",
                   temp->name, temp->type, temp->size);

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

            if (prev == NULL)
                head = temp->next;
            else
                prev->next = temp->next;

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

// Undo delete
void undoDelete() {
    if (deletedStack == NULL) {
        printf("Nothing to undo.\n");
        return;
    }

    struct File* temp = deletedStack;
    deletedStack = deletedStack->next;

    temp->next = head;
    head = temp;

    printf("Undo successful.\n");
}

// Sort files
void sortFiles(int bySize) {
    int count = 0, i, j;
    struct File* temp = head;

    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    if (count < 2) return;

    struct File* arr[count];
    temp = head;

    for (i = 0; i < count; i++) {
        arr[i] = temp;
        temp = temp->next;
    }

    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {

            if (bySize) {
                if (arr[j]->size > arr[j+1]->size) {
                    struct File* t = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = t;
                }
            } else {
                if (strcmp(arr[j]->name, arr[j+1]->name) > 0) {
                    struct File* t = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = t;
                }
            }
        }
    }

    printf("Sorted Files:\n");
    for (i = 0; i < count; i++) {
        printf("%s (%s) - %d MB\n",
               arr[i]->name, arr[i]->type, arr[i]->size);
    }
}

// MAIN MENU
int main() {
    int choice, size, sortChoice;
    char name[50], type[20];

    while (1) {
        printf("\n1.Add File\n2.View Files\n3.Search\n4.Delete\n5.Undo\n6.Sort\n7.Recent\n0.Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Name: "); scanf("%s", name);
                printf("Type (Document/Image/Video/Audio): "); scanf("%s", type);
                printf("Size (MB): "); scanf("%d", &size);
                addFile(name, type, size);
                break;

            case 2:
                viewFiles();
                break;

            case 3:
                printf("Search name: "); scanf("%s", name);
                searchFile(name);
                break;

            case 4:
                printf("Delete name: "); scanf("%s", name);
                deleteFile(name);
                break;

            case 5:
                undoDelete();
                break;

            case 6:
                printf("1.Name  2.Size: ");
                scanf("%d", &sortChoice);
                sortFiles(sortChoice == 2);
                break;

            case 7:
                viewRecent();
                break;

            case 0:
                return 0;
        }
    }
}
