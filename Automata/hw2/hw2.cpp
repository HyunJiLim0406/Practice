//
// 2021 Spring - Automata and Formal Languages (03)
// Homework #2: Building a DPDA simulator that accepts strings with prefix (a^n)(b^n)
//
// Copyright (c) Prof. Jaehyeong Sim
// Department of Computer Science and Engineering, Ewha Womans University
//

#include <iostream>
#include <string>
#include <deque>

#define NUM_STATES 4
#define MAX_NUM_INPUTS 16
#define NUM_ISYMBOLS 2                      // a, b
#define NUM_SSYMBOLS 2                      // a, $

using namespace std;

// Definition of DPDA controller
struct DPDA_CTRL {
    int num_states;                                         // the number of DFA states
    char isymbols[NUM_ISYMBOLS];                            // input alphabet 
    char ssymbols[NUM_SSYMBOLS];                            // stack alphabet

    // transitions: there are up to NUM_ISYMBOLS*NUM_SSYMBOLS possible transitions for a state
    // transition format (5 char): isqp + \0
    //  i: input symbol
    //  s: pop symbol
    //  q: target state number
    //  p: push symbol
    string trans[NUM_STATES][NUM_ISYMBOLS * NUM_SSYMBOLS];
    int idx_init;                                           // an initial state
    char sss;                                               // start stack symbol
    bool idx_isFinal[NUM_STATES];                            // final states
};

void init_trans(DPDA_CTRL *ctrl) { //초기화
    int i, j;
    for (i = 0; i < NUM_STATES; i++)
        for (j = 0; j < NUM_ISYMBOLS * NUM_SSYMBOLS; j++)
            ctrl->trans[i][j] = "0000";
}

void print_cur(int cur_idx, string str, deque<char> s) { //현재 상태 출력
    cout << "{" << cur_idx << ", " << str << ", ";
    int size = s.size();
    for (int i = 0; i < size; i++) {
        char c = s.front();
        cout << c;
        s.pop_front();
        s.push_back(c);
    }
    cout << "}\n";
}

int trans_idx(char i_symbol, char s_symbol) {
    if (i_symbol == 'a') //a-a -> 0, a-$ -> 1
        return (s_symbol == 'a') ? 0 : 1;
    else //b-a -> 2, b-$ -> 3
        return (s_symbol == 'a') ? 2 : 3;
}

int main(int argc, char *argv[]) {
    // declare structures
    DPDA_CTRL ctrl;
    deque<char> s;

    // do some initialization
    ctrl.num_states = 4;
    ctrl.isymbols[0] = 'a';
    ctrl.isymbols[1] = 'b';
    ctrl.ssymbols[0] = 'a';
    ctrl.ssymbols[1] = '$';
    init_trans(&ctrl);
    ctrl.idx_init = 0;
    ctrl.sss = '$';
    for (int i = 0; i < ctrl.num_states; i++) {
        if (i == 0 || i == 3) //final state q0, q3
            ctrl.idx_isFinal[i] = true;
        else
            ctrl.idx_isFinal[i] = false;
    }
    s.push_front(ctrl.sss);

    // a little hint for defining a transition
    ctrl.trans[0][0] = "al1a"; // d(q0, a, lambda) = (q1, a)
    ctrl.trans[0][1] = "al1a"; // d(q0, a, lambda) = (q1, a)
    ctrl.trans[1][0] = "al1a"; // d(q1, a, lambda) = (q1, a)
    ctrl.trans[1][1] = "al1a"; // d(q1, a, lambda) = (q1, a)
    ctrl.trans[1][2] = "ba2l"; // d(q1, b, a) = (q2, lambda)
    ctrl.trans[2][2] = "ba2l"; // d(q2, b, a) = (q2, lambda)
//    ctrl.trans[2][1] = "l$3$"; // d(q2, lambda, $) = (q3, $)
//    ctrl.trans[2][3] = "l$3$"; // d(q2, lambda, $) = (q3, $)

    int cur_idx = ctrl.idx_init; //현재 state
    int next_trans; //이번 trans index
    string remain_input = (argc == 2) ? argv[1] : ""; //남아있는 input
    string transition; //이번 transition
    print_cur(cur_idx, remain_input, s); //초기 상태 출력
    bool is_fin = false; //프로그램 종료 여부
    if (argc == 2) { //n>=1 일 때
        for (int i = 0; i < MAX_NUM_INPUTS; i++) {
            char inchar = argv[1][i]; //이번 문자
            if (inchar == '\0') { //마지막 문자
                if (cur_idx == 2 && s.front() == '$') //현재 state가 2이면서 스택 심볼이 $ 라면
                    transition = "l$3$";
                else
                    transition = "0000";
                is_fin = true;
            } else {
                next_trans = trans_idx(inchar, s.front()); //다음 trans index
                transition = ctrl.trans[cur_idx][next_trans]; //transition 문자열
            }
            if (transition == "0000") { //0000이면 이동 불가
                cout << "Rejected";
                return 0;
            }
            cur_idx = transition[2] - '0'; //다음 state
            if (transition[1] != 'l') //람다가 아니면 pop
                s.pop_front();
            if (transition[3] != 'l') //람다가 아니면 push
                s.push_front(transition[3]);
            if (remain_input.size() > 0) //남아있는 input 갱신
                remain_input = remain_input.substr(1);
            print_cur(cur_idx, remain_input, s); //상태 출력
            if (is_fin)
                break;
        }
    }
    if (ctrl.idx_isFinal[cur_idx]) //final state 도달
        cout << "Accepted";
    else
        cout << "Rejected";
    return 0;
}