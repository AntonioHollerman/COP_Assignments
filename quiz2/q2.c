#include "q2.h"

#include <stdio.h>

/*
    COP 3502C Q2
    This program is written by: Antonio Hollerman

    This file is intentionally incomplete to simulate a written exam.
    Further, you will not be provided the contents of the q2.h file.

    You should first work out your solution using pencil and paper.
    Once you are confident in your answer, type the final solution
    into this file.

    You may ONLY write code inside the bodies of the required functions.
    Do NOT modify function signatures, include directives, or any other
    part of this file.

    Write your answers using a plain text editor only. Do NOT use a
    compiler or an IDE while completing this file. Save your file as q2.c

    You may submit multiple times on Gradescope before the submission 
    deadline. Only the final submission before the deadline will be graded.
*/


#ifdef T1
int sum(int n) {
    // TODO: Complete this function
    // TODO 1 BEGIN
    if (n <= 1) {
        return n;
    }
    // Recursive step: n + sum of (n-1)
    return n + sum(n - 1);

    // TODO 1 END
}
#endif


#ifdef T2
void reverse(char *str, int left, int right) {
    // TODO: Complete this function
    // TODO 2 BEGIN

    if (left >= right) {
        return;
    }

    char temp = str[left];
    str[left] = str[right];
    str[right] = temp;

    reverse(str, left + 1, right - 1);



    // TODO 2 END
}
#endif


#ifdef T3
int max(int *arr, int n) {
    // TODO: Complete this function
    // TODO 3 BEGIN
    if (n == 1) {
        return arr[0];
    }

    int sub_max = max(arr, n - 1);

    if (arr[n - 1] > sub_max) {
        return arr[n - 1];
    } else {
        return sub_max;
    }

    // TODO 3 END
}
#endif
