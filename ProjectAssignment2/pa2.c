#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCORES 5
#define MAX_LEN 26
#define INPUT_FILE "scores.txt"
#define EPSILON 1e-6



/*
    COP 3502C PA2
    This program is written by: Antonio Hollerman
*/

typedef struct Cat_s {
    char *name;             // Cat's name (dynamic string)
    char *breed;            // Cat's breed (dynamic string)
    int scores[MAX_SCORES]; // Fixed-sized array for the cat's 5 traits scores
    int baseScore;          // Sum of the cat's 5 traits scores
} Cat;

typedef struct Rivals_s {
    Cat *cat1;              // Pointer to cat1
    Cat *cat2;              // Pointer to cat2
} Rivals;


// Global Variables
const float POSITION_BONUS[10] = {3.0, 5.0, 4.0, 6.0, 7.0, 2.0, 8.0, 1.0, 9.0, 1.5};

// The following 4 variables are the only allowed GLOBAL variables.
// These variables can be used or accessed by any functions. They
// do not have to be declared within a function. Further, try to
// prevent declaring local variables using the same names within
// a function to prevent variable shadowing.

Cat *cats = NULL;
Rivals *rivals = NULL;
double bestPermScore;
int **tracker = NULL;


// Function Prototypes
void myMain(FILE *ifile);
int countHighPerformerTraits(int catIdx, int traitIdx);
int doesSynergyBonusApply(int* teamIndices, int teamSize, int catIdxInTeam);
int doesRivalPenaltyApply(int* teamIndices, int teamSize, int currentIdx, int numRivals);
double calculateTeamScore(int* teamIndices, int teamSize, int numRivals);
void findPermutations(int* perm, int* used, int k, int nCats, int nTeams, int teamSize, int numRivals);

// You may add more functions if necessary





// BEGIN: DO NOT MODIFY THE MAIN FUNCTION
#ifndef MAIN_FUNCTION
int main(void) {
    // Open the input file for reading.
    // This is the only part of the entire code where the file
    // is going to be opened. You should not have any
    // fopen() function call in your functions. Simply
    // pass this existing FILE pointer when necessary.
    FILE *ifile = fopen(INPUT_FILE, "r");

    if( ifile == NULL ) {
        printf("File Does Not Exist!\n");
        return 1;
    }

    // Calls your own main function and passes the file stream
    myMain(ifile);

    // Close the file
    fclose(ifile);

    return 0;
}
#endif
// END: DO NOT MODIFY THE MAIN FUNCTION





// Function Definitions
int countHighPerformerTraits(int catIdx, int traitIdx) {
    if (traitIdx == MAX_SCORES) {
        return 0;
    }
    int count = (cats[catIdx].scores[traitIdx] >= 90) ? 1 : 0;
    return count + countHighPerformerTraits(catIdx, traitIdx + 1);
}

int doesSynergyBonusApply(int* teamIndices, int teamSize, int catIdxInTeam) {
    if (catIdxInTeam == teamSize) return 1;
    int hasOneHigh = 0;
    for (int i = 0; i < MAX_SCORES; i++) {
        if (cats[teamIndices[catIdxInTeam]].scores[i] >= 85) {
            hasOneHigh = 1;
            break;
        }
    }
    if (!hasOneHigh) {
        return 0;
    }
    return doesSynergyBonusApply(teamIndices, teamSize, catIdxInTeam + 1);
}

int doesRivalPenaltyApply(int* teamIndices, int teamSize, int currentIdx, int numRivals) {
    if (currentIdx >= teamSize - 1) return 0;
    Cat* c1 = &cats[teamIndices[currentIdx]];
    Cat* c2 = &cats[teamIndices[currentIdx + 1]];
    for (int i = 0; i < numRivals; i++) {
        if ((rivals[i].cat1 == c1 && rivals[i].cat2 == c2) ||
            (rivals[i].cat1 == c2 && rivals[i].cat2 == c1)) {
            return 1;
        }
    }
    return doesRivalPenaltyApply(teamIndices, teamSize, currentIdx + 1, numRivals);
}

