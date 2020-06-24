/* Turing machine */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Cell Cell;

 struct Cell {
    /* This is a node for the doubly linked list */
    char contents;
    struct Cell* next;
    struct Cell* prev;
};

typedef struct instructionSet {
    char writeVal;
    char moveDir;
    int newState;
}instructionSet;

void insertLeft(struct Cell** head, char content){
	struct Cell* new = malloc(sizeof(struct Cell));

    (*new).contents = content;
    (*new).next = (*head);
    (*new).prev = NULL;

    /* if the head already points to something, change the pointer */
    if(head != NULL){
        (**head).prev = new;
    }

    (*head) = new;

}

void insertRight(struct Cell** head, char content) {
    struct Cell* new = malloc(sizeof(struct Cell));
    struct Cell* last = malloc(sizeof(struct Cell));
    
    last = (*head);
    (*new).contents = content;
    (*new).next = NULL;

    /* If nothing else exists, create from scratch */
    if((*head) == NULL){
        (*new).prev = NULL;
        (*head) = new;

        return;
    }

    /* If stuff already exists, go to the end of that stuff */
    while((*last).next != NULL){
        last = (*last).next;
    }

    /* Fix up the last cell */
    (*last).next = new;
    (*new).prev = last;

    return;

}

int main() {
/* Turning machine.
    Note: Input file specification:
        Line 1: Initial tape content
        Line 2: Number of states
        Line 3: Start state
        Line 4: End state
        All other lines: State, ReadVal, WriteVal, MoveDirection, NewState 
*/

        /* Step 1: Read input file */
        char input[15];
        char initialTape[32] = "";
        int stateNum = 0;
        int startState;
        int endState;

        printf("What's the name of your input file? \n");
        scanf(" %s", input);

        FILE *inputFile = fopen(input, "r");
        size_t j;

        char *line[32];

        for(j = 0; j < 32; j++) {
            line[j] = malloc(sizeof(int) * 32);

            if( line[j] ==NULL ){
                printf("Malloc error 2");
                exit(1);
            }
        }

        /* Populate variables from input file */

        if (inputFile == NULL) {
            printf("Did not find the file, try again\n");
            exit(1);
        } else {
            int i = 0;
            while(fgets(line[i], 32, inputFile) != NULL) {
            i++;
		}
			
	}
    
	fclose(inputFile);

               

    /* Line 1: Initial Tape Contents */
        strcpy(initialTape, line[0]);
        printf("Initial Tape: %s", initialTape);

    /* Line 2: Number of states */
        stateNum = atoi(line[1]);
        printf("Number of states: %d\n", stateNum);

    /* Line 3: Start state */
        startState = atoi(line[2]);
        printf("Start state: %d\n", startState);

    /* Line 4: End state */
        endState = atoi(line[3]);
        printf("End state: %d\n", endState);

    /* All other lines: State, ReadVal, WriteVal, MoveDirection, NewState*/

    /* Run through each array spot (input line) */

    /* Set up the memory allocation for the 2d array of structs */
    struct instructionSet **instructionTable = (instructionSet**) malloc(stateNum * sizeof(struct instructionSet*));

    int i;
    for(i=0; i< stateNum; i++){
        instructionTable[i] = (instructionSet*)malloc(127 * sizeof(struct instructionSet));
    }

      
    if(instructionTable == NULL){
        printf("Malloc has failed.");
    }

    /* Populate the table with states and ASCII values */
    /* State, ReadVal, WriteVal, MoveDirection, NewState */
    char *tok;
    char *state, *newState;
    char *readVal, *writeVal, *moveDir;

    int counter = 1;

                 
    for(j=4; j<32; j++){
        /* make sure the array spot isn't empty */
        if(line[j]) {
            /* get the first token from the line */
            tok = strtok(line[j], " ");                                            
            state = tok;
         

            while( tok != NULL){
                                                                    
                tok = strtok(NULL, " ");
                if(counter == 1){
                        /* 2nd spot */
                        readVal = tok;
                        counter ++;
                } else if(counter == 2){
                        /* 3rd spot */
                        writeVal = tok;
                       /* printf("WriteVal: %s \n", writeVal);*/
                                                                                        counter ++;
                } else if (counter == 3) {
                        /* 4th spot */
                        moveDir = tok;
                        /*printf("moveDir: %s \n", moveDir);*/
                                                                                        counter ++;
                } else if (counter == 4) {
                        /* 5th spot */
                        newState = tok;
                        /*printf("newState: %s \n", newState);*/

                                                                                        /* jump hack */
                                                                                        counter = 75;
                } else if (counter == 75) {

                    /* assemble instruction table */
                    instructionTable[atoi(state)][(*readVal)].writeVal = (*writeVal);
                    instructionTable[atoi(state)][(*readVal)].moveDir = (*moveDir);
                    instructionTable[atoi(state)][(*readVal)].newState = atoi(newState);
                    counter = 1;
                }


            }

        }
    }



    /* head for linked list */
    struct Cell* head = malloc(sizeof(struct Cell));
    struct Cell* curr = malloc(sizeof(struct Cell));

    head = NULL;
    curr = NULL;
  

    /* Create linked initial tape */
    int z = 0;
    
   while(initialTape && initialTape[z] != '\n' && initialTape[z] != ' ' && initialTape[z] != '\0' && initialTape[z] != '\t' && initialTape[z] != '\r' && initialTape[z] != '\v' && initialTape[z] != '\f'){
        insertRight(&head, initialTape[z]);
        z++;
    }
    
    curr = head;
    int curState = startState;
    char curVal = initialTape[0];
    int count = 0;
    int skip = 0;


    while(curState != endState) {
        skip = 0;
        
    
    /* WRITE */
    (*curr).contents = instructionTable[curState][curVal].writeVal;
    printf("Write: %c\n", (*curr).contents);
  
    /* MOVE */
    if(instructionTable[curState][curVal].moveDir  == 'L'){
        printf("Move left\n");
        if((*curr).prev != NULL) {
            
            curr = (*curr).prev;
            
            if((*curr).prev == NULL){
                printf("Move left and make a new one\n");
                insertLeft(&head, 'B');
                
            }
        } 
    } else if(instructionTable[curState][curVal].moveDir == 'R'){
        printf("Moving right\n");
      
            if((*curr).next != NULL) {
            
                curr = (*curr).next;
                
                if((*curr).next == NULL){
                     printf("Moving right and make a new one\n");
                   
                    insertRight(&head,'B');
                }
            }
                    
    }

            
            
    if(skip == 0){
        /* NEW STATE */
        curState = instructionTable[curState][curVal].newState;
        printf("Cur state: %d\n", curState);
        
        if((*curr).next != NULL){
    
        /* READ VALUE */
        curVal = (*curr).contents;
        printf("curVal: %c\n", curVal);
     
        }
    }
    
    if(count == 50){
        exit(1);
    } /*use this in case of infinite loop*/
    
    count++;
    }

    /* Print everything */
    printf("Writing Tape ...\n ");


    while(head != NULL){
        printf("%c ", (*head).contents);
        head = (*head).next;
    }


return 0;
}
