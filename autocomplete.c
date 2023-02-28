#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "autocomplete.h"

//typedef struct term{
  //  char term[200]; // assume terms are not longer than 200
    //double weight;
//} term;

// Compares the terms alphabetically.
int compare(const void *a, const void *b) {
    term *p_a = (term *)a;
    term *p_b = (term *)b;
    return strcmp((p_a)->term, (p_b)->term);
}

// Compares the weights.
int nonincreasing_order(const void *a2, const void *b2){
  term *p_a2 = (term *)a2;
  term *p_b2 = (term *)b2;
  return ((p_b2)->weight - (p_a2)->weight);
} 

void read_in_terms(term **terms, int *pnterms, char *filename) 
{
    FILE *fp = fopen(filename, "r");
    char line[200];

    fgets(line, sizeof(line), fp);
    *pnterms = atoi(line);

    *terms = (term *)malloc((*pnterms)*sizeof(term));
    char t_weight[20];
    
    // Iterates through each line.
    for (int i = 0; i <= (*pnterms); i++) {
        fgets(line, 200, fp);
        line[200] = '\0';
        int z = 0;
        int q = 0;

        // Individually assigns each character to the weight or term, removing white spaces.
        for (int j = 0; j < strlen(line); j++) {
            if (line[j] >= '0' && line[j] <= '9') {
                t_weight[q] = line[j];
                q++;
            } else if (line[j] == ' ' && line[j-1] != ' ' && line[j+1] != ' ') {
                (*terms)[i].term[z] = line[j];
                z++;
            } else if ((line[j] < '0' || line[j] > '9') && line[j] != '\t' && line[j] != ' ') {
                (*terms)[i].term[z] = line[j];
                z++;
            } 
        }

        //  Converts the weight from a string to a number & ends the term/weight lines.
        (*terms)[i].term[z-1] = '\0';
        t_weight[q] = '\0';
        double temp2 = atof(t_weight);
        (*terms)[i].weight = temp2;   
    }

    qsort((*terms), (*pnterms), sizeof(term), compare);
    fclose(fp);
} 

int lowest_match(term *terms, int nterms, char *substr)
{
    int low = 0;
    int high = nterms - 1;
    int mid;
    int length = strlen(substr);

    // Compares the values to find the lowest by looking at the shared part of the term.
    while (low <= high){
        mid = ((low + high) / 2) ;

        char result[length];
        strncpy(result, terms[mid].term, length);
        int num = strcmp(result, substr);

        if (num < 0) {
            low = mid + 1;
        } else if (num > 0){
            high = mid - 1;
        } else {
            if (mid == low){
                return mid;
            } else{
                high = mid; 
            }
        }
    } 

    char result[length];
    strncpy(result, terms[low].term, length);
    result[length] = '\0';
    if (strcmp(result, substr) != 0) {
        return -1;
    } else {
        return low; 
    }
} 

int highest_match(term *terms, int nterms, char *substr)
{
    int low = 0;
    int high = nterms - 1;
    int mid;
    int length = strlen(substr);

    while (low <= high){
        mid = ((low + high) / 2) ;
        char result[length];
        strncpy(result, terms[mid].term, length);
        result[length] = '\0';
        int num = strcmp(result, substr);

        if (num < 0) {
            low = mid + 1;
        } else if (num > 0){
            high = mid - 1;
        } else {
            // Compares specifically matching terms to find the highest.
            char result2[length];
            strncpy(result2, terms[high].term, length);
            result2[length] = '\0';
            int num2 = strcmp(result2, substr);        

            if (num2 < 0) {
                low = mid + 1;
            } else if (num > 0){
                high = mid - 1;
            }
            if (mid == high){
                return mid;
            } else{
                low = mid + 1; 
            }
        }
    } 

    char result[length];
    strncpy(result, terms[high].term, length);
    result[length] = '\0';
    if (strcmp(result, substr) != 0) {
        return -1;
    } else {
        return high; 
    }
} 

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr) {
    // Finds the number of terms for results.
    int low_index = lowest_match(terms, nterms, substr);
    int high_index = highest_match(terms, nterms, substr);

    // Checks if there are any matching terms. 
    if (low_index == -1) {
        *n_answer = 0;
    } else {
        *n_answer = (high_index - low_index) + 1;
    }
    

    // Allocates space for a new term, answer.
    *answer = (term *)malloc((*n_answer + 1)*sizeof(term));

    // Puts the corresponding results into the new array.
    for (int i = 0; i < (*n_answer); i++) { 
        for (int j = 0; j < strlen(terms[low_index + i].term); j++) {
            (*answer)[i].term[j] = terms[low_index + i].term[j];
        }
        (*answer)[i].weight = terms[low_index + i].weight; // lets go no underlines ?
    }

    // Sorts by weight.
    qsort((*answer), (*n_answer), sizeof(term), nonincreasing_order);
}


/*
int main() {
    term *dictionary;
    int pnterms;
    char *filename = "wiktionary.txt";
    read_in_terms(&dictionary, &pnterms, filename);

    char *substr = "tor"; //29
    printf("%d\n", lowest_match(dictionary, pnterms, substr)); // 7
    printf("%d", highest_match(dictionary, pnterms, substr)); // 28

    term *answer;
    int n_answer; 
    autocomplete(&answer, &n_answer, dictionary, pnterms, substr);

    for (int i = 0; i < n_answer; i++) {
        printf("%s\n", answer[i].term);
        printf("%lf\n", answer[i].weight); //;-; WOW YAYYYYYYYYYYYYY
        printf("%d\n", i);
    }

    free(dictionary);
    free(answer);

    return 0;
}
*/