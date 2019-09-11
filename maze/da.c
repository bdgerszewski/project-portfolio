#include "da.h"

struct da {
	int curSize;
	int capacity;
	int debug;
	void **array;
	void (*display)(void *, FILE *);
	void (*free)(void*);
};

//The constructor returns an initialized DA object. 
DA *newDA(void) {
	DA *items = malloc(sizeof(DA));
	assert(items != 0);
	items->array = malloc(sizeof(void *));
	assert(items->array != 0);
	items->curSize = 0;
	items->capacity = 1;
	items->display = 0;
	items->free = 0;
	items->debug = 0;
	return items;
}

//This method is passed a function that knows how to display the generic
//value stored in an array slot.
void setDAdisplay(DA *items, void(*d)(void *v, FILE *fp)) {
	items->display = d;
	return;
}

//This method is passed a function that knows how to free the generic 
//value stored in an array slot. 
void  setDAfree(DA *items, void(*f)(void *v)) {
	items->free = f;
	return;
}

//The insert method places the given item at the given index. The
//method returns the given index. It should run in amortized constant
//time when inserting an item a constant distance from the back and in 
//linear time otherwise.
void  insertDA(DA *items, int index, void *value) { 
	assert(items->array != 0);
	assert(index >= 0 && index <= items->curSize);
	if (items->curSize >= items->capacity) {
		items->array = realloc(items->array, sizeof(void *) * items->capacity * 2);
		items->capacity *= 2;
		assert(items->array != 0);
	}
	for (int i = items->curSize - 1; i >= index; --i)
		items->array[i + 1] = items->array[i];
	items->array[index] = value;
	items->curSize++;
	return;
}

//The remove method removes the item at the given index. The method 
//returns the removed item. It should run in amortized constant time 
//when removing an item a constant distance from the back and in linear
//time otherwise.
void *removeDA(DA *items, int index) {
	assert(items->curSize > 0);
	assert(index >= 0 && index <= (items->curSize - 1));
	void* removed = items->array[index];
	for (int i = index; i < items->curSize - 1; ++i)
		items->array[i] = items->array[i + 1];
	items->curSize--;
	if (items->curSize == 0) {
		items->capacity = 1;
		items->array = realloc(items->array, sizeof(void *));
	}
	else if ((items->curSize) * 4 < items->capacity && items->capacity != 1) {
		items->array = realloc(items->array, sizeof(void *) * items->capacity / 2);
		items->capacity /= 2;
		assert(items->array != 0);
	}
	return removed;
}

//The union method takes two arrays and moves all the items in the donor
//array to the recipient array. If the recipient array has the items [3,4,5]
//and the donor array has the items [1,2], then, after the union, the donor
//array will be empty and recipient array will have the items [3,4,5,1,2]. 
//It should run in amortized linear time with respect to the size of 
//the donor array. 
void  unionDA(DA *recipient, DA *donor) {
	assert(donor != 0 && recipient != 0);
	void **temp = donor->array;
	for (int i = 0; i < donor->curSize; ++i)
		insertDAback(recipient, temp[i]);
	donor->array = malloc(sizeof(void *));
	donor->curSize = 0;
	donor->capacity = 1;
	free(temp);
	return;
}

//The get method returns the value at the given index. 
//It should run in constant time.
void *getDA(DA *items, int index) {
	assert(index >= 0 && index < items->curSize);
	return items->array[index];
}

//If the given index is equal to the size, the value is inserted via the 
//insert method. The method returns the replaced value or the null pointer 
//if no value was replaced. It should run in constant or amortized constant 
//time (in the case of the insert method being called). 
void *setDA(DA *items, int index, void *value) {
	assert(index >= 0 && index <= items->curSize);
	void *replaced = 0;
	if (index == items->curSize)
		insertDA(items, index, value);
	else {
		replaced = items->array[index];
		items->array[index] = value;
	}
	return replaced;
}

//The size method returns the number of items stored in the array. 
//It should run in amortized constant time.
int   sizeDA(DA *items) {
	return items->curSize;
}

//This visualizing method prints out the filled region, enclosed 
//in brackets and separated by commas, followed by the size of the 
//unfilled region, enclosed in brackets. 
//If the integers 5, 6, 2, 9, and 1 are stored in the array 
//(listed from index 0 upwards) and the array has capacity 8, the method 
//would generate this output:
//    [5, 6, 2, 9, 1]
//with no preceding or following whitespace.  If no display method is set, 
//the address of each item is printed(using %p).Each address is preceded 
//by an @ sign.
void  displayDA(DA *items, FILE *fp) {
	fprintf(fp, "[");
	for (int i = 0; i < items->curSize; ++i) {
		items->display ?
			items->display(items->array[i], fp) :
			fprintf(fp, "@%p", items->array[i]);
		if (i != items->curSize - 1)
			fprintf(fp, "%s", ",");
	}
	if (items->debug) {
		if (items->curSize > 0) fprintf(fp, ",");
		fprintf(fp, "[%d]", items->capacity - items->curSize);
	}
	fprintf(fp, "]");
	return;
}

//The debug method sets an internal flag to the given value. If the flag is 
//greater than zero, the display method adds a comma (if needed) and the 
//number of empty slots (in brackets) immediately after the last element. 
//If the above array had a capacity of 8, the display would be [5,6,2,9,1,[3]].
//An empty array with capacity 1 displays as [[1]].
//The method returns the previous debug value. 
int   debugDA(DA *items, int level) {
	int previous = items->debug;
	items->debug = level;
	return previous;
}

//If no free method is set, the individual items are not freed. In any case, 
//the array and its supporting allocations are freed.
void  freeDA(DA *items) {
	if (items->free)
		for (int i = 0; i < items->curSize; ++i)
			items->free(items->array[i]);
	free(items->array);
	free(items);
	return;
}