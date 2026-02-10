#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_FILE "store.txt"
#define MAX_LEN 100
#define ADOPTED 0
#define PENDING 1
#define AVAILABLE 2

/*
    COP 3502C PA1
    This program is written by: Antonio Hollerman
*/

typedef struct Cat_s {
    char *name;         // Cat's name (dynamic string)
    int age;            // Cat's age
    float weight;       // Cat's weight
    char *breed;        // Cat's breed (already allocated dynamic string)
    int status;         // Cat's status 0, 1, or 2 (default) [index to an array]
} Cat;

typedef struct Kennel_s {
    char *location;     // Kennel's location name (dynamic string)
    int occupancy;      // Actual number of cats currently residing
    Cat **cats;         // The cats residing (dynamic array of pointers to struct)
    int maxCapacity;    // The maximum number of cats that can reside
} Kennel;

typedef struct CatStore_s {
    Kennel *kennels;    // Kennels owned by the store (dynamic array)
    int numKennels;     // Number of kennels owned
    int **capacities;   // Dynamic 2D array indicating the constraints across kennels and breeds
} CatStore;


// Global Variable
const char CAT_STATUS[3][20] = {"ADOPTED", "PENDING", "AVAILABLE"};


// Function Prototypes (Required 14 Functions)
char **readBreeds(FILE *ifile, int *count);
char *lookupBreed(char **dictionary, int breedCount, char *breedName);
CatStore *createStore(FILE *ifile, int kennelCount, char **dictionary, int breedCount);
Kennel *createKennels(FILE *ifile, int kennelCount, char **dictionary, int breedCount, int **constraints);
Cat **createCats(FILE *ifile, char **dictionary, int breedCount, int catCount, int maxCap);
Cat *createSingleCat(FILE *ifile, char **dictionary, int breedCount);
int canMoveTo(CatStore *s, char *location, char *breed, char **dictionary, int breedCount);
Kennel *getKennelByCat(CatStore *s, Cat *cat);
int getCatIndex(Kennel *home, Cat *cat);
Cat *getCatByName(CatStore *s, char *catName);
void removeCatFromKennel(Kennel *k, Cat *cat);
void runQueries(FILE *ifile, CatStore *s, char **dictionary, int breedCount, int numQueries);
void freeBreeds(char **dictionary, int breedCount);
void freeStore(CatStore *store, int breedCount);
void query1(CatStore* s, char* breed);
void query2(CatStore* s, int newStatus, char* name);
void query3(CatStore* s, char* name, char* location, char **dictionary, int breedCount);

// You may add more functions if necessary

int getBreedCount(Kennel* k, char* breed){
    int count = 0;

    for (int i = 0; i < k->occupancy; i++) {
        char *cat_breed = k->cats[i]->breed;
        if (strcmp(breed, cat_breed) == 0 && k->cats[i]->status != 0) {
            count++;
        }
    }

    return count;
}

void query1(CatStore* s, char* breed) {
    int foundAny = 0;
    for (int i = 0; i < s->numKennels; i ++) {
        Kennel k = s->kennels[i];
        for (int j = 0; j < k.occupancy; j++) {
            if (strcmp(breed, k.cats[j]->breed) == 0 && k.cats[j]->status != 0) {
                foundAny = 1;
                printf("%s %.2f %d %s %s\n", k.cats[j]->name, k.cats[j]->weight, k.cats[j]->age, k.location, CAT_STATUS[k.cats[j]->status]);
            }
        }
    }

    if (foundAny == 0) {
        printf("No cat with breed %s\n", breed);
    }
}

void query2(CatStore* s, int newStatus, char* name) {
    Cat* cat = getCatByName(s, name);
    cat->status = newStatus;
    printf("%s is now %s!\n", name, CAT_STATUS[newStatus]);
}

