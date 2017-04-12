#ifndef LLIST_H_
#define LLIST_H_

typedef struct node node;

struct node{
	node* previous;
	node* next;
	void*  nodeData;
};

typedef struct {
	node* head;
	node* tail;
	int nodesCounter;
}queue;

void queueInit(queue*);
int queueIsEmpty(queue*);
node* queueAddLast(queue*, void*);
void* queueGetAndRemoveFirst(queue*);
void queueDestroy(queue*);


#endif /* LLIST_H_ */
