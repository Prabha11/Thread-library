#ifndef __THREAD_LIB_IMP__
#define __THREAD_LIB_IMP__

/**
 * some functions and data structures for implementing 
 * threads. 
 */


/******* Thread Control Block (TCB) **********
**********************************************
 * the following structure traks information about 
 * the thread. You would have to put what you require 
 * into this structure 
 */
struct tcb { 
  void *sp;  /* Address of stack pointer. 
	      * Keep this as first element would ease switch.S 
	      * You can do something else as well. 
	      */
  /* you will need others stuff */ 
};

typedef struct tcb * tcb_t;

/***********************************************/ 
/**
 * assembly code for switching 
 * @sp -- new stack to switch 
 * return sp of the old thread
 * 
 * Switching 
*/
void machine_switch(void *newstack /* addr. of new TCB */, 
		    void *oldstack /* addr. of old TCB */);

void switch_threads(tcb_t newthread /* addr. of new TCB */, 
		    tcb_t oldthread /* addr. of old TCB */);
		    

/* Data structure to support threads */
/*
 * Allocate a new tcb. Return NULL if you do not have 
 * memory or cannot allocate 
 */ 
tcb_t newtcb(void) ;

/* scheduler specific stff */
/*
 * a new thread is created, add it to the current 
 * thread pool. (think of a suitable data structure 
 * for this 
 * return 0 on scuccess, -1/or error code on failure 
 */ 
int append(tcb_t new_thread);


/*
 * Remove the said thread from thread pool
 * it should not be picked again
 */ 
int delete_item (tcb_t thread);

/*
 * return the address of the currently running thread 
 */
tcb_t current_thread(void);


/*
 * set the current thread 
 */
void set_current(tcb_t); 

/*
 * return the next thread to run. 
 * Your scheduling algorithm should run here to decide 
 * which thread should run next
 */
tcb_t schedule(tcb_t current); 

/***********************************************************
 *        Stack related functions and definitions          *
 ***********************************************************/

/* For each thread, we allocate this much of stack space 
 * Keep in mind this space will be used for function calls 
 * and local variables and we have no guard against stack 
 * overflow. So be careful here to allocate sufficient space. 
 * If space is too large malloc might fail. 
 */ 
#define STACK_SIZE (sizeof(void *) * 1024)

/* How many bytes do we need to save from the thread's 
 * context when we want to switch. 
 */ 
#define FRAME_SIZE 48 // is this correct for 64bits?

/* 
 * Allocate some space for a stack of a new thread 
 */ 
void * malloc_stack(void); 


#endif
