#ifndef __MAZE_INCLUDED__
#define __MAZE_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cell.h"

typedef struct maze MAZE;

extern MAZE *newMaze(int r, int c);
extern MAZE *buildMaze(MAZE *maze, int seed);
extern void writeMaze(MAZE *maze, FILE *fp);
extern MAZE *readMaze(FILE *fp);
extern void solveMaze(FILE *input, FILE *output);
extern void printMaze(MAZE *maze, FILE *fp);
extern void freeMaze(MAZE *maze);
extern CELL *checkTop(MAZE *maze, CELL *cell);
extern CELL *checkLeft(MAZE *maze, CELL *cell);
extern CELL *checkRight(MAZE *maze, CELL *cell);
extern CELL *checkBottom(MAZE *maze, CELL *cell);
extern void removeWall(CELL *one, CELL *two);

#endif