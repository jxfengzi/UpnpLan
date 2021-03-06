/*
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   TinyThread.h
 *
 * @remark
 *
 */

#ifndef __TINY_THREAD_H__
#define __TINY_THREAD_H__

#include "tiny_base.h"

#ifdef _WIN32
#else
#include <pthread.h> 
#endif

TINY_BEGIN_DECLS


#ifdef _WIN32
typedef void *              ThreadHandler;
typedef unsigned long       ThreadId;
#else /* Linux */
typedef int                 ThreadHandler;
typedef pthread_t           ThreadId;
#endif /* _WIN32 */

#define THREAD_NAME_LEN     256

typedef enum _TinyThreadStatus
{
    ThreadInit = 0,
    ThreadRunning = 1,
    ThreadStop = 2,
} TinyThreadStatus;

typedef void (*ThreadLoop)(void *param);

typedef struct _TinyThread
{
    char                    name[THREAD_NAME_LEN];
    TinyThreadStatus        status;
    ThreadLoop              loop;
    ThreadHandler           thread_handler;
    ThreadId                thread_id;
    void *                  thread_param;
} TinyThread;

TinyThread * TinyThread_New(void);
TinyRet TinyThread_Construct(TinyThread *thiz);
TinyRet TinyThread_Initialize(TinyThread *thiz, ThreadLoop loop, void *param, const char *name);
TinyRet TinyThread_Dispose(TinyThread *thiz);
void TinyThread_Delete(TinyThread *thiz);

const char * TinyThread_GetName(TinyThread *thiz);
bool TinyThread_Start(TinyThread *thiz);
bool TinyThread_Join(TinyThread *thiz);


TINY_END_DECLS

#endif /* __TINY_THREAD_H__ */
