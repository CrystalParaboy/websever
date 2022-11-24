
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include "locker.h"

template<typename T>
class threadpool{
public:
    /*thread_number是线程池中线程的数量，max_requests是请求队列中最多允许的、等待处理的请求的数量*/
    threadpool(int thread_number=8, int max_request=10000);
    ~threadpool();
    bool append(T *request, int state);
    bool append_p(T *request);
private:
    static void *worker(void *arg);
    void run();

    int m_thread_number;        //线程池中的线程数
    int m_max_requests;         //请求队列中允许的最大请求数
    pthread_t *m_threads;       //描述线程池的数组，其大小为m_thread_number
    std::list<T*> m_workqueue; //请求队列
    locker m_queuelocker;       //保护请求队列的互斥锁
    sem m_queuestat;            //是否有任务需要处理
    bool m_stop;
};

template<typename T>
threadpool<T>::threadpool(int threadpool_number, int max_requests):
                m_thread_number(thread_number),m_max_requests(max_requests),
                                m_stop(false),m_threads(NULL)
{
    
}
/*add work to request quene*/
