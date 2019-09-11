#ifndef __DA_INCLUDED__
#define __DA_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct da DA;

extern DA   *newDA(void);
extern void  setDAdisplay(DA *, void(*d)(void *v, FILE *fp));
extern void  setDAfree(DA *, void(*d)(void *v));
extern void  insertDA(DA *items, int index, void *value);
extern void *removeDA(DA *items, int index);
extern void  unionDA(DA *recipient, DA *donor);
extern void *getDA(DA *items, int index);
extern void *setDA(DA *items, int index, void *value);
extern int   sizeDA(DA *items);
extern void  displayDA(DA *items, FILE *fp);
extern int   debugDA(DA *items, int level);
extern void  freeDA(DA *items);

#define insertDAback(items,value) insertDA(items,sizeDA(items),value)
#define removeDAback(items)       removeDA(items,sizeDA(items)-1)
#endif