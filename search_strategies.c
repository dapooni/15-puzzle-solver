/*
    MEMBERS:
    Agao, Daphne Julienne 
    Balatinsayo, Judith
    Bertulfo, Juliet Clarisse
    Colasito, Neslie
    Judin, Jade Airin
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <stdbool.h>
#define BLANK_CHARACTER '0'

unsigned int nodesExpanded;  //number of expanded nodes
unsigned int nodesGenerated; //number of generated nodes
unsigned int solnLength; //number of moves in solution
double runtime;              //elapsed time (in milliseconds)

typedef struct Node Node;
typedef struct NodeList NodeList;
typedef struct State State;
typedef enum Move Move;
typedef struct SolutionPath SolutionPath;

void inputInitialState(State * const state);
void inputgoalState(State * const state);
void printBoard(char const board[][4]);
void printSolution(struct SolutionPath *path) ;
void destroySolution(SolutionPath **list);
char pushNode(Node *node, NodeList** const list);
Node* popNode(NodeList** const list);
Node* popNodehead(NodeList** const list);
void pushList(NodeList **toAppend, NodeList *list);
Node* createNode(unsigned int d, unsigned int h, State *s, Node *p);
void destroyTree(Node *node);
NodeList* getChildren(Node *parent, State *goalState);
State* createState(State *state, Move move);
void destroyState(State **state);
int manhattanDist(State * const curr, State * const goal);
void pushListInOrder(NodeList **toAppend, NodeList *list);
char statesMatch(State const *testState, State const *goalState);
void checkStates(State * const initial, State * const goal);

typedef enum Move {
    UP, DOWN, LEFT, RIGHT, //values for moving up, down, left, right, respectively
    NOT_APPLICABLE         //value assigned for initial and goal input states
} Move;

typedef struct State {
    Move action;           //action that resulted to `this` board state
   char board[4][4];      //resulting board configuration after applying action
} State; 

typedef struct ListNode {
    Node *currNode;
    struct ListNode *prevNode; //the node before `this` instance
    struct ListNode *nextNode; //the next node in the linked list
} ListNode;

typedef struct SolutionPath {
    Move action;
    struct SolutionPath *next;
} SolutionPath;

struct Node {
    unsigned int depth; //depth of the node from the root. For A* search,
                        //this will also represent the node's path cost
    unsigned int hCost; //heuristic cost of the node
    State *state;       //state designated to a node
    Node *parent;       //parent node
    NodeList *children; //list of child nodes
};

struct NodeList {
    unsigned int nodeCount;    //the number of nodes in the list
    ListNode *head;            //pointer to the first node in the list
    ListNode *tail;            //pointer to the last node in the list
};


SolutionPath* IDSearch(State *, State *,int depth_limit);
SolutionPath* AStarSearch(State *, State *);

int main() {
    int choice;
    int limit;
    State initial;       //initial board state
    State goal;         //goal board configuration

    SolutionPath *ids; //solution path of each search method
    SolutionPath *astar;

    do {
        system("cls||clear"); // Clear the console screen.
        printf("  _  ___       ___                _        ___       _                \n");
        printf(" / || __| ___ | _ \\ _  _  ___ ___| | ___  / __| ___ | |__ __ ___  _ _ \n");
        printf(" | ||__ \\|___||  _/| || ||_ /|_ /| |/ -_) \\__ \\/ _ \\| |\\ V // -_)| '_|\n");
        printf(" |_||___/     |_|   \\_,_|/__|/__||_|\\___| |___/\\___/|_| \\_/ \\___||_|  \n");
        printf("                                                                      \n");
        printf("-----------------------------------------------------------------\n");
        printf("|0.) Reconfigure the Board\t\t\t\t\t|\n");
        printf("|1.) Iterative Deepening Search\t\t\t\t\t|\n");
        printf("|2.) A* Search\t\t\t\t\t\t\t|\n");
        printf("|3.) End Program\t\t\t\t\t\t|\n");
        printf("-----------------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
            	printf("\nReconfiguring the puzzle board...");
                inputInitialState(&initial);
                inputgoalState(&goal);
                printf("Board reconfigured successfully.\n");
                printf("\n");
                system("pause");
                break;
            case 1:
                if (statesMatch(&goal, &initial)){
                    printf("No moves needed. The initial state is already the goal state.\n");
                }
   			 	for (limit = 1; limit < 1000000; limit++) {
                    nodesExpanded = 0;
                    nodesGenerated = 0;
                    solnLength = 0;
                    runtime = 0;

                    SolutionPath *ids = IDSearch(&initial, &goal, limit);
                    printf("\n----------- USING IDS ALGORITHM DEPTH LIMIT = %d -----------\n", limit);

                    printSolution(ids);
                    if (solnLength) {
                        limit = 1000000;
                        printf("IDS completed successfully.\n");
                    }
                    else {
                        printf("IDS did not find a solution.\n");
                    }
                    destroySolution(&ids);
   			    }
                system("pause");
                break;

			case 2:
                if (statesMatch(&goal, &initial)){
                    printf("No moves needed. The initial state is already the goal state.\n");
                }
   				printf("\n------------------ USING A* ALGORITHM  ---------------------- \n");
    			nodesExpanded = 0;
    			nodesGenerated = 0;
    			solnLength = 0;
    			runtime = 0;
    			SolutionPath *astar = AStarSearch(&initial, &goal);
    
    			printSolution(astar);

    			if (solnLength) {
        			printf("A* Search completed successfully.\n");
    			} else {
       	 		printf("A* Search did not find a solution.\n");
    			}

    			destroySolution(&astar);
    			system("pause");
    			break;
            case 3:
            	printf("\nYey! You're done! :)\n"); 
                return 0;  // Exit the program
            default:
                printf("Invalid choice. Please enter a valid menu option.\n");
                system("pause");
                break;	
        }
    } while (choice != 3);

    return 0;
}

SolutionPath* IDSearch(State *initial, State *goal,int depth_limit) {
    NodeList *queue = NULL;
    NodeList *children = NULL;
    Node *node = NULL;
   
    clock_t start = clock(); //start timer

  
    pushNode(createNode(0, manhattanDist(initial, goal), initial, NULL), &queue);   //initialize the queue with the root node of the search tree
    Node *root = queue->head->currNode; //for deallocating the generated tree


    while(queue->nodeCount > 0) {
        node = popNodehead(&queue);//pop the last node (tail) of the queue

        if(statesMatch(node->state, goal)) //if the state of the node is the goal state
            break;

        if(node->depth< depth_limit){
            children = getChildren(node, goal);
            ++nodesExpanded;
            pushList(&children, queue);
        }
    }

    runtime = (double)(clock() - start) / CLOCKS_PER_SEC; //determine the time elapsed

    SolutionPath *pathHead = NULL;
    SolutionPath *newPathNode = NULL;
    if(statesMatch(node->state, goal)){

        while(node) {
            newPathNode = malloc(sizeof(SolutionPath));
            newPathNode->action = node->state->action;
            newPathNode->next = pathHead;
            pathHead = newPathNode;

            ++solnLength; //update the solution length and move on the next node
            node = node->parent;
        }

        --solnLength; //uncount the root node
        }
       
        destroyTree(root);  //deallocate the generated tree

        return pathHead;
}

SolutionPath* AStarSearch(State *initial, State *goal){
    NodeList *openList = NULL;
    NodeList *closedList = NULL;
    Node *node = NULL;

    clock_t start = clock();

    pushNode(createNode(0, manhattanDist(initial, goal), initial, NULL), &openList);
    Node *root = openList->head->currNode; 


    while(openList->nodeCount > 0) {
        node = popNode(&openList);

        if(statesMatch(node->state, goal)) //if the state of the node is the goal state
            break;

        closedList = getChildren(node, goal); //else, expand the node and update the expanded-nodes counter
        ++nodesExpanded;

        pushListInOrder(&closedList, openList); //add the node's closedList to the openList
    }

    runtime = (double)(clock() - start) / CLOCKS_PER_SEC;

    SolutionPath *pathHead = NULL;
    SolutionPath *newPathNode = NULL;

    while(node) {
        newPathNode = malloc(sizeof(SolutionPath));
        newPathNode->action = node->state->action;
        newPathNode->next = pathHead;
        pathHead = newPathNode;

        ++solnLength;
        node = node->parent;
    }

    --solnLength; 

    destroyTree(root);

    return pathHead;
}

void inputInitialState(State * const state){
    state->action=NOT_APPLICABLE;

	printf("\nInput the initial state of 15 puzzle separated with space (enter 0 for the blank tile):\n");
    int i,j;
    int input;
    for(i = 0; i < 4; ++i) {
        for(j = 0; j < 4; ++j) {
            scanf("%d", &input);
            state->board[i][j] = input + '0';
        }
    }
}

void inputgoalState(State * const state){
    state->action=NOT_APPLICABLE;
    int i,j;

    int num=0;
    for(i = 0; i < 4; ++i) {
        for(j = 0; j < 4; ++j) {
            state->board[i][j] = num + '0';
            num++;
        }
    }
}

void printSolution(struct SolutionPath *path) {
    if(!path) {//check if solution exists
        return;
    }

	if(!path->next) { //if the initial state is already the goal state
		printf("No moves needed. The initial state is already the goal state.\n");
		return;
	}

    printf("SOLUTION PATH: (Relative to the space character)\n");

    char *move[4] = { "UP", "DOWN", "LEFT", "RIGHT" }; //will use hash map to speed up the proccess a bit
    int counter = 1;

    for(path = path->next; path; path = path->next, ++counter) { //will be skipping the first node since it represents the initial state with no action
        printf("%i. Move %s\n", counter, move[path->action]);
    }

    printf(
        "\nDETAILS:\n"
        " - Solution cost   : %i\n"
        " - Nodes expanded  : %i\n"
        " - Running time    : %g milliseconds\n\n",
        solnLength, nodesExpanded, runtime);
}

void destroySolution(SolutionPath **list) {
    SolutionPath *next;
    while(*list) {
        next = (*list)->next;
        free(*list);
        *list = next;
    }
    *list = NULL;
}

char pushNode(Node *node, NodeList** const list) {
    if(!node)
        return 0;

    ListNode *doublyNode = malloc(sizeof(ListNode));
    if(!doublyNode)
        return 0;

    doublyNode->currNode = node;

    if(*list && !(*list)->nodeCount) {
        (*list)->head = doublyNode;
        (*list)->tail = doublyNode;
        doublyNode->nextNode = NULL;
        doublyNode->prevNode = NULL;
        ++(*list)->nodeCount;
        return 1;
    }

    if(*list == NULL) {
        *list = malloc(sizeof(NodeList));
        if(*list == NULL)
            return 0;

        (*list)->nodeCount = 0;
        (*list)->head = NULL;
        (*list)->tail = doublyNode;
    }
    else {
        (*list)->head->prevNode = doublyNode;
    }

    doublyNode->nextNode = (*list)->head;
    doublyNode->prevNode = NULL;
    (*list)->head = doublyNode;

    ++(*list)->nodeCount;

    return 1;
}

Node* popNode(NodeList** const list) {
    if(!*list || (*list)->nodeCount == 0)
        return NULL;

    Node *popped = (*list)->tail->currNode;
    ListNode *prevNode = (*list)->tail->prevNode;

    free((*list)->tail); //free the list node pointing to node to be popped

    if((*list)->nodeCount == 1) {
        (*list)->head = NULL;
    }
	else {
		prevNode->nextNode = NULL;
	}

    (*list)->tail = prevNode;
    --(*list)->nodeCount;
    return popped;
}

Node* popNodehead(NodeList** const list){
    if(!*list || (*list)->nodeCount == 0)
        return NULL;

    Node *popped = (*list)->head->currNode;
    ListNode *nextNode = (*list)->head->nextNode;

    free((*list)->head);

    if((*list)->nodeCount == 1) {
        (*list)->tail = NULL;
    }
	else {
		nextNode->prevNode = NULL;
	}

    (*list)->head = nextNode;
    --(*list)->nodeCount;
    return popped;
}

void pushList(NodeList **toAppend, NodeList *list) {
    //if either of the list is NULL, the head of the list to be appended is NULL,
    //or the list points to the same starting node
    if(!*toAppend || !list || !(*toAppend)->head || (*toAppend)->head == list->head) {
        return;
    }

    if(!list->nodeCount) { //if the list to append to has currently no element
        list->head = (*toAppend)->head;
        list->tail = (*toAppend)->tail;
    }
    else { //connect the lists
        (*toAppend)->tail->nextNode = list->head;
        list->head->prevNode = (*toAppend)->tail;
		list->head = (*toAppend)->head;
    }

    list->nodeCount += (*toAppend)->nodeCount; //update list information

    free(*toAppend);
    *toAppend = NULL;
}

Node* createNode(unsigned int d, unsigned int h, State *s, Node *p) {
    Node *newNode = malloc(sizeof(Node));
    if(newNode) {
        newNode->depth = d;
        newNode->hCost = h;
        newNode->state = s;
        newNode->parent = p;
        newNode->children = NULL;
        ++nodesGenerated; //update counter
    }
    return newNode;
}

void destroyTree(Node *node) {
    if(node->children == NULL) {
        free(node->state);
        free(node);
        return;
    }

    ListNode *listNode = node->children->head;
    ListNode *nextNode;

    while(listNode) {
        nextNode = listNode->nextNode;
        destroyTree(listNode->currNode);
        listNode = nextNode;
    }

    //free(node->state);
    free(node->children);
    free(node);
}

NodeList* getChildren(Node *parent, State *goalState) {
    NodeList *childrenPtr = NULL;
    State *testState = NULL;
    Node *child = NULL;

    if(parent->state->action != DOWN && (testState = createState(parent->state, UP))) { //attempt to create states for each moves, and add to the list of children if true
        child = createNode(parent->depth + 1, manhattanDist(testState, goalState), testState, parent);
        pushNode(child, &parent->children);
        pushNode(child, &childrenPtr);
    }
    if(parent->state->action != UP && (testState = createState(parent->state, DOWN))) {
        child = createNode(parent->depth + 1, manhattanDist(testState, goalState), testState, parent);
        pushNode(child, &parent->children);
        pushNode(child, &childrenPtr);
    }
    if(parent->state->action != RIGHT && (testState = createState(parent->state, LEFT))) {
        child = createNode(parent->depth + 1, manhattanDist(testState, goalState), testState, parent);
        pushNode(child, &parent->children);
        pushNode(child, &childrenPtr);
    }
    if(parent->state->action != LEFT && (testState = createState(parent->state, RIGHT))) {
        child = createNode(parent->depth + 1, manhattanDist(testState, goalState), testState, parent);
        pushNode(child, &parent->children);
        pushNode(child, &childrenPtr);
    }

    return childrenPtr;
}

State* createState(State *state, Move move) {
    State *newState = malloc(sizeof(State));

    //copy the board configuration of `state` to `newState`
    //while searching for the row and column of the blank character
    int i, j;        //used for traversing the 3x3 arrays
    int row=0;
    int col=0;    //coordinates of the blank character

    for(i = 0; i < 4; ++i) {
        for(j = 0; j < 4; ++j) {
            if(state->board[i][j] == BLANK_CHARACTER) {
                row = i;
                col = j;
            }

            newState->board[i][j] = state->board[i][j];
        }
    }

     //test if the coordinates are valid after translation based on the move
    //if it is, swap the concerned board values to reflect the move
    if(move == UP && row - 1 >= 0) {
        char temp = newState->board[row - 1][col];
        newState->board[row - 1][col] = BLANK_CHARACTER;
        newState->board[row][col] = temp;
        newState->action = UP;
        return newState;
    }
    else if(move == DOWN  && row + 1 < 4) {
        char temp = newState->board[row + 1][col];
        newState->board[row + 1][col] = BLANK_CHARACTER;
        newState->board[row][col] = temp;
        newState->action = DOWN;
        return newState;
    }
    else if(move == LEFT  && col - 1 >= 0) {
        char temp = newState->board[row][col - 1];
        newState->board[row][col - 1] = BLANK_CHARACTER;
        newState->board[row][col] = temp;
        newState->action = LEFT;
        return newState;
    }
    else if(move == RIGHT && col + 1 < 4) {
        char temp = newState->board[row][col + 1];
        newState->board[row][col + 1] = BLANK_CHARACTER;
        newState->board[row][col] = temp;
        newState->action = RIGHT;
        return newState;
    }

    free(newState);
    return NULL;
}

void destroyState(State **state) {
    free(*state);
    state = NULL;
}

int manhattanDist(State * const curr, State * const goal) {
    int x0, y0; //used for indexing each symbol in `curr`
    int x1, y1; //correspoinding row and column of symbol from curr[y0, x0] at `goal`
    int dx, dy; //change in x0 and x1, and y0 and y1, respectively
    int sum = 0;

    //for each symbol in `curr`
    for(y0 = 0; y0 < 4; ++y0) {
        for(x0 = 0; x0 < 4; ++x0) {
            //find the coordinates of the same symbol in `goal`
            for(y1 = 0; y1 < 4; ++y1) {
                for(x1 = 0; x1 < 4; ++x1) {
                    if(curr->board[y0][x0] == goal->board[y1][x1]) {
                        dx = (x0 - x1 < 0)? x1 - x0 : x0 - x1;
                        dy = (y0 - y1 < 0)? y1 - y0 : y0 - y1;
                        sum += dx + dy;
                    }
                }
            }
        }
    }
    return sum;
}

int totalCost(Node * const node) {
    return node->depth + node->hCost;
}
void pushListInOrder(NodeList **toAppend, NodeList *list){
    if(!*toAppend || !list || !(*toAppend)->head || (*toAppend)->head == list->head) {
        return;
    }

    if(!list->nodeCount) {
        pushNode(popNode(toAppend), &list);
    }

    ListNode *toAppendNode; 
    ListNode *listNode;     
    Node *node;

     while((toAppendNode = (*toAppend)->head)) {
        listNode = list->head;

        while(listNode && totalCost(toAppendNode->currNode) < totalCost(listNode->currNode)) {
            listNode = listNode->nextNode;
        }

        ListNode *temp = toAppendNode->nextNode;

        if(!listNode) {
            list->tail->nextNode = toAppendNode;
            toAppendNode->prevNode = list->tail;
            toAppendNode->nextNode = NULL;
            list->tail = toAppendNode;
        }
        else {
            if(listNode->prevNode) {
                toAppendNode->prevNode = listNode->prevNode;
                toAppendNode->nextNode = listNode;
                listNode->prevNode->nextNode = toAppendNode;
                listNode->prevNode = toAppendNode;
            }
            else {
                toAppendNode->nextNode = list->head;
                toAppendNode->prevNode = NULL;
                list->head->prevNode = toAppendNode;
                list->head = toAppendNode;
            }
        }

        (*toAppend)->head = temp;
        --(*toAppend)->nodeCount;
        ++list->nodeCount;
    }
    free(*toAppend);
    *toAppend = NULL;
}

char statesMatch(State const *testState, State const *goalState) {
    int row = 4, col;

    while(row--) {
        col = 4;
        while(col--) {
            if(testState->board[row][col] != goalState->board[row][col])
                return 0;
        }
    }
    return 1;
}


