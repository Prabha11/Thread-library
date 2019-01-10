#ifndef __THREAD_LIB__
#define __THREAD_LIB__

/**
 * create a thread. 
 * @ip -- stating address of the thread (function pointer)
 * returns 0 on success -1 if failed
 * For now we cannot pass any argumnets to the thread 
 */
int create_thread(void (*ip)(void));

/****************************************************/
/*
 * FixMe: for additional marks you can implement 
 * Note: only for additional marks and for those who 
 * who think sleeping is not for them :) 
 */
/**
 * create a thread passing an argument 
 * @ip -- stating address of the thread (function pointer to a 
 * function which takes a void * as an argument)
 * @arg -- argument for the thread 
 * returns 0 on success -1 if failed
 * For now we cannot pass any argumnets to the thread 
 */
int create_thread_arg(void (*ip)(void *), void *arg);
/****************************************************/



/**
 * yeild the cpu (give a chance for another to run)
 */
void yield(void);

/**
 * The main function runs in the kernel created thread. 
 * This will create all the other threads and once other 
 * threads are created this function will be called and 
 * after that main thread should not be called at all. 
 * You should be able to use the same switch function 
 * that your would use to switch from one thread to another. 
 */
void stop_main(void);

/**
 * a thread wants to die
 */
void delete_thread(void);

#endif