void query3(CatStore* s, char* name, char* location, char **dictionary, int breedCount) {
    Cat* cat = getCatByName(s, name);
    Kennel* old_k = getKennelByCat(s, cat);

    Kennel* new_k = NULL;
    for (int i = 0; i < s->numKennels; i++) {
        if (strcmp(location, s->kennels[i].location) == 0) {
            new_k = &s->kennels[i];
            break;
        }
    }

    if (canMoveTo(s, location, cat->breed, dictionary, breedCount) == 0) {
        printf("%s cannot take a %s cat!\n", location, cat->breed);
        return;
    }
    removeCatFromKennel(old_k, cat);

    new_k->cats[new_k->occupancy] = cat;
    new_k->occupancy++;
    printf("%s moved successfully to %s\n", name, location);
}
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

    char **dictionary;      // Dynamic array of dynamic strings
    int breedCount;         // The number of strings in dictionary

    // Allocate the array and populate it
    dictionary = readBreeds(ifile, &breedCount);

    // Read the number of kennels
    int kennelCount;
    fscanf(ifile, "%d", &kennelCount);

    // Create a store and populate it
    CatStore *s = createStore(ifile, kennelCount, dictionary, breedCount);

    // Read the number of queries
    int numQueries;
    fscanf(ifile, "%d", &numQueries);

    // Run the queries
    runQueries(ifile, s, dictionary, breedCount, numQueries);

    // Close the file
    fclose(ifile);

    // Deallocate the store and all memory associated within
    freeStore(s, breedCount);
    // Deallocate the array of dynamic strings
    freeBreeds(dictionary, breedCount);

    return 0;
}
#endif
// END: DO NOT MODIFY THE MAIN FUNCTION



// Function Definitions
char **readBreeds(FILE *ifile, int *count) {
    // TODO: Complete this function
    // TODO 1 BEGIN

    fscanf(ifile, "%d", count);

    char** list = malloc(sizeof(char*) * (*count));

    char buffer[MAX_LEN];

    for (int i = 0; i < *count; i++) {
        fscanf(ifile, "%s", buffer);
        list[i] = malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(list[i], buffer);
    }

    return list;

    // TODO 1 END
}

char *lookupBreed(char **dictionary, int breedCount, char *breedName) {
    // TODO: Complete this function
    // TODO 2 BEGIN
    char* breedPointer = NULL;

    for (int i = 0; i < breedCount; i ++) {
        if (strcmp(dictionary[i], breedName) == 0) {
            breedPointer = dictionary[i];
        }
    }

    return breedPointer;
    // TODO 2 END
}

CatStore *createStore(FILE *ifile, int kennelCount, char **dictionary, int breedCount) {
    // TODO: Complete this function
    // TODO 3 BEGIN
    CatStore* store = malloc(sizeof(CatStore));
    store->capacities = malloc(sizeof(int*) * kennelCount);
    store->numKennels = kennelCount;

    for (int i = 0; i < kennelCount; i++) {
        store->capacities[i] = malloc(sizeof(int) * breedCount);

        for (int j = 0; j < breedCount; j++) {
            fscanf(ifile, "%d", &store->capacities[i][j]);
        }
    }

    store -> kennels = createKennels(ifile, kennelCount, dictionary, breedCount, store->capacities);

    return store;
    // TODO 3 END
}

Kennel *createKennels(FILE *ifile, int kennelCount, char **dictionary, int breedCount, int **constraints) {
    // TODO: Complete this function
    // TODO 4 BEGIN
    Kennel* kennels = malloc(sizeof(Kennel) * kennelCount);

    for (int i = 0; i < kennelCount; i ++) {
        Kennel ken;

        char locationBuffer[MAX_LEN];
        int occupancy = 0;
        fscanf(ifile, "%s %d", locationBuffer, &occupancy);

        ken.occupancy = occupancy;
        ken.location = malloc(sizeof(char) * strlen(locationBuffer) + 1);
        strcpy(ken.location, locationBuffer);

        int maxCap = 0;
        for (int j = 0; j < breedCount; j++) {
            maxCap += constraints[i][j];
        }
        ken.maxCapacity = maxCap;

        ken.cats = createCats(ifile, dictionary, breedCount, occupancy, maxCap);

        kennels[i] = ken;
    }



    return kennels;

    // TODO 4 END
}

Cat **createCats(FILE *ifile, char **dictionary, int breedCount, int catCount, int maxCap) {
    // TODO: Complete this function
    // TODO 5 BEGIN

    Cat** catList = malloc(sizeof(Cat*) * maxCap);

    for (int i = 0; i < catCount; i++) {
        catList[i] = createSingleCat(ifile, dictionary, breedCount);
    }

    for (int i = catCount; i < maxCap; i++) {
        catList[i] = NULL;
    }

    return catList;
    // TODO 5 END
}

Cat *createSingleCat(FILE *ifile, char **dictionary, int breedCount) {
    // TODO: Complete this function
    // TODO 6 BEGIN
    Cat *cat = malloc(sizeof(Cat));
    cat->status = AVAILABLE;

    char name_buffer[MAX_LEN];
    char breed_buffer[MAX_LEN];
    fscanf(ifile, "%s %d %f %s", name_buffer, &cat->age, &cat->weight, breed_buffer);

    cat->name = malloc(sizeof(char) * strlen(name_buffer) + 1);
    strcpy(cat->name, name_buffer);

    cat->breed = lookupBreed(dictionary, breedCount, breed_buffer);

    return cat;
    // TODO 6 END
}

