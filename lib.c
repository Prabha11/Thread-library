#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "threadlib.h"
#include "threadimp.h"

#ifndef __x86_64__
#error "NOT 64bit" 
#endif


#define DEBUG
#undef DEBUG /* uncomment when you are done! */

/* 
 * Idea here is that, if DEBUG is defined PRINT will be 
 * printf else it would be nothing. 
 * Use PRINT to print all your debug information and when you 
 * undefine DEBUG all that code will be gone :) 
 */ 
#ifdef DEBUG
 #define PRINT   printf
#else 
 #define PRINT(...)
#endif

/* Track the current thread
 */ 
tcb_t __current_thread;

/** Data structures and functions to support thread control box */ 
  struct queue Que;
  struct queue * Q = &Que;
  int Qinitzed = 0;
/** end of data structures */

/* You might need other information about the 
 * thread system as well. 
 */
tcb_t current_thread(void)
{
  return __current_thread;
}

void set_current(tcb_t current)
{
  __current_thread = current;
}

/*Creates a new tcb*/
tcb_t newtcb(void)
{ 
  tcb_t dummy = (tcb_t) malloc(sizeof(struct tcb));
	
	/* might want to initialize the fields in the TCB */
	return dummy;
}

int append(tcb_t newguy)
{
  return -1; 
}

int delete_item (tcb_t thread)
{
  return -1;
}


tcb_t schedule(tcb_t current)
{
  /* decide who should run next */
  return current; 

}

void switch_threads(tcb_t newthread /* addr. of new TCB */,
		    tcb_t oldthread /* addr. of old TCB */) {

  /* This is basically a front end to the low-level assembly 
   * code to switch. */
  /* Might have store the SP values back to TCB */ 
  machine_switch(newthread -> sp,oldthread -> sp);

  /* When you do this call, you should return back to this 
   * point in a different thread. 
   * When you are eventually rescheduled, return through 
   * the calling stack 
   */ 
  return; 

}


/*********************************************************
 *                 Thread creation etc 
 *********************************************************/
#define STACK_SIZE (sizeof(void *) * 1024)
#define FRAME_REGS 48 // is this correct for x86_64?

#include <stdlib.h>
#include <assert.h>

int create_thread(void (*ip)(void))
{  
    long int  *stack; 
    stack = malloc_stack();
    if(!stack) return -1; /* no memory? */
    if(!Qinitzed){
            queueCreate(Q);
            Qinitzed = 1;
    }

  /**
   * Stack layout: last slot should contain the return address and I should have some space 
   * for callee saved registers. Also, note that stack grows downwards. So need to start from the top. 
   * Should be able to use this code without modification Basic idea: C calling convention tells us the top 
   * most element in the stack should be return ip. So we create a stack with the address of the function 
   * we want to run at this slot. 
   */
    *(stack) = (long int) ip;
    TCB newtcb = malloc(sizeof(tcb_t));
    newtcb->sp = (void *) stack-64;
    enqueue(Q, newtcb); 
    return 0;
}

void yield()
{
  TCB old = dequeue(Q);
  enqueue(Q, old);
  TCB new = queuePeek(Q);
  switch_threads(new, old); 
}


void delete_thread(void)
{

  /* When a user-level thread calls this you should not 
   * let it run any more but let others run
   * make sure to exit when all user-level threads are dead */   
    TCB old = dequeue(Q);
    free(old);
    if(queueIsEmpty(Q)) exit(0);
    else {
        TCB temp = malloc(sizeof(tcb_t));
        TCB new = queuePeek(Q);
        switch_threads(new, temp);
        free(temp);
    } 
}

#include <assert.h>
void stop_main(void)
{ 
  /* Main function was not created by our thread management system. 
   * So we have no record of it. So hijack it. 
   * Do not put it into our ready queue, switch to something else.*/

    TCB temp = malloc(sizeof(tcb_t));
    TCB new = queuePeek(Q);
    switch_threads(new, temp);
    free(temp);
}

#include <stdlib.h>


/*
 * allocate some space for thread stack.
 * malloc does not give size aligned memory 
 * this is some hack to fix that.
 * You can use the code as is. 
 */
void * malloc_stack() 
{
  /* allocate something aligned at 16
   */
   void *ptr = malloc(STACK_SIZE + 16);
   if (!ptr) return NULL;
   ptr = (void *)(((long int)ptr & (-1 << 4)) + 0x10);
   return ptr;
}


