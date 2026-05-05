#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// File structure (Linked List node)
struct File {
    char name[50];
    char type[20];       // Document, Image, Video
    int size;
    char sizeType[5];    // KB or MB
    struct File* next;
};

// Main list
struct File* head = NULL;

// Stack for undo delete
struct File* deletedStack = NULL;

// Queue for recent files
struct File* front = NULL;
struct File* rear = NULL;

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
void addFile(char name[], char type[], int size, char sizeType[]) {
    struct File* newFile = (struct File*)malloc(sizeof(struct File));

    strcpy(newFile->name, name);
    strcpy(newFile->type, type);
    newFile->size = size;
    strcpy(newFile->sizeType, sizeType);

    newFile->next = head;
    head = newFile;
}

// Display all files
void viewFiles() {
    struct File* temp = head;

    if (temp == NULL) {
        printf("No files.\n");
        return;
    }

    while (temp != NULL) {
        printf("Name: %s | Type: %s | Size: %d %s\n",
               temp->name, temp->type,
               temp->size, temp->sizeType);
        temp = temp->next;
    }
}

// Search file (Linear Search)
void searchFile(char name[]) {
    struct File* temp = head;

    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("Found: %s (%s) - %d %s\n",
                   temp->name, temp->type,
                   temp->size, temp->sizeType);

            addRecent(temp); // add to queue
            return;
        }
        temp = temp->next;
    }

    printf("File not found.\n");
}

// Delete file and push to stack
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

// Undo delete (pop from stack)
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

// Sort files (Bubble Sort using array)
void sortFiles(int bySize) {
    int count = 0, i, j;
    struct File* temp = head;

    // count nodes
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    if (count < 2) return;

    struct File* arr[count];
    temp = head;

    // store in array
    for (i = 0; i < count; i++) {
        arr[i] = temp;
        temp = temp->next;
    }

    // bubble sort
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

    // display sorted
    printf("Sorted Files:\n");
    for (i = 0; i < count; i++) {
        printf("%s (%s) - %d %s\n",
               arr[i]->name, arr[i]->type,
               arr[i]->size, arr[i]->sizeType);
    }
}

// MAIN MENU
int main() {
    int choice, size, sortChoice;
    char name[50], type[20], sizeType[5];

    while (1) {
        printf("\n1.Add File\n2.View Files\n3.Search\n4.Delete\n5.Undo\n6.Sort\n7.Recent\n0.Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Name: "); scanf("%s", name);
                printf("Type (Document/Image/Video/Audio): "); scanf("%s", type);
                printf("Size: "); scanf("%d", &size);
                printf("Size type (KB/MB): "); scanf("%s", sizeType);
                addFile(name, type, size, sizeType);
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
