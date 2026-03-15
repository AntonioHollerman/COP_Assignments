#include <stdio.h>

void swap (int *a , int * b );
void bubble_sort (int * arr , int size );
void insertion_sort (int * arr , int size );
void selection_sort (int * arr , int size );
void print_array(int *arr, int size);


int main() {
    int arr[] = {50, 40, 30, 20, 10};
    selection_sort(arr, 5);
    return 0;
}

void print_array(int *arr, int size) {
    for(int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void swap (int *a , int * b ) {
    int tmp = * a ;
    * a = * b ;
    * b = tmp ;
}

void bubble_sort (int * arr , int size ) {
    for (int i = 0; i < size -1; i ++) {
        for (int j = 0; j < size -1 - i ; j ++) {
            if( arr [ j ] > arr [ j +1] ) {
                swap ( & arr [ j ] , & arr [ j +1] ) ; // Helper to swap values
            }
        }
        
        if (i == 2) {
            print_array(arr, size);
        }
    }
}

void insertion_sort (int * arr , int size ) {
    for (int i = 1; i < size ; i ++) {
        int tmp = arr [ i ];
        int j ;
        for ( j = i -1; j >= 0 && tmp < arr [ j ]; j --) {
            arr [ j +1] = arr [ j ];
        }

        arr [ j +1] = tmp ;

        if (i == 2) {
            print_array(arr, size);
        }
    }
}

void selection_sort (int * arr , int size ) { 
    for (int i = 0; i < size -1; i ++) { 
        int min_idx = i ; 
        for (int j = i +1; j < size ; j ++) { 
            if( arr [ j ] < arr [ min_idx ] ) { 
                min_idx = j ; 
            } 
        } 
        if( i != min_idx ) 
            swap ( & arr [ i ] , & arr [ min_idx ] ) ; // Helper to swap values

        if (i == 2) {
            print_array(arr, size);
        }
    } 
}
