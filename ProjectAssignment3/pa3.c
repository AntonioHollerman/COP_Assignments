#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_FILE "logs.txt"

/*
    COP 3502C PA3
    This program is written by: Antonio Hollerman
*/

typedef struct Cat_s {
    int arrival;            // Cat's arrival time at the clinic
    // Minutes since clinic opened (unique)
    char *name;             // Cat's unique name (dynamic string)
    int duration;           // Required treatment time in minutes
} Cat;

typedef struct SLLNode_s {
    Cat *cat;               // Pointer to a dynamically allocated Cat
    struct SLLNode_s *next;
} SLLNode;

typedef struct CatList_s {
    SLLNode *head;
} CatList;
// Function Prototypes
void myMain(FILE *ifile);


// You may add more functions if necessary
void addCat(CatList *list, Cat *cat);
void freeCats(CatList *list);
void populateFullList(FILE *ifile, CatList *list);
void reversePrintList(SLLNode *head);


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
void myMain(FILE *ifile) {
    // TODO: Complete this function
    CatList *allCats = malloc(sizeof(CatList));
    allCats->head = NULL;
    CatList *exposed = malloc(sizeof(CatList));
    exposed->head = NULL;

    populateFullList(ifile, allCats);

int unoFreeAt = 0;
int dosFreeAt = 0;
SLLNode *ptr = allCats->head;

    for (int minute = 0; minute < 480; minute++) {
        if (ptr == NULL) {
            break;
        }

        while (ptr != NULL && ptr->cat->arrival <= minute) {
            if (ptr->cat->duration == 0) {
                ptr = ptr->next;
                continue;
            }

            if (unoFreeAt <= minute) {
                if (minute + ptr->cat->duration <= 480) {
                    printf("Doctor Uno treated %s at %d\n", ptr->cat->name, minute);
                    unoFreeAt = minute + ptr->cat->duration;
                } else {
                    printf("Cannot accommodate %s\n", ptr->cat->name);
                }
                ptr = ptr->next;
            }
            else if (dosFreeAt <= minute) {
                if (minute + ptr->cat->duration <= 480) {
                    printf("Doctor Dos treated %s at %d\n", ptr->cat->name, minute);
                    dosFreeAt = minute + ptr->cat->duration;
                    SLLNode *exposedNode = malloc(sizeof(SLLNode));
                    exposedNode->cat = ptr->cat;
                    exposedNode->next = exposed->head;
                    exposed->head = exposedNode;

                } else {
                    printf("Cannot accommodate %s\n", ptr->cat->name);
                }
                ptr = ptr->next;
            }
            else {
                break;
            }
        }
    }

    while (ptr != NULL) {
        if (ptr->cat->duration != 0) {
            printf("Cannot accommodate %s\n", ptr->cat->name);
        }
        ptr = ptr->next;
    }

    if (exposed->head == NULL) {
        printf("No Exposed Cats\n");
    }else {
        printf("Exposed Cats\n");
    }

    reversePrintList(exposed->head);

    free(exposed);
    freeCats(allCats);
}

void freeCats(CatList *list) {
    SLLNode *ptr = list->head;
    SLLNode *lastPtr = NULL;
    while (ptr != NULL) {
        lastPtr = ptr;
        ptr = ptr->next;

        free(lastPtr->cat->name);
        free(lastPtr->cat);
        free(lastPtr);
    }

    free(list);
}

void addCat(CatList *list, Cat *cat) {
    SLLNode *ptr = list->head;
    SLLNode *nextPtr = NULL;
    if (ptr == NULL) {
        ptr = malloc(sizeof(SLLNode));
        ptr->cat = cat;
        list->head = ptr;
        list->head->next = NULL;
        return;
    }

    if (cat->arrival < ptr->cat->arrival) {
        SLLNode *newPtr = malloc(sizeof(SLLNode));
        newPtr->cat = cat;

        list->head = newPtr;
        list->head->next = ptr;
        return;
    }

    while (ptr->next != NULL) {
        nextPtr = ptr->next;
        if (ptr->cat->arrival <= cat->arrival && cat->arrival <= nextPtr->cat->arrival) {
            SLLNode *newPtr = malloc(sizeof(SLLNode));
            newPtr->cat = cat;

            ptr->next = newPtr;
            ptr->next->next = nextPtr;
            return;
        }

        ptr = ptr->next;
    }

    ptr->next = malloc(sizeof(SLLNode));
    ptr->next->cat = cat;
    ptr->next->next = NULL;
}

void populateFullList(FILE *ifile, CatList *list) {
    int arrivalTime = 0;
    char name[50];
    int duration = 0;

    fscanf(ifile, "%d", &arrivalTime);

    while (arrivalTime != -1) {
        fscanf(ifile, "%s", name);
        fscanf(ifile, "%d", &duration);

        Cat *newCat = malloc(sizeof(Cat));

        newCat->arrival = arrivalTime;
        newCat->name = malloc(sizeof(char) * strlen(name) + 1);
        newCat->duration = duration;
        strcpy(newCat->name, name);

        addCat(list, newCat);
        fscanf(ifile, "%d", &arrivalTime);
    }
}

void reversePrintList(SLLNode *head) {

    SLLNode *ptr = head;
    while (ptr != NULL) {
        printf("%s\n", ptr->cat->name);
        ptr = ptr->next;
    }
}