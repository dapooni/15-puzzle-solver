#include <stdio.h>

void aStarSearch(int start, int goal) {
    // Initialize open and closed lists
    Node* openList = createNode(start, 0, NULL);
    openList->cost = 0;
    openList->parent = NULL;
    Node* closedList = NULL;

    while (openList != NULL) {
        // Find the node with the lowest cost in the open list
        Node* current = openList;
        Node* currentParent = openList;
        int currentCost = openList->cost;

        Node* currentParentPrev = NULL;
        while (current != NULL) {
            if (current->cost < currentCost) {
                currentCost = current->cost;
                currentParent = currentParentPrev;
                currentParentPrev = current;
            }
            else {
                currentParentPrev = current;
            }
            current = current->parent;
        }
        current = currentParent;

        // Remove the current node from the open list
        if (current == openList) {
            openList = openList->parent;
        }
        else {
            currentParentPrev->parent = currentParent->parent;
        }

        // Add the current node to the closed list
        current->parent = closedList;
        closedList = current;

        // If the goal state is reached, reconstruct and print the path
        if (current->value == goal) {
            Node* path = current;
            while (path != NULL) {
                printf("%d ", path->value);
                path = path->parent;
            }
            printf("\n");
            return;
        }

        // Expand the current node (add child nodes to the open list)
        int childValue1 = current->value + 1;
        int childValue2 = current->value * 2;
        Node* child1 = createNode(childValue1, current->cost + 1, current);
        Node* child2 = createNode(childValue2, current->cost + 1, current);
        child1->parent = openList;
        child2->parent = openList;
        openList = child1;

        // Add a heuristic to guide the search (e.g., manhattan distance to the goal)
        // You can customize this heuristic based on your problem domain

        // Perform any additional checks for your specific problem here
    }

    printf("No solution found.\n");
}

int main(){
	int choice;
	do	{
		system("cls||clear");   //This function is used to run system/command prompt commands and here cls is a command to clear the output screen for both windows and linux
		printf("\t\t\t-------------------------------------------------------------------\n");
		printf("\t\t\t|\t\t\t   AI SEARCH STRATEGIES  \t\t\t  |\n");
		printf("\t\t\t-------------------------------------------------------------------\n");
		printf("\t\t\t|\t\t\t\t\t\t\t\t  |\n");
		printf("\t\t\t|1.) Iterative Deepening Search \t\t\t\t\t\t  |\n");
		printf("\t\t\t|2.) A* search\t\t\t\t\t\t  |\n");
		printf("\t\t\t|3.) End Program \t\t\t\t\t\t  |\n");
		printf("\t\t\t-------------------------------------------------------------------\n");
		
		printf("\nEnter your number choice: ");
		scanf("%d", &choice); 
			
		switch(choice){
			case 1:
                idsSearch();
				break;
			case 2:
				aSearch();
				break;
			default:
				getchar();
			  	printf("\nInvalid. Please try again.");
				printf("\n\n");
				printf("\nPress enter to continue...");
				fflush(stdout);
				getchar(); 			  			
				break;	
		}
	} while(choice != 3);
}    
