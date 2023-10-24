#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_DEPTH 50

typedef struct{
    int board[4][4];
	int x, y;
	struct Node* parent;
    struct Node* next;
} PuzzleState;

// Create a function to print the 4x4 table
void printTable(PuzzleState state) {
	printf("\n-----------------------------\n");
	printf("|");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf(" %4d |", state.board[i][j]);
        }
        printf("\n-----------------------------");
		printf("\n|");
    }
	printf("\tInitial State\t    |\n");
}

PuzzleState configureBoard(PuzzleState state){
	int num, i, j;

	printf("Input initial board configuration numbers below:\n");
	printf("\n-----------------------------\n");
	printf("|");
	num=0;
	for (i=0;i<4;i++){
		for (j=0;j<4;j++){
			printf(" %4d |", state.board[i][j]);
		}
		printf("\n-----------------------------");
		printf("\n|");
	}
	printf("\tInitial State\t    |\n");
	
	printf("\nEnter 0-15 numbers (with spaces) to fill the board:\n");
	for (i=0;i<4;i++){
		for (j=0;j<4;j++){
			scanf("%d.", &state.board[i][j]);
        }
    }
	system("cls||clear");

	printTable(state);
    return state;
}

void modifyBoard(){
	int choice0;
    PuzzleState initial;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            initial.board[i][j] =  0;
        }
    }
	configureBoard(initial);
	do{
		printf("\nWant to configure board again?");
		printf("\n[1] Yes\t\t[2] No/Continue\n\n");
		scanf("%d", &choice0);
		if (choice0==1){
			system("cls||clear");
			configureBoard(initial);
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

void idsSearch(PuzzleState initial){
	for (int depth = 0; depth < MAX_DEPTH; depth++) {
        if (depthLimitedSearch(initial, depth)) {
            return;
        }
    }
    printf("No solution found within the depth limit.\n");
}

bool DLSearch(PuzzleState state, int limit){ //return a solution or failure/cutoff
    return RecursiveDLS(state, limit);
}

bool RecursiveDLS(PuzzleState state, int limit){
    /*cutoff_occurred? <- false
    if GOAL-TEST[problem](STATE[node]) then return SOLUTION(node)
    else if DEPTH[node] == limit then return cutoff
    else for each successor in EXPAND(node, problem) do
        result <- RECURSIVE-DLS(successor, problem, limit)
        if result == cutoff then cutoff_occurred? <- true
        else if result  failure then return result
    if cutoff_occurred? then return cutoff else return failure*/
}

bool isGoalState(PuzzleState state) {
    int value = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == 4 - 1 && j == 4 - 1) {
                if (state.board[i][j] != 0)
                    return false;
            } else {
                if (state.board[i][j] != value)
                    return false;
                value++;
            }
        }
    }
    return true;
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
