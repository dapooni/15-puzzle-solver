#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//define constants for the puzzle size, the empty slot value, and the maximum depth for search
#define SIZE 4
#define MAX_DEPTH 50

//define a PuzzleState structure that represents the state of the puzzle with the board and empty slot's position
typedef struct {
    int board[SIZE][SIZE];
    int x, y;  //empty slot's position
} PuzzleState;

PuzzleState initial;  //global variable to hold the initial state of the puzzle

//define a Node structure for search algorithms, containing state, parent, action, costs, and a pointer to the next node
typedef struct Node {
    PuzzleState state; //store the current puzzle state in the node
    struct Node* parent; //a pointer to the parent node. This helps to trace back the actions taken to reach the current state.
    char action; //the action that was taken to reach the current state from its parent (e.g., 'U', 'D', 'L', 'R')
    int cost;  //the cumulative cost to reach this state from the initial state. This will be the number of moves taken.
    int totalCost; //the cumulative cost plus the heuristic cost ( used in algorithms A*). 
    			  //this helps to estimate the total cost to reach the goal from the current state.
    struct Node* next; //a pointer to the next node
} Node;

//function to print the current configuration of the sliding puzzle.
void printBoard(PuzzleState state){
    int i, j;  //loop counters for rows and columns.
    //print a label indicating that this is the initial state.
	printf("\t\t\t    Initial State\t");
    //print the top border of the table.
	printf("\n\t\t    -----------------------------\n");

    //loop through each row of the puzzle.
    for (i = 0; i < 4; i++) {
        //start a new row with a vertical border.
		printf("\t\t    |");

        //loop through each column of the current row.
        for (j = 0; j < 4; j++) 
        {
            //print the value in the current cell, right-aligned in a width of 4 spaces.
            printf(" %4d |", state.board[i][j]);
        }

        //print the bottom border of the current row.
        printf("\n\t\t    -----------------------------\n");
    }
}
//determine if a move to a specific position is valid (within the bounds of the board)
int isValidMove(int x, int y) {
    // printf("%d", x);
    // printf("%d", y);
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE)
        return 0;  // False: The position is outside the grid boundaries
    return 1;      // True: The position is inside the grid boundaries
}

//generate a successor state/node from the current state based on a given move (U, D, L, R)
Node* generateSuccessor(Node* currNode, char move) {
    int dx = 0, dy = 0;
    
    // printf("%c", move);
    //switch statement to determine the direction of movement based on the 'move' character.
    switch (move) {
        case 'U': dx = -1; break; //if move is 'U', set dx to -1 to move one row up.
        case 'D': dx = 1; break;  //if move is 'D', set dx to 1 to move one row down.
        case 'L': dy = -1; break; //if move is 'L', set dy to -1 to move one column to the left.
        case 'R': dy = 1; break;  //if move is 'R', set dy to 1 to move one column to the right.
    }

    int newX = currNode->state.x + dx; //compute the new x (row) position for the empty slot by adding dx to the current x position.
    int newY = currNode->state.y + dy; //compute the new y (column) position for the empty slot by adding dy to the current y position.
//check if the attempted move (resulting in newX, newY) is within the valid bounds of the board
    // printf("x: %d\n", dx);
    // printf("y: %d\n", dy);
    if (isValidMove(newX, newY)) {
        PuzzleState newState = currNode->state; 	//copy the current state to create a new modified state
        newState.board[currNode->state.x][currNode->state.y] = newState.board[newX][newY]; //swap the empty slot with the adjacent slot based on the move
		newState.board[newX][newY] = 0; 	//set the moved tile's value to empty, completing the swap
        
		newState.x = newX; //update the x and y coordinates in the new state to represent the new empty slot's position
        newState.y = newY;
		
        Node* newNode = malloc(sizeof(Node)); //allocate memory for a new node to represent the new state in search tree/path
		newNode->state = newState; //set the new node's state to the modified state
		newNode->parent = currNode; //link the new node to its parent (the node/state from which it was generated)
		newNode->action = move; //record the action/move ('U', 'D', 'L', 'R') that was used to generate this new state
		newNode->cost = currNode->cost + 1; //update the cost. Here, every move has a uniform cost of 1, so the new node's cost is its parent's cost + 1
        return newNode; //return the newly created node representing the new state
    }
    return NULL;  //if the move was not valid (i.e., it resulted in a position outside the board's bounds), return NULL
}

//Heuristic (Manhattan distance)
int heuristic(PuzzleState state){
    int distance = 0;
    int i, j;
    
    //nested for-loop to iterate through each tile in the puzzle.
    for (i = 0; i < SIZE; i++) 
	{
        for (j = 0; j < SIZE; j++) 
		{
            int value = state.board[i][j]; // Retrieve the value of the tile at position [i][j].
            if (value != 0)  //check if the tile is not the empty slot (value != 0).
			{
				 //calculate the target (goal) x-coordinate (row) for the current tile.
                // The formula determines the correct row based on the tile's value.
                int targetX = (value - 1) / SIZE;
                
                //calculate the target (goal) y-coordinate (column) for the current tile.
                //the formula determines the correct column based on the tile's value.
                int targetY = (value - 1) % SIZE;
                
                //increment the distance by the Manhattan distance for the current tile.
                //this is the sum of the absolute differences between the current position and the target position for both rows and columns.
                distance += abs(i - targetX) + abs(j - targetY);
            }
        }
    }
    return distance; //return the total calculated Manhattan distance for the entire board.
}

