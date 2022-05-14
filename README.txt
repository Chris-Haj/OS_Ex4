Polynomial Calculator
Authored by Christopher Haj
207824772

==Description==
This program is used to calculate and print out the result of the polynomial equation entered in by the user with the x value placed at the end
of the user's input, each degree of x is calculated by either a (thread in ex4a)/(child process in ex4b) and they all add their results to each other
to sum of the entire value of the polynomial equation.

==Functions==
Shared functions between files:
    1)loop: function used to get input from user until "done" is entered in.
    2)calculatePol: function to split each degree of x by itself and store them all in a string array,
                    and it also stores the value placed at the end of the input into a global variable,
                    to use later on when calculating each part of the polynomial equation.
    3)calcNum: this functions calculates the part of the equation receives.
    4)Pow: this function calculates returns the value of x^y.
ex4a functions:
    1)createThreadsAndCalc: this function creates threads and sends each thread a part of the equation to calculate,
                            after each thread finishes calculating its received input, it brings it back to the main thread,
                            and they all add their results to each other to return the result of the equation.
ex4a functions:
    1)createForksAndCalc: this function creates children processes according to the size of the array/the degree of the polynomial and each child
                          process calculates an index of the polynomials placed in the struct, and they all add the value they calculate
                          onto the total variable
==Program Files==
ex4a.c
exba.c

==How to compile?==
compile: gcc ex4a.c -o ex4a
compile: gcc ex4b.c -o ex4b

==How to run?==
./ex4a
./ex4b


==Input==
1) "done" to finish the execution of the program and quit.
2)A polynomial equation that contains a maximum degree of 3.
Examples:
    1)2*x^3+x^1+3,1
    2)2*x^3+x^1+3,2
    3)x^3+5*x^2+x^1+3,2
    4)x^2+3,2
    5)x^2+x^1+4,0
    6)3,2

==Output==
1)6
2)21
3)33
4)7
5)4
6)3
