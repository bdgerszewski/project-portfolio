#ifndef __STACK_INCLUDED__
#define __STACK_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct stack STACK;

extern STACK *newSTACK(void);
extern void setSTACKdisplay(STACK *, void(*d)(void *, FILE *));
extern void setSTACKfree(STACK *, void(*f)(void *));
extern void push(STACK *items, void *value);
extern void *pop(STACK *items);
extern void *peekSTACK(STACK *items);
extern int sizeSTACK(STACK *items);
extern void displaySTACK(STACK *items, FILE *);
extern int debugSTACK(STACK *items, int level);
extern void freeSTACK(STACK *items);

#endif