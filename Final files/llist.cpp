#include "llist.hpp"
#include <cstdlib>			//malloc

void queueInit(queue* currentQueue) {
	currentQueue->head = NULL;
	currentQueue->tail = NULL;
	currentQueue->nodesCounter = 0;
}

int queueIsEmpty(queue* currentQueue) {
	return !(currentQueue->nodesCounter);
}

node* queueAddLast(queue* currentQueue, void* currentData) {
	node* temp = (node*)malloc(sizeof(node));
	temp->nodeData = currentData;
	if (queueIsEmpty(currentQueue)) {
		currentQueue->head = temp;
		currentQueue->tail = temp;
		temp->next = NULL;
	}
	else {
		currentQueue->tail->previous = temp;
		temp->next = currentQueue->tail;
		currentQueue->tail = temp;
	}
	temp->previous = NULL;
	currentQueue->nodesCounter++;
	return temp;
}

void* queueGetAndRemoveFirst(queue* currentQueue) {
	if (queueIsEmpty(currentQueue)) {
		//printf("Trying to get and remove element from empty queue.\n");
		return NULL;
	} else {
		void* temp = currentQueue->head->nodeData;
		if (currentQueue->nodesCounter == 1) {
			free(currentQueue->head);
			currentQueue->head = NULL;
			currentQueue->tail = NULL;
		} else {
			currentQueue->head = currentQueue->head->previous;
			free(currentQueue->head->next);
			currentQueue->head->next = NULL;
		}
		currentQueue->nodesCounter--;
		return temp;
	}
}

void queueDestroy(queue* currentQueue) {
	while (!queueIsEmpty(currentQueue)) {
		queueGetAndRemoveFirst(currentQueue);
	}
	return;
}
