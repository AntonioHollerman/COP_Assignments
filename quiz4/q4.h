//
// Created by tonio on 3/1/2026.
//

#ifndef COP_ASSIGNMENTS_Q4_H
#define COP_ASSIGNMENTS_Q4_H
typedef struct Player_s {
    char *name;
    int hp;
    struct Player_s *next;
} Player;

typedef struct Queue_s {
    Player *front;
    Player *back;
    int size;
} Queue;

typedef struct Stack_s {

} Stack;
void enqueue(Queue *q, Player *p); // Enqueues player p to the queue q
int size(Queue *q); // Returns the number of elements in q
Player *dequeue(Queue *q); // Removes front node, returns ptr to it
Player *front(Queue *q); // Returns pointer to front node
void delete_player(Player *p); // Frees memory pointed to by player

void initialize(Stack *s); // Initializes an empty stack (does not allocate)
int push(Stack *s, char value); // Pushes the char value onto the stack
int is_empty(Stack *s); // Returns 1 if the stack is empty, 0 otherwise
char pop(Stack *s); // Pops and returns the character at the top
char peek(Stack *s); // Returns the character at the top

#endif //COP_ASSIGNMENTS_Q4_H