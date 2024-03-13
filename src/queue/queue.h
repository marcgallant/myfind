#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

#include "../list/list.h"

struct queue
{
    struct list *head;
    struct list *tail;
    size_t size;
};

/**
 * \brief Allocate a queue on the heap
 *
 * \return The allocated queue
 */
struct queue *queue_init(void);

/**
 * \brief Push at the end of the queue
 *
 * \param queue The queue to push into
 * \param elt The token to push
 */
void queue_push(struct queue *queue, struct ast *elt);

/**
 * \brief Pop at the head of the queue
 *
 * \param queue The queue to pop into
 * \return The popped element or NULL on empty queue
 */
struct ast *queue_pop(struct queue *queue);

/**
 * \brief Destroy the queue
 *
 * Free all elements contained and the queue itself
 *
 * \param queue The queue to destroy
 */
void queue_destroy(struct queue *queue);

#endif /* ! QUEUE_H */
