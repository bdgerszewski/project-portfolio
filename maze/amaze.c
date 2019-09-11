#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "maze.h"
//#include "cell.h"
//#include "stack.h"
//#include "queue.h"
//#include "cda.h"
//#include "da.h"

void flashIntro(int rows, int cols) {
	char introduction[3][14] = {"   CS  201   ", "  Assign  1  ", "Demonstration"};
	int lines = (rows * 2) + 1;
	int begin = lines / 2;
	int pad = ((cols * 4) - 7) / 2;
	int end = lines % 2 == 1 ? (lines / 2) + 1 : lines / 2;
	for (int i = 0; i < begin; ++i)
		printf("\n");
	printf("\e[?25l");
	fflush(stdout);
	int r = 10, g = 50, b = 25;
	time_t start = time(NULL);
	time_t finish = time(NULL);
	while (finish - start < 5) {
		usleep(100000);
		for (int j = 0; j < 3; ++j) {
			printf("%*c", pad, ' ');
			for (int i = 0; i < 13; ++i)
				printf("\x1b[38;2;%d;%d;%dm" "%c" "\x1b[0m", r, g, b, introduction[j][i]);
			r = (r + 20) % 255;
			g = (g + 23) % 255;
			b = (b + 26) % 255;
			printf("\n");
		}
		fflush(stdout);
		printf("\x1b[A\x1b[A\x1b[A");
		finish = time(NULL);
	}
	for (int i = 0; i < begin; ++i)
		printf("\x1b[A");
	printf("\e[?25h");
	fflush(stdout);
}


int main(int argc, char **argv) {
	int author = 0, solve = 0, create = 0, seedVal = 1, draw = 0;
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-v") == 0) author = 1;
		if (strcmp(argv[i], "-s") == 0) solve = i;
		if (strcmp(argv[i], "-c") == 0) create = i;
		if (strcmp(argv[i], "-r") == 0) seedVal = atoi(argv[i + 1]);
		if (strcmp(argv[i], "-d") == 0) draw = i;
	}
	if (author) {
		printf("Written by Ben Gerszewski\n");
		return 0;
	}
	//creation-->solving-->drawing
	if (create) {
		int r = atoi(argv[create + 1]);
		int c = atoi(argv[create + 2]);
		MAZE *maze = newMaze(r, c);
		FILE *output = fopen(argv[create + 3], "w");
		flashIntro(r, c);
		buildMaze(maze, seedVal);
		writeMaze(maze, output);
		freeMaze(maze);
		fclose(output);
	}
	if (solve) {
		FILE *input = fopen(argv[solve + 1], "r");
		FILE *output = fopen(argv[solve + 2], "w");
		solveMaze(input, output);
		fclose(input);
		fclose(output);
	}
	if (draw) {
		FILE *input = fopen(argv[draw + 1], "r");
		MAZE *maze = readMaze(input);
		printMaze(maze, stdout);
		freeMaze(maze);
		fclose(input);
	}
	return 0;
}