/*
 *Describe:	sbuf.h
 *	This struct is use to the producer-consumer moudle.
 *	It provides a critical zone for shared finite buffer through
 *	semaphore.
 */
#ifndef __SBUF_H__
#define __SBUF_H__
#include "csapp.h"
typedef struct{
	int *buf;	/* Buffer array */
	int n;		/* Maxinum number of slots */
	int front;	/* buf[(front+1)%n] is first item*/
	int rear;	/* buf[rear%n] is last item */
	sem_t mutex;	/* Protects accesses to buf*/
	sem_t slots;	/* Counts available slots */
	sem_t items;	/* Counts available items */
}sbuf_t;
/* Create an empty,bounded,shared FIFO buffer with n slots */
extern void sbuf_init(sbuf_t *sp,int n);

/* free dynamic allocated buffer*/
extern void sbuf_deinit(sbuf_t *sp);

/*Insert item into the rear of shared buffer sp*/
extern void sbuf_insert(sbuf_t *sp,int item);

/* Remove and return the first item from sp*/
extern int sbuf_remove(sbuf_t *sp);

extern int get_sbuf_items_count(sbuf_t *sp);
#endif
