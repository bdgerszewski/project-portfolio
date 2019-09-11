#include "cell.h"

struct cell {
	int bottom;
	int right;
	int value;
	char color[20];
	int searched;
	int rPos;
	int cPos;
	CELL *pred;
};

CELL *newCELL(int r, int c) {
	CELL *cell = malloc(sizeof(CELL));
	cell->bottom = cell->right = 1;
	cell->value = -1;
	cell->searched = 0;
	cell->rPos = r;
	cell->cPos = c;
	cell->pred = 0;
	strcpy(cell->color, "\x1b[36m");
	return cell;
}
void removeRight(CELL *cell) {
	cell->right = 0;
	return;
}

void removeBottom(CELL *cell) {
	cell->bottom = 0;
	return;
}

int getRight(CELL *cell) {
	return cell->right;
}
int getBottom(CELL *cell) {
	return cell->bottom;
}

void setBottom(CELL *cell, int bottom) {
	cell->bottom = bottom;
	return;
}

void setRight(CELL *cell, int right) {
	cell->right = right;
	return;
}

int getValue(CELL *cell) {
	return cell->value;
}

void setValue(CELL *cell, int val) {
	cell->value = val;
	return;
}
int getSearched(CELL *cell) {
	return cell->searched;
}

void setSearched(CELL *cell, int val) {
	cell->searched = val;
	return;
}

int getR(CELL *cell) {
	return cell->rPos;
}

int getC(CELL *cell) {
	return cell->cPos;
}

CELL *getPred(CELL *cell) {
	return cell->pred;
}

void setPred(CELL *source, CELL *predecessor) {
	source->pred = predecessor;
	return;
}

void setColor(CELL *cell, char *color) {
	strcpy(cell->color, color);
	return;
}

char *getColor(CELL *cell) {
	return cell->color;
}