int canMoveTo(CatStore *s, char *location, char *breed, char **dictionary, int breedCount) {
    // TODO: Complete this function
    // TODO 7 BEGIN
    Kennel* k;
    int kenIndex = -1;
    int breedIndex = -1;

    for (int i = 0; i < s->numKennels; i++) {
        if (strcmp(location, s->kennels[i].location) == 0) {
            k = &s->kennels[i];
            kenIndex = i;
            break;
        }
    }

    for (int i = 0; i < breedCount; i ++) {
        if (strcmp(breed, dictionary[i]) == 0 ) {
            breedIndex = i;
        }
    }

    int maxCap = s->capacities[kenIndex][breedIndex];
    int breedOcc = getBreedCount(k, breed);

    return breedOcc >= maxCap ? 0 : 1;
    // TODO 7 END
}

Kennel *getKennelByCat(CatStore *s, Cat *cat) {
    // TODO: Complete this function
    // TODO 8 BEGIN


    for (int i = 0; i < s->numKennels; i ++) {
        if (getCatIndex(&s->kennels[i], cat)  != -1) {
            return &s->kennels[i];
        }
    }

    return NULL;
    // TODO 8 END
}

int getCatIndex(Kennel *home, Cat *cat) {
    // TODO: Complete this function
    // TODO 9 BEGIN

    for (int i = 0; i < home->occupancy; i ++) {
        if (cat == home->cats[i]) {
            return i;
        }
    }


    return -1;
    // TODO 9 END
}

Cat *getCatByName(CatStore *s, char *catName) {
    // TODO: Complete this function
    // TODO 10 BEGIN

    for (int i = 0; i < s->numKennels; i ++) {
        Kennel curKen = s->kennels[i];

        for (int j = 0; j < curKen.occupancy; j ++) {
            if (strcmp(catName, curKen.cats[j]->name) == 0) {
                return curKen.cats[j];
            }
        }
    }

    return NULL;
    // TODO 10 END
}

void removeCatFromKennel(Kennel *k, Cat *cat) {
    int catIndex = getCatIndex(k, cat);
    if (catIndex == -1) return;

    for (int i = catIndex; i < k->occupancy - 1; i++) {
        k->cats[i] = k->cats[i + 1];
    }

    k->cats[k->occupancy - 1] = NULL;
    k->occupancy--;
}

void runQueries(FILE *ifile, CatStore *s, char **dictionary, int breedCount, int numQueries) {
    // TODO: Complete this function
    // TODO 12 BEGIN


    for (int i = 0; i < numQueries; i++) {
        int query;
        char breed[MAX_LEN];
        int status;
        char name[MAX_LEN];
        char location[MAX_LEN];

        fscanf(ifile, "%d", &query);

        if (query == 1) {
            fscanf(ifile, "%s", breed);
            query1(s, breed);
        }

        if (query == 2) {
            fscanf(ifile, "%d %s", &status, name);
            query2(s, status, name);
        }

        if (query == 3) {
            fscanf(ifile, "%s %s", name, location);
            query3(s, name, location, dictionary, breedCount);
        }
    }


    // TODO 12 END
}

void freeBreeds(char **dictionary, int breedCount) {
    // TODO: Complete this function
    // TODO 13 BEGIN

    if (dictionary == NULL) {
        return;
    }

    for (int i = 0; i < breedCount; i++) {
        free(dictionary[i]);
    }

    free(dictionary);

    // TODO 13 END
}

void freeStore(CatStore *store, int breedCount) {
    if (store == NULL) return;

    for (int i = 0; i < store->numKennels; i++) {
        Kennel *ken = &(store->kennels[i]);

        // Free the string inside the kennel
        free(ken->location);

        for (int j = 0; j < ken->occupancy; j++) {
            if (ken->cats[j] != NULL) {
                free(ken->cats[j]->name);
                // Note: We don't free ken->cats[j]->breed because
                // that points to the dictionary, which is freed later.
                free(ken->cats[j]);
            }
        }
        free(ken->cats);
    }

    // FIX: You must free each row of the 2D capacities array
    for (int i = 0; i < store->numKennels; i++) {
        free(store->capacities[i]);
    }
    free(store->capacities);

    free(store->kennels);
    free(store);
}

// You may add more functions if necessary
