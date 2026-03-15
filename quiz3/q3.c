// #include "q3.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct SLLNode_s {
    int data ;
    struct SLLNode_s * next ;
} SLLNode ;
/*
    COP 3502C Q3
    This program is written by: Your Full Name

    This file is intentionally incomplete to simulate a written exam.
    Further, you will not be provided the contents of the q3.h file.

    You should first work out your solution using pencil and paper.
    Once you are confident in your answer, type the final solution
    into this file.

    You may ONLY write code inside the bodies of the required functions.
    Do NOT modify function signatures, include directives, or any other
    part of this file.

    Write your answers using a plain text editor only. Do NOT use a
    compiler or an IDE while completing this file. Save your file as q3.c

    You may submit multiple times on Gradescope before the submission
    deadline. Only the final submission before the deadline will be graded.
*/


#ifdef T1
void solve(int *solution, int size) {
    // TODO: Complete this function
    // TODO 1 BEGIN

    // Put your answers inside the curly braces
    // and separate each value with a comma. An
    // example is provided for reference.

    int answer[] = {
        1, 3, 5, 4, 2               // If your answer is: 1, 2, 3, 4, 5
    };

    // DO NOT MODIFY THIS PORTION
    for(int i = 0; i < size; i++)
        solution[i] = answer[i];

    // TODO 1 END
}
#endif


#ifdef T2
void append(SLLNode *head1, SLLNode *head2) {
    // TODO: Complete this function
    // TODO 2 BEGIN

    SLLNode *ptr = head1;
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }

    ptr->next = head2;


    // TODO 2 END
}
#endif


#ifdef T3
SLLNode *merge(SLLNode *ptr1, SLLNode *ptr2) {
    // TODO: Complete this function
    // TODO 3 BEGIN

    SLLNode* head;
    if (ptr1 == NULL) {
        return ptr2;
    }

    if (ptr2 == NULL) {
        return ptr1;
    }

    if (ptr1->data < ptr2->data) {
        head = ptr1;
        head->next = merge(ptr1->next, ptr2);
    } else {
        head = ptr2;
        head->next = merge(ptr1, ptr2->next);
    }

    return head;
    // TODO 3 END
}
#endif


#ifdef T4
SLLNode *reverse(SLLNode *head) {
    // TODO: Complete this function
    // TODO 4 BEGIN

    SLLNode *tail = head;
    SLLNode *prev = NULL;

    if (head->next == NULL) {
        return head;
    }

    while (tail->next != NULL) {
        prev = tail;
        tail = tail->next;
    }
    prev->next = NULL;
    tail->next = reverse(head);

    return tail;
    // TODO 4 END
}
#endif
