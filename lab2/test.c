//
// Created by tonio on 2/3/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_FILE "things.txt"
#define MAX_LEN 100
#define ADOPTED 0
#define PENDING 1
#define AVAILABLE 2
int main() {
    int size = 0;
    FILE *ifile = fopen(INPUT_FILE, "r");
    fscanf(ifile, "%d", &size);
    for (int i = 0; i < size; i++) {
        int num = 0;
        fscanf(ifile, "%d", &num);
        printf("%d\n", num);
    }
}