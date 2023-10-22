#include <stdio.h>
#include <stdlib.h>

// Define the singly linked list node structure
struct Node {
    int data;
    struct Node* next;
};

// Create a function to insert elements into the linked list
void insert(struct Node** head, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = *head;
    *head = newNode;
}

// Create a function to convert the linked list into a 4x4 table
void convertToList(struct Node* head, int table[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (head != NULL) {
                table[i][j] = head->data;
                head = head->next;
            }
        }
    }
}

// Create a function to print the 4x4 table
void printTable(int table[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%4d ", table[i][j]);
        }
        printf("\n");
    }
}

int main() {
    struct Node* list = NULL;

    // Insert elements into the linked list (you should have exactly 16 elements)
    for (int i = 16; i >= 1; i--) {
        insert(&list, i);
    }

    int table[4][4];

    // Convert the linked list into a 4x4 table
    convertToList(list, table);

    // Print the 4x4 table
    printTable(table);

    return 0;
}
