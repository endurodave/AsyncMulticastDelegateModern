#ifndef _WORKER_THREAD_H
#define _WORKER_THREAD_H

#include "DelegateOpt.h"
#if USE_WIN32_THREADS

#include "ThreadWin.h"

/// @brief A worker thread 
class WorkerThread : public ThreadWin
{
public:
	/// Constructor
	/// @param[in] threadName - the thread name. 
	WorkerThread(const CHAR* threadName);

private:
	/// The worker thread entry function
	virtual unsigned long Process (void* parameter);
};

#endif

#endif