double calculateTeamScore(int* teamIndices, int teamSize, int numRivals) {
    double score = 0;
    double totalBase = 0;
    int highPerformerPoints = 0;

    for (int i = 0; i < teamSize; i++) {
        int cIdx = teamIndices[i];
        totalBase += cats[cIdx].baseScore;
        highPerformerPoints += countHighPerformerTraits(cIdx, 0) * 5;
        score += cats[cIdx].baseScore * (POSITION_BONUS[i] / 100.0);
    }
    score += (totalBase / teamSize);
    score += highPerformerPoints;

    if (doesSynergyBonusApply(teamIndices, teamSize, 0)) {
        score += 30;
    }

    int duplicate = 0;
    for (int i = 0; i < teamSize && !duplicate; i++) {
        for (int j = i + 1; j < teamSize; j++) {
            if (strcmp(cats[teamIndices[i]].breed, cats[teamIndices[j]].breed) == 0) {
                duplicate = 1;
                break;
            }
        }
    }
    score += (duplicate) ? -15 : 10;

    if (doesRivalPenaltyApply(teamIndices, teamSize, 0, numRivals)) {
        score -= 25;
    }

    return score;
}

void findPermutations(int* perm, int* used, int k, int nCats, int nTeams, int teamSize, int numRivals) {
    if (k == nCats) {
        double currentTotal = 0;
        for (int i = 0; i < nTeams; i++) {
            currentTotal += calculateTeamScore(&perm[i * teamSize], teamSize, numRivals);
        }
        if (currentTotal > bestPermScore + EPSILON) {
            bestPermScore = currentTotal;
            for (int i = 0; i < nTeams; i++) {
                for (int j = 0; j < teamSize; j++) {
                    tracker[i][j] = perm[i * teamSize + j];
                }
            }
        }
        return;
    }
    for (int i = 0; i < nCats; i++) {
        if (!used[i]) {
            used[i] = 1;
            perm[k] = i;
            findPermutations(perm, used, k + 1, nCats, nTeams, teamSize, numRivals);
            used[i] = 0;
        }
    }
}

void myMain(FILE *ifile) {
    int n, c, r;
    fscanf(ifile, "%d %d", &n, &c);
    int totalCats = n * c;
    cats = malloc(totalCats * sizeof(Cat));
    tracker = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) tracker[i] = malloc(c * sizeof(int));

    char tN[MAX_LEN], tB[MAX_LEN];
    for (int i = 0; i < totalCats; i++) {
        fscanf(ifile, "%s %s", tN, tB);
        cats[i].name = strdup(tN);
        cats[i].breed = strdup(tB);
        cats[i].baseScore = 0;
        for (int j = 0; j < MAX_SCORES; j++) {
            fscanf(ifile, "%d", &cats[i].scores[j]);
            cats[i].baseScore += cats[i].scores[j];
        }
    }

    fscanf(ifile, "%d", &r);
    rivals = malloc(r * sizeof(Rivals));
    for (int i = 0; i < r; i++) {
        char n1[MAX_LEN], n2[MAX_LEN];
        fscanf(ifile, "%s %s", n1, n2);
        for(int j=0; j<totalCats; j++) {
            if(strcmp(cats[j].name, n1) == 0) {
                rivals[i].cat1 = &cats[j];
            }
            if(strcmp(cats[j].name, n2) == 0) {
                rivals[i].cat2 = &cats[j];
            }
        }
    }

    int *perm = malloc(totalCats * sizeof(int));
    int *used = calloc(totalCats, sizeof(int));
    findPermutations(perm, used, 0, totalCats, n, c, r);

    printf("Best Teams Grouping Score: %.2f\n", bestPermScore);
    double maxT = -1.0;
    int bIdx = 0;
    for (int i = 0; i < n; i++) {
        double s = calculateTeamScore(tracker[i], c, r);
        printf("Team %d:", i + 1);
        for (int j = 0; j < c; j++) {
            printf(" %s", cats[tracker[i][j]].name);
        }
        printf(" %.2f\n", s);
        if (s > maxT + EPSILON) {
            maxT = s; bIdx = i;
        }
    }
    printf("Best Candidate:");
    for (int j = 0; j < c; j++) {
        printf(" %s", cats[tracker[bIdx][j]].name);
    }
    
    printf(" \n");

    for (int i = 0; i < totalCats; i++) {
        free(cats[i].name); free(cats[i].breed);
    }
    free(cats); free(rivals);
    for (int i = 0; i < n; i++) {
        free(tracker[i]);
    }
    free(tracker); free(perm); free(used);
}