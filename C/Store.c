/* 
Reads input from user and based on input creates and populates 2D array of structs.
Rows are shelves [] and columns are locations on shelves [][]
Struct represents item w/ name and quantity
Users can enter and retrieve items (prints to screen)
*/



/* Reads in user input to create 2D struct array of item objects. Can then be retrieved and printed */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct item {
        char itemName[15];
        int itemAmount;
}item;

int main() {
                int shelves, locations = 0;
                int maxsize, mysize = 0;

                /* User interface */
                printf("Hello and welcome to Grocery Gromps Internal Storage TM! \n");
                printf("How many shelves do you have in your shop? \n");
                scanf(" %d", &shelves);
                printf("How many locations are there on each shelf? \n");
                scanf(" %d", &locations);

                /* Set up the memory allocation for the 2d array */
                struct item **storefront = (item**) malloc(shelves * sizeof(struct item*));

                int i= 0;
                for(i=0; i<shelves; i++){
                        storefront[i] = (item*)malloc(locations * sizeof(struct item));
                }

                if(storefront == NULL){
                        printf("Malloc has failed and that's your whole problem.");
                }

                maxsize = shelves * locations;

                /* Allow user input */
                while(1) {
                        char input[15];
                        int inputNum = 0;

                        printf("Would you like to VIEW your shop or ADD items? \n");
                        scanf(" %s", &input);

                        /* Add to the array and dynamically grow */
                        if(strcmp(input, "ADD") == 0) {
                                int myshelf, myloc = 0;
                                printf("Which shelf would you like this product on? \n");
                                scanf(" %d", &myshelf);
                                printf("What location on shelf %d would you like? \n", myshelf);
                                scanf(" %d", &myloc);

                                if(mysize < maxsize) {

                                        /* couple of sanity checks */
                                        if(storefront == NULL) {
                                                printf("Your problem is storefront. It is null. Again. Somehow.");
                                        }

                                        printf("Please enter an object you would like to fill the shelves with \n");
                                        scanf(" %s", &input);
                                        strcat(storefront[myshelf][myloc].itemName, input);
                                        printf("How many %s do you have? \n", storefront[myshelf][myloc].itemName);
                                        scanf(" %d", &inputNum);
                                        storefront[myshelf][myloc].itemAmount = inputNum;

                                        printf("You have placed %d %s on shelf %d, location %d", storefront[myshelf][myloc].itemAmount, storefront[myshelf][myloc].itemName, myshelf, myloc);
                                        mysize ++;

                                } else if(mysize >= maxsize) {
                                        /* Dynamically growing part */
                                        printf("It looks like you've ran out of space! We will now increase your storage units. \n");
                                        printf("What would you like your new number of shelves to be? \n");
                                        scanf(" %d", &shelves);
                                        printf("How many locations are there on each shelf? \n");
                                        scanf(" %d", &locations);

                                        printf("Please hold while we create your new storefront! ....  \n");

                                        storefront = realloc(storefront, sizeof(item*) * shelves * locations);

                                        item(*newStoreFront)[locations] = (item(*)[locations])storefront;
                                                int i= 0;
                                                for(i=0; i<shelves; i++){
                                                        memmove( newStoreFront[i], storefront[i], sizeof(item) * locations);

                                                }

                                        newStoreFront = realloc( storefront, sizeof(item) * shelves * locations);
                                        maxsize = shelves * locations;
                                }else{
                                        printf("There's been some sort of crazy error! I'm not really sure what to do. Try Again? \n");

                                }

                        /* View all dumped contents of 2D array */
                        } else if(strcmp(input, "VIEW") == 0) {

                                int i, j=0;
                                int imp = 0;

                                for(i=0; i<shelves; i++){
                                        for(j=0; j<locations; j++){
                                                if(storefront[i][j].itemAmount !=0) {
                                                        inputNum = storefront[i][j].itemAmount;
                                                        printf("%d ", inputNum);
                                                        printf("%s", storefront[i][j].itemName);
                                                        printf("\n");
                                                } else {
                                                        continue;
                                                }
                                        }
                                }

                        }

                }
          /* Free memory allocated to the heap */

                        for(i=0; i<shelves; i++){
                                free(storefront[i]);
                        }
                        free(storefront);

        return 0;
}
