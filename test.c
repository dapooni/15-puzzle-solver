#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_DEPTH 50 // Maximum depth for iterative deepening search

struct Node {
    int data;
	int x, y;
	struct Node* parent;
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
    struct Node* current = head;
    struct Node* prev = NULL;
    struct Node* next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    head = prev;
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
void printBoard(int board[4][4]) {
	printf("\n-----------------------------\n");
	printf("|");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf(" %4d |", board[i][j]);
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
	
	printf("\nEnter 0-15 numbers (with spaces) to fill the board:\n");
	for (i=0; i<16; i++){
		scanf("%d", &num);
		insert(&list, num);
	}
	system("cls||clear");
	int board[4][4];

	convertToList(list, board);
	printTable(board);
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

void IDSearch(struct Node* head, int table[4][4]){ 
    for (int depth=0; depth<MAX_DEPTH; depth++){
            if (DLSearch(head, depth)){
                return;
            }
    }
}

bool DLSearch(struct Node* head, int limit){ //return a solution or failure/cutoff
    return RecursiveDLS(head, limit);
}

bool RecursiveDLS(struct Node* head, int limit){
    /*cutoff_occurred? <- false
    if GOAL-TEST[problem](STATE[node]) then return SOLUTION(node)
    else if DEPTH[node] == limit then return cutoff
    else for each successor in EXPAND(node, problem) do
        result <- RECURSIVE-DLS(successor, problem, limit)
        if result == cutoff then cutoff_occurred? <- true
        else if result  failure then return result
    if cutoff_occurred? then return cutoff else return failure*/
}

bool isGoalState(Puzzle board) {
    int value = 1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == N - 1 && j == N - 1) {
                if (board.board[i][j] != 0)
                    return false;
            } else {
                if (board.board[i][j] != value)
                    return false;
                value++;
            }
        }
    }
    return true;
}

void ASearch(){
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
		printf("\t\t\t|\t\t           AI SEARCH STRATEGIES         \t\t|\n");
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
