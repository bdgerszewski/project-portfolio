#ifndef __QUEUE_INCLUDED__
#define __QUEUE_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct queue QUEUE;

extern QUEUE *newQUEUE(void);
extern void setQUEUEdisplay(QUEUE *items, void(*d)(void *, FILE *));
extern void setQUEUEfree(QUEUE *items, void(*f)(void *));
extern void enqueue(QUEUE *items, void *value);
extern void *dequeue(QUEUE *items);
extern void *peekQUEUE(QUEUE *items);
extern int sizeQUEUE(QUEUE *items);
extern void displayQUEUE(QUEUE *items, FILE *);
extern int debugQUEUE(QUEUE *items, int level);
extern void freeQUEUE(QUEUE *items);

#endif