#include "queue.h"
#include "cda.h"

struct queue {
	CDA *CDA;
	int debug;
	void (*display)(void *, FILE *);
	void (*free)(void*);
};

QUEUE *newQUEUE(void){
	QUEUE *queue = malloc(sizeof(QUEUE));
	assert(queue != 0);
	queue->CDA = newCDA();
	queue->display = 0;
	queue->free = 0;
	queue->debug = 0;
	setCDAdisplay(queue->CDA, 0);
	setCDAfree(queue->CDA, 0);
	return queue;
}
void setQUEUEdisplay(QUEUE *items, void(*d)(void *, FILE *)) {
	items->display = d;
	setCDAdisplay(items->CDA, d);
	return;
}
void setQUEUEfree(QUEUE *items, void(*f)(void *)) {
	items->free = f;
	setCDAfree(items->CDA, f);
	return;
}
void enqueue(QUEUE *items, void *value){
	insertCDA(items->CDA, sizeCDA(items->CDA), value);
	return;
}
void *dequeue(QUEUE *items){
	assert(sizeQUEUE(items) > 0);
	return removeCDA(items->CDA, 0);
}
void *peekQUEUE(QUEUE *items){
	assert(sizeQUEUE(items) > 0);
	return getCDA(items->CDA, 0);
}
int sizeQUEUE(QUEUE *items){
	return sizeCDA(items->CDA);
}
void displayQUEUE(QUEUE *items, FILE *fp){
	if (items->debug == 0) {
		fprintf(fp, "<");
		for (int i = 0; i < sizeCDA(items->CDA); ++i) {
			items->display ?
				items->display(getCDA(items->CDA, i), fp) :
				fprintf(fp, "@%p", getCDA(items->CDA, i));
			if (i != sizeCDA(items->CDA) - 1)
				fprintf(fp, "%s", ",");
		}
		fprintf(fp, ">");
	}
	else
		displayCDA(items->CDA, fp);
	return;
}
int debugQUEUE(QUEUE *items, int level) {
	if (level == 1) debugCDA(items->CDA, 0);
	else if (level == 2) debugCDA(items->CDA, 1);
	int old = items->debug;
	items->debug = level;
	return old;
}
void freeQUEUE(QUEUE *items){
	freeCDA(items->CDA);
	free(items);
	return;
}