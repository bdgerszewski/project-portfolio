#ifndef __CELL_INCLUDED__
#define __CELL_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct cell CELL;

extern CELL *newCELL(int r, int c);
extern void removeRight(CELL *cell);
extern void removeBottom(CELL *cell);
extern int getRight(CELL *cell);
extern int getBottom(CELL *cell);
extern void setBottom(CELL *cell, int bottom);
extern void setRight(CELL *cell, int right);
extern int getValue(CELL *cell);
extern void setValue(CELL *cell, int val);
extern int getSearched(CELL *cell);
extern void setSearched(CELL *cell, int val);
extern int getR(CELL *cell);
extern int getC(CELL *cell);
extern CELL *getPred(CELL *cell);
extern void setPred(CELL *source, CELL *pred);
extern void setColor(CELL *cell, char *color);
extern char *getColor(CELL *cell);

#endif