bool isGoalState(PuzzleState state)  //checks if the given puzzle state is the goal state
{
    int value = 0; //used to check if the tiles in the puzzle are in the correct ascending order.
    int i, j;
    
    //nested for-loop to iterate through each tile in the puzzle.
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
			//check if the current tile is the last tile in the puzzle.
            if (state.board[i][j]!=value) {
				//if the last tile is not the empty slot (value 0), the puzzle state is not the goal state. Return false.
                if (state.board[i][j] != 0) 
				return false;
            } 
			value++;
        }
    }
    return true; //return true, indicating that the puzzle state is the goal state.
}

// the Iterative Deepening Search (IDS) function.
void IDS(PuzzleState initial) {
    
    //capture the starting clock time to measure the performance of the search.
    clock_t start, end;
    start = clock();
    
    int searchCost = 0;  //initialize the search cost (number of nodes expanded).
    int i, depth;  //declare loop counter i and search depth.
    
    char moves[4] = {'U', 'D', 'L', 'R'}; //possible moves: Up, Down, Left, Right.

    //iterate over increasing depths up to a maximum defined depth (MAX_DEPTH).
    for (depth = 0; depth < MAX_DEPTH; depth++) {
        //using an array as a simple stack for depth-first traversal. 
        Node* stack[MAX_DEPTH * MAX_DEPTH];
        
        int top = -1;  //initialize the stack pointer.

        //create the initial node using the provided puzzle state.
        Node* initialNode = malloc(sizeof(Node));
        initialNode->state = initial;
        initialNode->parent = NULL;
        initialNode->action = ' ';  // No action needed for the initial state.
        initialNode->cost = 0;
        
        //push the initial node onto the stack.
        stack[++top] = initialNode;
        
        while (top != -1) {
            Node* currNode = stack[top--]; //pop the current node from the stack.
            
            //if the current node's state is the goal state.
            if (isGoalState(currNode->state)) { 
                end = clock();  //capture the end time.
                double time_taken = ((double)end - start) / CLOCKS_PER_SEC;

                printf("Solution path: ["); //print the solution path.
                while (currNode->parent) {
                    printf("%c -", currNode->action);
                    currNode = currNode->parent;
                }
                printf("]\n");

                printf("Solution cost: %d\n", depth);
                printf("Search cost: %d\n", searchCost);
                printf("Running time: %f seconds\n\n", time_taken);
                printBoard(currNode->state);
                return;
            }
            
            //if the cost is less than the current depth, expand the current node.
            if (currNode->cost < depth) {
                for (i = 0; i < 4; i++) {
                    //generate a successor for the current move direction.
                    Node* successor = generateSuccessor(currNode, moves[i]);
                    if (successor) {
                        //if the successor is valid, push it onto the stack.
                        stack[++top] = successor;
                        searchCost++;  //increment the search cost.
                    }
                }
            }
        }
    }
    //if the loop completes without finding a solution, inform the user.
    printf("No solution found within the depth limit.\n");
}

// A* Search algorithm function for the sliding puzzle problem.
void AStar(PuzzleState initial) {
    
    //capture the starting time to measure the performance of the search.
    clock_t start, end;
    start = clock();
    
    int searchCost = 0;  //initialize the search cost (number of nodes expanded).
    int i;  // loop counter.
    
    //possible moves: Up, Down, Left, Right.
    char moves[4] = {'U', 'D', 'L', 'R'};

    //using an array as a simple priority queue for nodes based on their total cost.
    Node* pQueue[MAX_DEPTH * MAX_DEPTH];
    int count = 0;  //this counter keeps track of the number of nodes in the priority queue.

    //create the initial node using the provided puzzle state.
    Node* initialNode = malloc(sizeof(Node));
    initialNode->state = initial;
    initialNode->parent = NULL;
    initialNode->action = ' ';  //no action needed for the initial state.
    initialNode->cost = 0;
    initialNode->totalCost = heuristic(initial);  //total cost = path cost + heuristic value.
    pQueue[count++] = initialNode;

    //loop until the priority queue is empty.
    while (count > 0) {
        
        //dequeue the node with the lowest total cost.
        Node* currNode = pQueue[0];
        
        //shift the remaining nodes in the priority queue to remove the dequeued node.
        for (i = 0; i < count - 1; i++) 
		{
            pQueue[i] = pQueue[i + 1];
        }
        count--;

        //if the dequeued node's state is the goal state.
        if (isGoalState(currNode->state)) 
		{
            end = clock();  //capture the end time.
            double time_taken = ((double)end - start) / CLOCKS_PER_SEC;

            //print the solution path.
            printf("Solution path: [");
            while (currNode->parent) 
			{
                printf("%c - ", currNode->action);
                currNode = currNode->parent;
            }
            printf("]\n");

            printf("Solution cost: %d\n", currNode->cost);
            printf("Search cost: %d\n", searchCost);
            printf("Running time: %f seconds\n", time_taken);
            printBoard(currNode->state);
            return;
        }

        //expand the dequeued node by generating its successors.
        for (i = 0; i < 4; i++) {
            Node* successor = generateSuccessor(currNode, moves[i]);
            if (successor) {
                successor->cost = currNode->cost + 1;
                successor->totalCost = successor->cost + heuristic(successor->state);  //calculate the total cost.
                
                //insert the successor into the priority queue at the correct position.
                int j;
                for (j = count; j > 0 && pQueue[j-1]->totalCost > successor->totalCost; j--) 
				{
                    pQueue[j] = pQueue[j-1];
                }
                pQueue[j] = successor;
                count++;
                searchCost++;  //increment the search cost.
            }
        }
    }

    //if the loop completes without finding a solution, inform the user.
    printf("No solution found.\n");
}



