#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "maze.h"
#include "cell.h"
#include "stack.h"

void srandom(unsigned int);
long int random(void);
#define RESET   "\x1b[0m"
struct maze {
	int rows;
	int cols;
	CELL ***cellArray;
};

MAZE *newMaze(int r, int c) {
	MAZE *maze = malloc(sizeof(MAZE));
	maze->rows = r;
	maze->cols = c;
	maze->cellArray = malloc(sizeof(CELL **) * maze->rows);
	for (int i = 0; i < maze->rows; ++i) {
		maze->cellArray[i] = malloc(sizeof(CELL *) * maze->cols);
		for (int j = 0; j < maze->cols; ++j) {
			maze->cellArray[i][j] = newCELL(i, j);
		}
	}
	return maze;
}

void printMaze(MAZE *maze, FILE *fp) {
	printf("\e[?25l");
	for (int j = 0; j < maze->cols; j++) {
		if (j == 0) fprintf(fp, "-----");
		else fprintf(fp, "----");
	}
	fprintf(fp, "\n");
	for (int i = 0; i < maze->rows; ++i) {
		//sides
		for (int j = 0; j < maze->cols; ++j) {
			char val = ' ';
			if (getValue(maze->cellArray[i][j]) != -1) val = (char)((getValue(maze->cellArray[i][j]) % 10) + 48);
			if (i == 0 && j == 0) fprintf(fp, " ");
			else if (j == 0) fprintf(fp, "|");
			if (getRight(maze->cellArray[i][j]) && !(i == maze->rows - 1 && j == maze->cols - 1))
				fprintf(fp, "%s %c " RESET "|", getColor(maze->cellArray[i][j]), val);
			else fprintf(fp, "%s %c  " RESET, getColor(maze->cellArray[i][j]), val);
		}
		fprintf(fp, "\n");
		//bottom
		for (int j = 0; j < maze->cols; ++j) {
			if (i == maze->rows - 1) {
				if (j == 0) fprintf(fp, "-----");
				else fprintf(fp, "----");
			}
			else if (getBottom(maze->cellArray[i][j])) {
				if (j == 0) fprintf(fp, "-----");
				else fprintf(fp, "----");
			}
			else {
				if (j == 0) fprintf(fp, "-   -");
				else fprintf(fp, "   -");
			}
		}
		fprintf(fp, "\n");
	}
	for (int i = 0; i < (maze->rows * 2) + 1; ++i)
		printf("\x1b[A");
	return;
}
CELL *checkTop(MAZE *maze, CELL *cell) {
	int r = getR(cell);
	int c = getC(cell);
	if (r == 0) return 0;
	if (getSearched(maze->cellArray[r - 1][c]) == 0) {
		return maze->cellArray[r - 1][c];
	}
	return 0;
}
CELL *checkLeft(MAZE *maze, CELL *cell) {
	int r = getR(cell);
	int c = getC(cell);
	if (c == 0) return 0;
	if (getSearched(maze->cellArray[r][c - 1]) == 0) {
		return maze->cellArray[r][c - 1];
	}
	return 0;
}
CELL *checkRight(MAZE *maze, CELL *cell) {
	int r = getR(cell);
	int c = getC(cell);
	if (c == maze->cols - 1) {
		return 0;
	}
	if (getSearched(maze->cellArray[r][c + 1]) == 0) {
		return maze->cellArray[r][c + 1];
	}
	return 0;
}
CELL *checkBottom(MAZE *maze, CELL *cell) {
	int r = getR(cell);
	int c = getC(cell);
	if (r == maze->rows - 1) {
		return 0;
	}
	if (getSearched(maze->cellArray[r + 1][c]) == 0) {
		return maze->cellArray[r + 1][c];
	}
	return 0;
}

void removeWall(CELL *one, CELL *two) {
	if (getR(one) == getR(two)) {
		if (getC(one) < getC(two))
			removeRight(one);
		else
			removeRight(two);
	}
	else {
		if (getR(one) < getR(two))
			removeBottom(one);
		else
			removeBottom(two);
	}
	return;
}

int getNeighbors(MAZE *maze, CELL *cell, CELL **candidates) {
	int count = 0;
	if (checkTop(maze, cell))
		candidates[count++] = checkTop(maze, cell);
	if (checkLeft(maze, cell))
		candidates[count++] = checkLeft(maze, cell);
	if (checkRight(maze, cell))
		candidates[count++] = checkRight(maze, cell);
	if (checkBottom(maze, cell))
		candidates[count++] = checkBottom(maze, cell);
	return count;
}

MAZE *buildMaze(MAZE *maze, int seed) {
	srandom(seed);
	int count;
	CELL *cell = maze->cellArray[0][0];
	STACK *stack = newSTACK();
	push(stack, (void *)cell);
	CELL *temp;
	int range[9] = { 199, 196, 202, 184, 47, 112, 87, 27, 165 };
	char color[20] = "\0";
	int steps = 0;
	while (sizeSTACK(stack) != 0) {
		CELL *candidates[3];
		cell = (CELL *)peekSTACK(stack);
		setSearched(cell, 1);
		count = getNeighbors(maze, cell, candidates);
		if (count) {
			int select = random() % count;
			removeWall(cell, candidates[select]);
			setValue(cell, 0);
			int choice = range[steps % 9];
			steps++;
			sprintf(color, "\x1b[38;5;%dm", choice);
			setColor(cell, color);
			printMaze(maze, stdout);
			setValue(cell, -1);
			usleep(200000);
			push(stack, (void *)candidates[select]);
		}
		else {
			temp = pop(stack);
			setValue(temp, 0);
			int choice = range[steps % 9];
			sprintf(color, "\x1b[38;5;%dm", choice);
			setColor(cell, color);
			printMaze(maze, stdout);
			setValue(temp, -1);
			usleep(100000);
		}
	}
	freeSTACK(stack);
	return maze;
}

