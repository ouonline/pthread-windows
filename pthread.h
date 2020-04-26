#ifndef __PTHREAD_WINDOWS_H__
#define __PTHREAD_WINDOWS_H__

// XXX NOTE: partial pthread support for windows. search param name "unsupported" for more details.

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

typedef HANDLE pthread_t;

int pthread_create(pthread_t* pid, void* unsupported,
                   void* (*f)(void*), void* arg);
pthread_t pthread_self();
int pthread_join(pthread_t pid, void** unsupported);

/* -------------------------------------------------------------------------- */

typedef CRITICAL_SECTION pthread_mutex_t;

int pthread_mutex_init(pthread_mutex_t* mutex, void* unsupported);
int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);
int pthread_mutex_destroy(pthread_mutex_t* mutex);

/* -------------------------------------------------------------------------- */

typedef CONDITION_VARIABLE pthread_cond_t;

int pthread_cond_init(pthread_cond_t* cond, void* unsupported);
int pthread_cond_signal(pthread_cond_t* cond);
int pthread_cond_broadcast(pthread_cond_t* cond);
int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex);
int pthread_cond_destroy(pthread_cond_t* cond);

#ifdef __cplusplus
}
#endif

#endif
