#include <stdio.h>
#include <stdlib.h>

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
	printf("\n-----------------------------\n");
	printf("|");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf(" %4d |", table[i][j]);
        }
        printf("\n-----------------------------");
		printf("\n|");
    }
	printf("\tInitial State\t    |\n");
}

void configureBoard(){
	struct Node* list = NULL;
	int num, i, j;

	printf("Input initial board configuration numbers below:\n");
	printf("\n-----------------------------\n");
	printf("|");
	num=0;
	for (i=0;i<4;i++){
		for (j=0;j<4;j++){
			printf(" %4d |", num);
		}
		printf("\n-----------------------------");
		printf("\n|");
	}
	printf("\tInitial State\t    |\n");
	
	printf("\nEnter 16 numbers (with spaces) to fill the board:\n");
	for (i=0; i<16; i++){
		scanf("%d", &num);
		insert(&list, num);
	}
	system("cls||clear");
	int table[4][4];

	convertToList(list, table);
	printTable(table);
}

void modifyBoard(){
	int choice0;
	configureBoard();
	do{
		printf("\nWant to configure board again?");
		printf("\n[1] Yes\t\t[2] No/Continue\n\n");
		scanf("%d", &choice0);
		if (choice0==1){
			system("cls||clear");
			configureBoard();
		}
		else if (choice0==2){
			system("cls||clear");
			break;
		}
		else {
			printf("Invalid Input");
		}
	} while (choice0 != 'n');
}

void idsSearch(){
	//code here
}

void aSearch(){
	//code here
}

int main(){
	int choice1;
	struct Node* list = NULL;
	int num, i, j;
	
	modifyBoard();
	do	{
		//system("cls||clear");   //This function is used to run system/command prompt commands and here cls is a command to clear the output screen for both windows and linux
		printf("\t\t\t-------------------------------------------------------------------\n");
		printf("\t\t\t|\t\t          AI SEARCH STRATEGIES         \t\t|\n");
		printf("\t\t\t-------------------------------------------------------------------\n");
		printf("\t\t\t|\t\t\t\t\t\t\t\t  |\n");
		printf("\t\t\t|0.) Modify Board  \t\t\t\t\t\t  |\n");
		printf("\t\t\t|1.) Iterative Deepening Search \t\t\t\t  |\n");
		printf("\t\t\t|2.) A* search\t\t\t\t\t\t\t  |\n");
		printf("\t\t\t|3.) End Program \t\t\t\t\t\t  |\n");
		printf("\t\t\t-------------------------------------------------------------------\n");
		
		printf("\nEnter your number choice: ");
		scanf("%d", &choice1); 
			
		switch(choice1){
			case 0:
				modifyBoard();
			case 1:
                //idsSearch();
				break;
			case 2:
				//aSearch();
				break;
			case 3:
				return 0;
			default:
				getchar();
			  	printf("\nInvalid. Please try again.");
				printf("\n\n");
				printf("\nPress enter to continue...");
				fflush(stdout);
				getchar(); 			  			
				break;	
		}
	} while(choice1 != 3);
}    
