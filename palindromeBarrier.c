#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define STR_SIZE 6
#define T_COUNT 6
#define HALF 3

#define LOWER 97
#define UPPER 122

#define REPEAT 10

char string[T_COUNT];
int count_char = 0; //Flag responsible to indicate the position on the array

pthread_barrier_t wall;

void* letterCreation(void* tp_id){  //Thread responssible the generate the randon letters
    int tid = *(int*) tp_id;    //Thread "ID" based on their creation order
    char letter;

    printf("Thread %d created successfuly\n", tid);
    printf("Thread %d will access the critical section\n", tid);

    pthread_barrier_wait(&wall);

    letter = (rand() % (UPPER - LOWER +1)) + LOWER; //Randon letter generation
    string[count_char++] = letter;

    printf("Thread %d created the letter -> %c\n", tid, letter);
    pthread_exit(0);
    return NULL;
}

void palindromeCheck(char* string){ //Function responsible for checking if the string is a palindrome
    count_char = 0;
    int check = 0;  //Counter for string check
    int j = STR_SIZE-1; 

    printf("The string created was: %s\n", string);

    for(int i = 0; i < HALF; i++){  //Logic responsible to check the string generated
        if(string[i] == string[j--]){
            check++;
        }else{
        printf("The string isn't a palindrome\n\n");
        sleep(1);
        break;
        } 
    }

    if(check == HALF){
        printf("The string is a palindrome\n\n");
        sleep(1);
    }
}

int main(void){
    srand(time(NULL));
    pthread_t threads[T_COUNT];
    int t_id[T_COUNT];

    pthread_barrier_init(&wall, NULL, T_COUNT + 1);

    for(int j = 0; j < REPEAT; j++){    //Logic responsible for generating 10 diferent strings
        for(int i = 0; i < T_COUNT; i++){   //Thread creation
            t_id[i] = i;
            pthread_create(&threads[i], NULL, letterCreation, &t_id[i]);
        }

        pthread_barrier_wait(&wall);


        for(int i = 0; i < T_COUNT; i++){
            pthread_join(threads[i], NULL); //Put the threads on the join queue
        }

        palindromeCheck(string);
    }

    pthread_barrier_destroy(&wall);

    printf("\n");
    printf("Verification string test\n");
    palindromeCheck("asddsa");  /*Calling the function with a predefined string just to make sure that it's working correctly, because the strings are always 
                                  randomly generated */

    return 0;
}