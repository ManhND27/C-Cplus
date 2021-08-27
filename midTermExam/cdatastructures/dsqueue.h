/**
 * @file dsqueue.h
 *
 * @brief Double-ended queue.
 *
 * A double ended queue stores a list of values in order.  New values
 * can be added and removed from either end of the queue.
 *
 * To create a new queue, use @ref queue_new.  To destroy a queue, use
 * @ref queue_free.
 *
 * To add values to a queue, use @ref queue_push_head and
 * @ref queue_push_tail.
 *
 * To read values from the ends of a queue, use @ref queue_popHead
 * and @ref queue_popTail.  To examine the ends without removing values
 * from the queue, use @ref queue_peekHead and @ref queue_peekTail.
 *
 */

#ifndef DSQUEUE_H
#define DSQUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A double-ended queue.
 */

typedef struct _Queue Queue;

/**
 * A value stored in a @ref Queue.
 */

typedef void *QueueValue;

/**
 * A null @ref QueueValue.
 */

#define QUEUE_NULL ((void *) 0)

/**
 * Create a new double-ended queue.
 *
 * @return           A new queue, or NULL if it was not possible to allocate
 *                   the memory.
 */

Queue *queue_new(void);

/**
 * Destroy a queue.
 *
 * @param queue      The queue to destroy.
 */

void queue_free(Queue *queue);

/**
 * Add a value to the head of a queue.
 *
 * @param queue      The queue.
 * @param data       The value to add.
 * @return           Non-zero if the value was added successfully, or zero
 *                   if it was not possible to allocate the memory for the
 *                   new entry.
 */

int queue_pushHead(Queue *queue, QueueValue data);

/**
 * Remove a value from the head of a queue.
 *
 * @param queue      The queue.
 * @return           Value that was at the head of the queue, or
 *                   @ref QUEUE_NULL if the queue is empty.
 */

QueueValue queue_popHead(Queue *queue);

/**
 * Read value from the head of a queue, without removing it from
 * the queue.
 *
 * @param queue      The queue.
 * @return           Value at the head of the queue, or @ref QUEUE_NULL if the
 *                   queue is empty.
 */

QueueValue queue_peekHead(Queue *queue);

/**
 * Add a value to the tail of a queue.
 *
 * @param queue      The queue.
 * @param data       The value to add.
 * @return           Non-zero if the value was added successfully, or zero
 *                   if it was not possible to allocate the memory for the
 *                   new entry.
 */

int queue_pushTail(Queue *queue, QueueValue data);

/**
 * Remove a value from the tail of a queue.
 *
 * @param queue      The queue.
 * @return           Value that was at the head of the queue, or
 *                   @ref QUEUE_NULL if the queue is empty.
 */

QueueValue queue_popTail(Queue *queue);

/**
 * Read a value from the tail of a queue, without removing it from
 * the queue.
 *
 * @param queue      The queue.
 * @return           Value at the tail of the queue, or QUEUE_NULL if the
 *                   queue is empty.
 */

QueueValue queue_peekTail(Queue *queue);

/**
 * Query if any values are currently in a queue.
 *
 * @param queue      The queue.
 * @return           Zero if the queue is not empty, non-zero if the queue
 *                   is empty.
 */

int queue_isEmpty(Queue *queue);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSQUEUE_H */

