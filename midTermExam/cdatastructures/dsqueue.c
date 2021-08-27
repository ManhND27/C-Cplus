#include <stdlib.h>

#include "dsqueue.h"


/* A double-ended queue */

typedef struct _QueueEntry QueueEntry;

struct _QueueEntry {
	QueueValue data;
	QueueEntry *prev;
	QueueEntry *next;
};

struct _Queue {
	QueueEntry *head;
	QueueEntry *tail;
};

Queue *queue_new(void)
{
    Queue *queue = (Queue *) malloc(sizeof(Queue));
	if (queue == NULL) {
		return NULL;
	}

	queue->head = NULL;
	queue->tail = NULL;

	return queue;
}

void queue_free(Queue *queue)
{
	/* Empty the queue */

	while (!queue_isEmpty(queue)) {
		queue_popHead(queue);
	}

	/* Free back the queue */

	free(queue);
}

int queue_pushHead(Queue *queue, QueueValue data)
{
	/* Create the new entry and fill in the fields in the structure */

    QueueEntry *newEntry = malloc(sizeof(QueueEntry));
    if (newEntry == NULL) {
		return 0;
	}

    newEntry->data = data;
    newEntry->prev = NULL;
    newEntry->next = queue->head;

	/* Insert into the queue */

	if (queue->head == NULL) {

		/* If the queue was previously empty, both the head and
		 * tail must be pointed at the new entry */

        queue->head = newEntry;
        queue->tail = newEntry;

	} else {

		/* First entry in the list must have prev pointed back to this
		 * new entry */

        queue->head->prev = newEntry;

		/* Only the head must be pointed at the new entry */

        queue->head = newEntry;
	}

	return 1;
}

QueueValue queue_popHead(Queue *queue)
{
	/* Check the queue is not empty */

	if (queue_isEmpty(queue)) {
		return QUEUE_NULL;
	}

	/* Unlink the first entry from the head of the queue */

    QueueEntry *entry = queue->head;
	queue->head = entry->next;
    QueueValue result = entry->data;

	if (queue->head == NULL) {

		/* If doing this has unlinked the last entry in the queue, set
		 * tail to NULL as well. */

		queue->tail = NULL;
	} else {

		/* The new first in the queue has no previous entry */

		queue->head->prev = NULL;
	}

	/* Free back the queue entry structure */

	free(entry);

	return result;
}

QueueValue queue_peekHead(Queue *queue)
{
	if (queue_isEmpty(queue)) {
		return QUEUE_NULL;
	} else {
		return queue->head->data;
	}
}

int queue_pushTail(Queue *queue, QueueValue data)
{
	/* Create the new entry and fill in the fields in the structure */

    QueueEntry *newEntry = malloc(sizeof(QueueEntry));
    if (newEntry == NULL) {
		return 0;
	}

    newEntry->data = data;
    newEntry->prev = queue->tail;
    newEntry->next = NULL;

	/* Insert into the queue tail */

	if (queue->tail == NULL) {

		/* If the queue was previously empty, both the head and
		 * tail must be pointed at the new entry */

        queue->head = newEntry;
        queue->tail = newEntry;

	} else {

		/* The current entry at the tail must have next pointed to this
		 * new entry */

        queue->tail->next = newEntry;

		/* Only the tail must be pointed at the new entry */

        queue->tail = newEntry;
	}

	return 1;
}

QueueValue queue_popTail(Queue *queue)
{
	/* Check the queue is not empty */

	if (queue_isEmpty(queue)) {
		return QUEUE_NULL;
	}

	/* Unlink the first entry from the tail of the queue */

    QueueEntry *entry = queue->tail;
	queue->tail = entry->prev;
    QueueValue result = entry->data;

	if (queue->tail == NULL) {

		/* If doing this has unlinked the last entry in the queue, set
		 * head to NULL as well. */

		queue->head = NULL;

	} else {

		/* The new entry at the tail has no next entry. */

		queue->tail->next = NULL;
	}

	/* Free back the queue entry structure */

	free(entry);

	return result;
}

QueueValue queue_peekTail(Queue *queue)
{
	if (queue_isEmpty(queue)) {
		return QUEUE_NULL;
	} else {
		return queue->tail->data;
	}
}

int queue_isEmpty(Queue *queue)
{
	return queue->head == NULL;
}
