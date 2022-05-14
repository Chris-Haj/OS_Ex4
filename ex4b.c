#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>

#include <string.h>

#include <sys/wait.h>

#include <sys/types.h>

#include <sys/ipc.h>

#include <sys/shm.h>

#define LENGTH 1000
#define DONE "done"
int running = 1;
int xValue;

typedef struct {
    char ** polynomial;
    int degree;
}pol;

void loop();
int calculatePol(char * input);
void createForksAndCalc(pol Poly);
int Pow(int x, int y);
int calcNum(char * expression);

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
    createForksAndCalc(Poly);
}

/*this function creates children processes according to the size of the array/the degree of the polynomial and each child
 * process calculates an index of the polynomials placed in the struct, and they all add the value they calculate
 * onto the total variable*/
void createForksAndCalc(pol Poly) {
    int * total, status, sharedID;
    /*we create a shared memory section that both the father and children processes are connected to,
     * so they can all update the result of the total variable*/
    key_t key = ftok("/tmp", 'y');
    if ((sharedID = shmget(key, 1024, IPC_EXCL | IPC_CREAT | 0600)) == -1) {
        perror("memory issuing failure");
    }
    if ((total = (int * ) shmat(sharedID, 0, 0)) < 0) {
        perror("memory attaching failure");
    }
    * total = 0;
    pid_t forks[Poly.degree];
    for (int i = 0; i < Poly.degree; i++) {
        forks[i] = fork(); // we create a child for each index of the polynomial, so they can calculate its result
        if(forks[i]==0){
            exit(0);
        }
        else if (forks[i] == 0) {
            int cur = calcNum(Poly.polynomial[i]);
            * total += cur; //After the child calculates the index of the polynomial it adds it up to total.
            exit(0);
        }
    }

    for (int i = 0; i < Poly.degree; i++)
        waitpid(forks[i], & status, 0);
    int res = * total;
    shmdt(total); //After finishing with the shared memory section, we free it and deletes its pointer
    shmctl(sharedID, IPC_RMID, 0);
    printf("Result of polynomial is %d\n", res);
}

/*this function which is called by each thread receives a void pointer to a string which we recast into a char pointer then
 * it searches if the string has a power or a number to multiply by x, and it calculates the small equation
 * depending on if the string contains (^) a power for x and (*) a multiplier for x then after calculation
 * the thread returns the value of the equation to the main thread*/
int calcNum(char * expression) {
    int res;
    char * indexOfPow = strchr(expression, '^');
    char * indexOfMult = strchr(expression, '*');
    if (!indexOfPow && !indexOfMult) {
        res = atoi(expression);
    } else if (!indexOfPow) {
        res = xValue * atoi(expression);
    } else if (!indexOfMult) {
        indexOfPow++;
        res = Pow(xValue, atoi(indexOfPow));
    } else {
        indexOfPow++;
        res = atoi(expression) * Pow(xValue, atoi(indexOfPow));
    }
    return res;
}
int Pow(int x, int y) { //function to calculate the power of a number
    int res = 1;
    for (int i = 0; i < y; i++)
        res *= x;
    return res;
}