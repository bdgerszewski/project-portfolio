#ifndef __CDA_INCLUDED__
#define __CDA_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct cda CDA;

extern CDA *newCDA(void); 
extern void setCDAdisplay(CDA *items, void(*display)(void *v, FILE *fp));
extern void setCDAfree(CDA *items, void(*free)(void *v));
extern void insertCDA(CDA *items, int index, void *value);
extern void *removeCDA(CDA *items, int index);
extern void unionCDA(CDA *recipient, CDA *donor);
extern void *getCDA(CDA *items, int index);
extern void *setCDA(CDA *items, int index, void *value);
extern int sizeCDA(CDA *items);
extern void displayCDA(CDA *items, FILE *fp);
extern int debugCDA(CDA *items, int level);
extern void freeCDA(CDA *items);

#define insertCDAfront(items,value) insertCDA(items,0,value)
#define insertCDAback(items,value)  insertCDA(items,sizeCDA(items),value)
#define removeCDAfront(items)       removeCDA(items,0)
#define removeCDAback(items)        removeCDA(items,sizeCDA(items)-1)

#endif