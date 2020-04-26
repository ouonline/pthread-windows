#include "pthread.h"
#include <synchapi.h>
#include <stdlib.h>

/* -------------------------------------------------------------------------- */

int pthread_mutex_init(pthread_mutex_t* mutex, void* unsupported) {
    if (unsupported) {
        return -1;
    }
    InitializeCriticalSection(mutex);
    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t* mutex) {
    DeleteCriticalSection(mutex);
    return 0;
}

int pthread_mutex_lock(pthread_mutex_t* mutex) {
    EnterCriticalSection(mutex);
    return 0;
}

int pthread_mutex_unlock(pthread_mutex_t* mutex) {
    LeaveCriticalSection(mutex);
    return 0;
}

/* -------------------------------------------------------------------------- */

int pthread_cond_init(pthread_cond_t* cond, void* unsupported) {
    if (unsupported) {
        return -1;
    }
    InitializeConditionVariable(cond);
    return 0;
}

int pthread_cond_destroy(pthread_cond_t* cond) {
    return 0;
}

int pthread_cond_signal(pthread_cond_t* cond) {
    WakeConditionVariable(cond);
    return 0;
}

int pthread_cond_broadcast(pthread_cond_t* cond) {
    WakeAllConditionVariable(cond);
    return 0;
}

int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
    SleepConditionVariableCS(cond, mutex, INFINITE);
    return 0;
}

/* -------------------------------------------------------------------------- */

typedef void* (*pthread_routine_t)(void*);

typedef struct {
    pthread_routine_t func;
    void* arg;
} pthread_param_t;

static DWORD WINAPI thread_wrapper(LPVOID p) {
    pthread_param_t* param = (pthread_param_t*)p;
    pthread_routine_t func = param->func;
    void* arg = param->arg;
    free(param);
    func(arg);
    return 0;
}

int pthread_create(pthread_t* pid, void* unsupported,
                   void* (*f)(void*), void* arg) {
    if (unsupported) {
        return -1;
    }
    pthread_param_t* param = (pthread_param_t*)malloc(sizeof(pthread_param_t));
    if (!param) {
        return -1;
    }

    param->func = f;
    param->arg = arg;
    HANDLE h = CreateThread(NULL, 0, thread_wrapper, param, 0, NULL);
    if (!h) {
        free(param);
        return -1;
    }
    *pid = h;
    return 0;
}

pthread_t pthread_self() {
    return GetCurrentThread();
}

int pthread_join(pthread_t pid, void** unsupported) {
    if (unsupported) {
        return -1;
    }
    WaitForSingleObject(pid, INFINITE);
    CloseHandle(pid);
    return 0;
}
