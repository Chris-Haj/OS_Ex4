#include <stdio.h>

#include <pthread.h>

#include <stdlib.h>

#include <string.h>

#define LENGTH 1000
#define DONE "done"
int running = 1; // running is equal to one until done is entered
int xValue;

typedef struct {
    char ** polynomial;
    int degree;
}pol;

void loop();
int calculatePol(char * input);
void createThreadsAndCalc(pol Poly);
void * calcNum(void * expression);
int Pow(int x, int y);

int main() {
    loop();
    return 0;
}
/*main loop used to get input from the user*/
void loop() {
    char input[LENGTH];
    while (running) {
        printf("Please enter a polynomial with a maximum degree of 3\n");
        fgets(input, LENGTH, stdin); //remove \n from input if it is there
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';
        if (strcmp(input, DONE) == 0) {
            printf("Program finished.\n");
            running = 0;
        } else {
            calculatePol(input);
            printf("\n");
        }
    }
}

/* this function is used to split each degree of x by itself and place it in an index
 * of nums array, it also searches for the last number which is what we place in place of the x in the equation,
 * and it creates a struct which holds the array of nums and the degree to send it to another createThreadsAndCalc*/
int calculatePol(char * input) {
    char copy[strlen(input) + 1];
    strcpy(copy, input);
    int degree = 1;
    size_t i;
    for (i = 0; copy[i] != ','; i++)
        if (copy[i] == 'x')
            degree++;
    copy[i] = '\0';
    i++;
    while (copy[i] == ' ')
        i++;
    xValue = atoi( & copy[i]);
    char * nums[degree];
    nums[0] = strtok(copy, "+");
    for (i = 1; i < degree; i++)
        nums[i] = strtok(NULL, "+");
    pol Poly = {
            nums,
            degree
    };
    createThreadsAndCalc(Poly);
}
/*this function creates threads according to the size of the array/the degree of the polynomial and sends it
 * each thread calculates an index of the polynomials placed in the struct then they all return their values
 * to the main thread and add up each other's results*/
void createThreadsAndCalc(pol Poly) {
    int * cur;
    int totalres = 0;
    pthread_t threads[Poly.degree];
    for (int i = 0; i < Poly.degree; i++) {
        pthread_create( & threads[i], NULL, (void * ) calcNum, (Poly.polynomial)[i]);
    }
    for (int i = 0; i < Poly.degree; i++) {
        pthread_join(threads[i], (void ** ) & cur);
        totalres += * cur;
        free(cur);
    }
    printf("Result of polynomial is %d\n", totalres);
}

/*this function which is called by each thread receives a void pointer to a string which we recast into a char pointer then
 * it searches if the string has a power or a number to multiply by x, and it calculates the small equation
 * depending on if the string contains (^) a power for x and (*) a multiplier for x then after calculation
 * the thread returns the value of the equation to the main thread*/
void * calcNum(void * expression) {
    char * polynomial = (char * ) expression;
    int * res = calloc(1, sizeof(int));
    char * indexOfPow = strchr(polynomial, '^');
    char * indexOfMult = strchr(polynomial, '*');
    if (!indexOfPow && !indexOfMult) {
        * res = atoi(polynomial);
    } else if (!indexOfPow) {
        * res = xValue * atoi(polynomial);
    } else if (!indexOfMult) {
        indexOfPow++;
        * res = Pow(xValue, atoi(indexOfPow));
    } else {
        indexOfPow++;
        * res = atoi(polynomial) * Pow(xValue, atoi(indexOfPow));
    }
    pthread_exit(res);
}
int Pow(int x, int y) { //function to calculate the power of a number
    int res = 1;
    for (int i = 0; i < y; i++) {
        res *= x;
    }
    return res;
}