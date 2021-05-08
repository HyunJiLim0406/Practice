//
// 2021 Spring - Automata and Formal Languages (03)
// Homework #1: Building a DFA simulator that accepts strings with prefix bbab
//
// Copyright (c) Prof. Jaehyeong Sim
// Department of Computer Science and Engineering, Ewha Womans University
//

#include <stdio.h>

#define MAX_NUM_STATES 10
#define MAX_NUM_INPUTS 16
#define NUM_SYMBOLS 2


// Definition of DFA
struct DFA {
    int num_states;                         // the number of DFA states
    char symbols[NUM_SYMBOLS];              // symbols 
    int trans[MAX_NUM_STATES][NUM_SYMBOLS]; // transitions
    int idx_init;                           // an initial state
    int idx_isFinal[MAX_NUM_STATES];        // final states
};

int main(int argc, char *argv[]) {
    // declare struct DFA
    struct DFA u1;

    // assign values to member variables
    u1.num_states = 6;

    u1.symbols[0] = 'a';
    u1.symbols[1] = 'b';

    u1.trans[0][0] = 5;
    u1.trans[0][1] = 1;
    u1.trans[1][0] = 5;
    u1.trans[1][1] = 2;
    u1.trans[2][0] = 3;
    u1.trans[2][1] = 5;
    u1.trans[3][0] = 5;
    u1.trans[3][1] = 4;
    u1.trans[4][0] = 4;
    u1.trans[4][1] = 4;
    u1.trans[5][0] = 5;
    u1.trans[5][1] = 5;

    u1.idx_init = 0;
    for (int i = 0; i < u1.num_states; i++) {
        if (i == 4)
            u1.idx_isFinal[i] = 1;
        else
            u1.idx_isFinal[i] = 0;
    }

    // do transitions according to input characters
    int currentState = u1.idx_init;

    for (int i = 0; i < MAX_NUM_INPUTS; i++) {
        char inchar = argv[1][i];
        if (inchar == '\0') {
            break;
        }
        if (inchar == 'a')
            currentState = u1.trans[currentState][0];
        else if (inchar == 'b')
            currentState = u1.trans[currentState][1];
        else { //a, b 이외의 문자
            printf("Rejected\n");
            return 0;
        }
    }

    // print the output
    if (u1.idx_isFinal[currentState] == 1)
        printf("Accepted\n");
    else
        printf("Rejected\n");
    return 0;
}