//function to configure the sliding puzzle's board based on user input.
PuzzleState configureBoard(PuzzleState state){
    int i, j;  //loop counters for rows and columns.

    //prompt the user to input the initial configuration.
    printf("Input initial board configuration numbers below:\n");
    
    //display the current configuration of the board using the printTable function.
    printBoard(state); 

    //prompt the user to input numbers from 0 to 15 for each cell of the board.
    printf("\nEnter 0-15 numbers (with spaces) to fill the board:\n");
    
    //nested loops to iterate over each row and column of the board.
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
             int input;

            do { // Prompt the user for input until a unique, valid number is entered
                scanf("%d", &input);

                if (input < 0 || input > 15 || isUnique[input]) {
                    printf("\nError: Invalid input or a repeated number. Please try again.\n");
                }
            } while (input < 0 || input > 15 || isUnique[input]);

            state.board[i][j] = input;
            isUnique[input] = true;   
        } 
    }

    //clear the screen (works for both Windows and Linux).
    system("cls||clear");

    //display the newly configured board to the user.
    printBoard(state);

    //return the updated PuzzleState structure with the new configuration.
    return state;
}

//function to allow the user to modify the sliding puzzle's board.
void modifyBoard(){
    int i, j, choice0;  //loop counters and user's menu choice.
    PuzzleState current;  //current board configuration.

    //initialize the current board configuration with all zeroes.
    for( i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            current.board[i][j] = 0;
        }
    }
    
    //configure the board based on user input using the configureBoard function.
    current = configureBoard(current);  

    //loop to offer the user the choice to reconfigure the board or continue.
    do {
        printf("\nWant to configure board again?");
        printf("\n[1] Yes\t\t[2] No/Continue\n\n");
        scanf("%d", &choice0);  //read the user's choice.

        if (choice0 == 1) 
		{
            //if the user chooses to reconfigure, clear the screen and call the configureBoard function again.
            system("cls||clear");
            current = configureBoard(current);  
        }
        else if (choice0 == 2) 
		{
            // If the user chooses to continue, update the global 'initial' board with the current configuration, then clear the screen and exit the loop.
            initial = current;
            system("cls||clear");
            break;
        }
        else {
            // If the user inputs an invalid choice, display an error message.
            printf("Invalid Input");
        }
    } while (choice0 != 2);  // Continue looping until the user decides to proceed without reconfiguring.
}


int main() {
	int choice;
 
    do	{
		//system("cls||clear");   //this function is used to run system/command prompt commands and here cls is a command to clear the output screen for both windows and linux
		printf("-----------------------------------------------------------------\n");
		printf("|\t\t          15-PUZZLE SOLVER          \t\t|\n");
		printf("-----------------------------------------------------------------\n");
		printBoard(initial);
		printf("-----------------------------------------------------------------\n");
		printf("|0.) Modify Board  \t\t\t\t\t\t|\n");
		printf("|1.) Iterative Deepening Search \t\t\t\t|\n");
		printf("|2.) A* search\t\t\t\t\t\t\t|\n");
		printf("|3.) End Program \t\t\t\t\t\t|\n");
		printf("-----------------------------------------------------------------\n");
		
		printf("\nEnter your number choice: ");
		scanf("%d", &choice); 
			
		switch(choice){
			case 0:
				modifyBoard();
				getchar();
                break;
			case 1:
				system("cls||clear");
                IDS(initial);
                printf("\n\n\nPress enter to continue...\n");
				fflush(stdout);
				getchar(); 
				break;
			case 2:
				AStar(initial);
				printf("\n\n\nPress enter to continue...\n");
				getchar(); 
                fflush(stdout);
				break;
			case 3:
				return 0;
			default:
				getchar();
			  	printf("\nInvalid. Please try again.");
				printf("\n\n\nPress enter to continue...");
				fflush(stdout);
				getchar(); 			  			
				break;	
		}
	} while(choice != 3);

return 0;
}
