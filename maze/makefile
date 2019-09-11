OPTS = -Wall -Wextra -std=c99
OBJS = da.o cda.o stack.o queue.o cell.o maze.o amaze.o
all: amaze

cell.o : cell.c cell.h
	gcc $(OPTS) -c cell.c

da.o : da.c da.h
	gcc $(OPTS) -c da.c
	
cda.o : cda.c cda.h
	gcc $(OPTS) -c cda.c
	
stack.o : stack.c stack.h da.h
	gcc $(OPTS) -c stack.c
	
queue.o : queue.c queue.h cda.h
	gcc $(OPTS) -c queue.c

maze.o : maze.c stack.h cell.h da.h cda.h queue.h maze.h
	gcc $(OPTS) -c maze.c
	
amaze.o : amaze.c cell.h da.h cda.h stack.h queue.h maze.h
	gcc $(OPTS) -c amaze.c

amaze : $(OBJS)
	gcc $(OPTS) $(OBJS) -o amaze

clean :
	rm -f $(OBJS) amaze

cleanMaze: 
	rm -f *.m *.maze

better :
	gcc -Wall -Wextra -std=c99 da.c cda.c stack.c queue.c cell.c betterMaze.c amaze.c -o better

test: amaze
	./amaze -r 17 -c 8 8 out.maze -s out.maze out.solved -d out.solved

test1: amaze
	./amaze -c 3 3 hopeful.maze -d hopeful.maze

test2 : amaze
	./amaze -r 2 -c 10 10 out -d out

test3 : amaze
	./amaze -c 3 3 m.data -s m.data m.solved -d m.solved

valgrind : amaze
	valgrind ./amaze -r 17 -c 8 8 out.maze -s out.maze out.solved -d out.solved
