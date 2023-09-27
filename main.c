#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void read_automaton(const char* file_name, int* alphabet_size, int* state_count, int* s0, int* final_state_count, int** final_states, int*** transitions) {
    FILE* file = fopen(file_name, "r");

    fscanf(file, "%d", alphabet_size);
    fscanf(file, "%d", state_count);
    fscanf(file, "%d", s0);
    fscanf(file, "%d", final_state_count);

    *final_states = (int*) malloc((*final_state_count) * sizeof(int));
    for(int i = 0; i < (*final_state_count); i++) {
        fscanf(file, "%d", (*final_states) + i);
    }

    *transitions = (int**) malloc((*state_count) * sizeof(int*));
    for(int i = 0; i < (*state_count); i++) {
        (*transitions)[i] = (int*) malloc((*alphabet_size) * sizeof(int));
        for(int j = 0; j < (*alphabet_size); j++) {
            (*transitions)[i][j] = -1;
        }
    }

    int s, sp, a;
    while (fscanf(file, "%d %d %d", &s, &a, &sp) != EOF) {
        (*transitions)[s][a] = sp;
    }

    fclose(file);
}

bool is_accepted_by_automaton(const char* w0, int s0, int state_count, const int* final_states, int final_state_count, int** transitions) {
    int current_state = s0;

    for (const char* c = w0; *c != '\0'; c++) {
        int symbol = *c - 'a';
        current_state = transitions[current_state][symbol];

        if(current_state == -1) {
            return false;
        }
    }

    for(int i = 0; i < final_state_count; i++) {
        if (current_state == final_states[i]) {
            return true;
        }
    }

    return false;
}

int main() {
    int alphabet_size, state_count, s0, final_state_count;
    int* final_states;
    int** transitions;

    read_automaton("automaton.txt", &alphabet_size, &state_count, &s0, &final_state_count, &final_states, &transitions);

    char w0[256];
    printf("Enter w0: ");
    scanf("%s", w0);

    bool accepted = is_accepted_by_automaton(w0, s0, state_count, final_states, final_state_count, transitions);

    if (accepted) {
        printf("The automaton can accept a word of the form w = w0w1.\n");
    } else {
        printf("The automaton cannot accept any word w = w0w1.\n");
    }

    free(final_states);
    for(int i = 0; i < state_count; i++) {
        free(transitions[i]);
    }
    free(transitions);

    return 0;
}