void writeMaze(MAZE *maze, FILE *fp) {
	fprintf(fp, "%d %d\n", maze->rows, maze->cols);
	for (int i = 0; i < maze->rows; ++i) {
		for (int j = 0; j < maze->cols; ++j) {
			fprintf(fp, "%d ", getBottom(maze->cellArray[i][j]));
			fprintf(fp, "%d ", getRight(maze->cellArray[i][j]));
			fprintf(fp, "%d\t", getValue(maze->cellArray[i][j]));
		}
		fprintf(fp, "\n");
	}
	return;
}

MAZE *readMaze(FILE *fp) {
	int r, c, bottom, right, value;
	fscanf(fp, "%d %d", &r, &c);
	MAZE *maze = newMaze(r, c);
	maze->rows = r;
	maze->cols = c;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			fscanf(fp, "%d %d %d", &bottom, &right, &value);
			setBottom(maze->cellArray[i][j], bottom);
			setRight(maze->cellArray[i][j], right);
			setValue(maze->cellArray[i][j], value);
		}
	}
	return maze;
}

CELL *goUp(MAZE *maze, CELL *cell) {
	if (checkTop(maze, cell)) {
		CELL *temp = checkTop(maze, cell);
		if (getBottom(temp) == 0)
			return temp;
	}
	return 0;
}
CELL *goLeft(MAZE *maze, CELL *cell) {
	if (checkLeft(maze, cell)) {
		CELL *temp = checkLeft(maze, cell);
		if (getRight(temp) == 0)
			return temp;
	}
	return 0;
}
CELL *goRight(MAZE *maze, CELL *cell) {
	if (checkRight(maze, cell) && getRight(cell) == 0)
		return checkRight(maze, cell);
	return 0;
}
CELL *goDown(MAZE *maze, CELL *cell) {
	if (checkBottom(maze, cell) && getBottom(cell) == 0)
		return checkBottom(maze, cell);
	return 0;
}
int directed(QUEUE *queue, MAZE *maze, CELL *cell, int steps) {
	int value = getValue(cell) + 1;
	//int num = (random() % 9);
	int range[9] = { 199, 196, 202, 184, 47, 112, 87, 27, 165 };
	int select = range[steps % 9];
	int count = 0;
	char color[20] = "\0";
	sprintf(color, "\x1b[38;5;%dm", select);
	if (goUp(maze, cell)) {
		count++;
		CELL *temp = goUp(maze, cell);
		setPred(temp, cell);
		setValue(temp, value);
		enqueue(queue, (void *)goUp(maze, cell));
		setColor(temp, color);
	}
	if (goLeft(maze, cell)) {
		count++;
		CELL *temp = goLeft(maze, cell);
		setPred(temp, cell);
		setValue(temp, value);
		enqueue(queue, (void *)goLeft(maze, cell));
		setColor(temp, color);
	}
	if (goRight(maze, cell)) {
		count++;
		CELL *temp = goRight(maze, cell);
		setPred(temp, cell);
		setValue(temp, value);
		enqueue(queue, (void *)goRight(maze, cell));
		setColor(temp, color);
	}
	if (goDown(maze, cell)) {
		count++;
		CELL *temp = goDown(maze, cell);
		setPred(temp, cell);
		setValue(temp, value);
		setColor(temp, color);
		enqueue(queue, (void *)goDown(maze, cell));
	}
	return count;
}

void solveMaze(FILE *input, FILE *output) {
	MAZE *maze = readMaze(input);
	QUEUE *queue = newQUEUE();
	CELL *cell = maze->cellArray[0][0];
	setColor(cell, "\x1b[38;5;199m");
	setValue(cell, 0);
	CELL *goal = maze->cellArray[maze->rows - 1][maze->cols - 1];
	enqueue(queue, (void *)cell);
	int steps = 1;
	while (sizeQUEUE(queue) != 0) {
		cell = (CELL *)dequeue(queue);
		if (cell == goal) {
			if (getPred(goal))
				setValue(goal, getValue(getPred(goal)) + 1);
			break;
		}
		int div = sizeQUEUE(queue) + 1;
		//Problem isn't whether it's branching or not, problem is how many branches there are.  sizeQUEUE?
		if (directed(queue, maze, cell, steps)) {
			steps++;
			usleep(200000 / div);
			printMaze(maze, stdout);
		}
		setSearched(cell, 2);
	}
	printMaze(maze, stdout);
	freeQUEUE(queue);
	for (int i = 0; i < (maze->rows * 2) + 1; ++i)
		printf("\n");
	writeMaze(maze, output);
	freeMaze(maze);
	return;
}

void freeMaze(MAZE *maze) {
	for (int i = 0; i < maze->rows; ++i) {
		for (int j = 0; j < maze->cols; ++j)
			free((CELL *)maze->cellArray[i][j]);
		free((CELL **)maze->cellArray[i]);
	}
	free((CELL ***) maze->cellArray);
	free(maze);
	return;
}