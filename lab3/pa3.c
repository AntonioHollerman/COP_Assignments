#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 101
#define LIMIT_SECONDS 600
#define INPUT_FILE "songs.txt"

typedef struct Song_s {
    int song_id;
    char title[MAX_LEN];
    int length;
} Song;

void permutation_init(const Song *songs, int n);
void permutation_fill(Song *solution, int size, int pos,
                      const Song *actual, int *is_used);
void permutation_print(const Song *solution, int size);

void subset_init(const Song *songs, int n);
int subset_fill(int *solution, int size, int pos, const Song *actual);
int subset_check(int *solution, int size, const Song *actual);
void subset_print(int *solution, int size, const Song *actual);

Song *read_songs(FILE *ifile, int *count);
void destroy_songs(Song *songs);
void song_print(const Song *s);

#ifndef MAIN_FUNCTION
int main(void) {
    FILE *ifile = fopen(INPUT_FILE, "r");

    if( ifile == NULL ) {
        printf("File Does Not Exist!\n");
        return 1;
    }

    int song_count;
    Song *songs = read_songs(ifile, &song_count);
    fclose(ifile);

    printf("Shuffle Playlists\n");
    printf("=================\n");
    permutation_init(songs, song_count);

    printf("\n");

    printf("Subset Playlists\n");
    printf("================\n");
    subset_init(songs, song_count);

    destroy_songs(songs);

    return 0;
}
#endif

void permutation_init(const Song *songs, int n) {
    Song *solution = malloc(sizeof(Song) * n);
    int *is_used = calloc(n, sizeof(int));
    permutation_fill(solution, n, 0, songs, is_used);
    free(solution);
    free(is_used);
}

void permutation_fill(Song *solution, int size, int pos,
                      const Song *actual, int *is_used) {
    if (pos == size) {
        permutation_print(solution, size);
        return;
    }

    for (int i = 0; i < size; i++) {
        if (!is_used[i]) {
            is_used[i] = 1;
            solution[pos] = actual[i];
            permutation_fill(solution, size, pos + 1, actual, is_used);
            is_used[i] = 0;
        }
    }
}

void permutation_print(const Song *solution, int size) {
    for (int i = 0; i < size; i++) {
        song_print(&solution[i]);
        if (i < size - 1) {
            printf(" | ");
        }
    }
    printf("\n");
}

void subset_init(const Song *songs, int n) {
    int *solution = malloc(sizeof(int) * n);
    int total = subset_fill(solution, n, 0, songs);
    printf("%d\n", total);
    free(solution);
}

int subset_fill(int *solution, int size, int pos, const Song *actual) {
    if (pos == size) {
        if (subset_check(solution, size, actual)) {
            subset_print(solution, size, actual);
            return 1;
        }
        return 0;
    }

    int count = 0;
    solution[pos] = 1;
    count += subset_fill(solution, size, pos + 1, actual);

    solution[pos] = 0;
    count += subset_fill(solution, size, pos + 1, actual);

    return count;
}

int subset_check(int *solution, int size, const Song *actual) {
    int total_time = 0;
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (solution[i] == 1) {
            total_time += actual[i].length;
            count++;
        }
    }
    return (count > 0 && total_time <= LIMIT_SECONDS);
}

void subset_print(int *solution, int size, const Song *actual) {
    int first = 1;
    for (int i = 0; i < size; i++) {
        if (solution[i] == 1) {
            if (!first) {
                printf(" | ");
            }
            song_print(&actual[i]);
            first = 0;
        }
    }
    printf("\n");
}

Song *read_songs(FILE *ifile, int *count) {
    if(!ifile || !count) return NULL;
    fscanf(ifile, "%d", count);
    Song *songs = malloc(sizeof(Song) * (*count));
    if(!songs) return NULL;
    for(int i = 0; i < *count; i++) {
        fscanf(ifile, "%d", &songs[i].song_id);
        fscanf(ifile, "%s", songs[i].title);
        fscanf(ifile, "%d", &songs[i].length);
    }
    return songs;
}

void destroy_songs(Song *songs) {
    free(songs);
}

void song_print(const Song *s) {
    printf("%s (%d)", s->title, s->length);
}