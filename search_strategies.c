#include <stdio.h>

void idsSearch(){
	//code here
}

void aSearch(){
	//code here
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