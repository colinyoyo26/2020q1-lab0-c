/*
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));

    if (!q) {
        free(q);
        return NULL;
    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    while (q_remove_head(q, NULL, 0))
        ;
    free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    char *str;
    if (!q)
        return false;

    size_t len = strlen(s);
    newh = malloc(sizeof(list_ele_t));
    str = malloc(len * sizeof(char) + 1);

    if (!newh || !str) {
        free(str);
        free(newh);
        return false;
    }

    strncpy(str, s, len);
    str[len] = '\0';
    newh->value = str;

    newh->next = q->head;
    q->head = newh;
    if (!q->tail)
        q->tail = q->head;
    q->size++;
    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newh;
    char *str;

    if (!q || !s)
        return false;

    size_t len = strlen(s);
    newh = malloc(sizeof(list_ele_t));
    str = malloc(len * sizeof(char) + 1);

    if (!newh || !str) {
        free(str);
        free(newh);
        return false;
    }

    strncpy(str, s, len);
    str[len] = '\0';
    newh->value = str;

    if (!q->head) {
        q->head = newh;
        q->tail = newh;
    }

    q->tail->next = newh;
    newh->next = NULL;
    q->tail = newh;
    q->size++;

    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* You need to fix up this code. */
    if (!q || !q->head)
        return false;

    if (bufsize > 0 && sp) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_ele_t *tem = q->head;
    q->head = q->head->next;
    free(tem->value);
    free(tem);
    q->size--;

    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return !q ? 0 : q->size;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size < 2)
        return;

    list_ele_t *cur = q->head, *prev = NULL, *tem;
    q->tail = q->head;

    while (cur) {
        tem = cur;
        cur = cur->next;
        tem->next = prev;
        prev = tem;
    }
    q->head = prev;
}

static void q_merge(queue_t *left, queue_t *right, queue_t *q);

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size < 2)
        return;
    queue_t left, right;
    left.size = (q->size >> 1) + (q->size & 1);
    right.size = q->size >> 1;
    list_ele_t *cur = left.head = q->head;
    right.tail = q->tail;

    for (size_t i = 0; i < left.size - 1; i++)
        cur = cur->next;

    left.tail = cur;
    right.head = cur->next;
    left.tail->next = right.tail->next = NULL;
    q->head = q->tail = NULL;

    q_sort(&left);
    q_sort(&right);
    q_merge(&left, &right, q);
}

static void q_merge(queue_t *left, queue_t *right, queue_t *q)
{
    q->size = left->size + right->size;
    list_ele_t *l = left->head, *r = right->head;
    list_ele_t *tem = NULL;

    q->head = LESS_THAN(left->head->value, right->head->value) ? left->head
                                                               : right->head;
    q->tail = q->head;
    for (size_t i = 0; i < q->size; i++) {
        if (!r || (l && LESS_THAN(l->value, r->value))) {
            tem = l;
            l = l->next;
        } else {
            tem = r;
            r = r->next;
        }
        q->tail->next = tem;
        q->tail = tem;
    }
    tem->next = NULL;
}