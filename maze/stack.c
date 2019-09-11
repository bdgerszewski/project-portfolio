#include "stack.h"
#include "da.h"

struct stack {
	DA *DA;
	int debug;
	void(*display)(void *, FILE *);
	void(*free)(void *);
};

STACK *newSTACK(void) {
	STACK *items = malloc(sizeof(STACK));
	assert(items != 0);
	items->debug = 0;
	items->DA = newDA();
	setDAdisplay(items->DA, 0);
	setDAfree(items->DA, 0);
	items->display = 0;
	items->free = 0;
	return items;
}
void setSTACKdisplay(STACK *items, void(*d)(void *, FILE *)) {
	items->display = d;
	setDAdisplay(items->DA, d);
	return;
}
void setSTACKfree(STACK *items, void(*f)(void *)) {
	items->free = f;
	setDAfree(items->DA, f);
	return;
}
void push(STACK *items, void *value) { 
	insertDA(items->DA, sizeDA(items->DA), value);
	return;
}
void *pop(STACK *items){
	assert(sizeDA(items->DA) > 0);
	return removeDA(items->DA, sizeDA(items->DA) -1);
}
void *peekSTACK(STACK *items){
	assert(sizeDA(items->DA) > 0);
	return getDA(items->DA, sizeDA(items->DA) - 1);
}
int sizeSTACK(STACK *items){
	return sizeDA(items->DA);
}
void displaySTACK(STACK *items, FILE *fp){
	if (items->debug == 0) {
		fprintf(fp, "|");
		for (int i = sizeDA(items->DA) - 1; i > -1 ; --i) {
			items->display ?
				items->display(getDA(items->DA, i), fp) :
				fprintf(fp, "@%p", getDA(items->DA, i));
			if (i != 0)
				fprintf(fp, ",");
		}
		fprintf(fp, "|");
	}
	else
		displayDA(items->DA, fp);
	return;
}
int debugSTACK(STACK *items, int level){
	if (level == 1) debugDA(items->DA, 0);
	else if (level == 2) debugDA(items->DA, 1);
	int old = items->debug;
	items->debug = level;
	return old;
}
void freeSTACK(STACK *items){
	freeDA(items->DA);
	free(items);
	return;
}