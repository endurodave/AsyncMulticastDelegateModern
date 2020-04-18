#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include "DelegateOpt.h"
#include "DataTypes.h"

#if USE_WIN32_THREADS
	// On Windows API, the semaphore is a Windows handle
	#define SEMA HANDLE
#elif USE_STD_THREADS	
	#include <condition_variable>
	#include <mutex>
	#include <atomic>
	// On std API, the semaphore is a condition variable
	#define SEMA std::condition_variable
#else
	#error Must implement the Semaphore class on non-Windows platforms
	#define SEMA int
#endif

namespace DelegateLib {

/// @brief A semaphore wrapper class. 
class Semaphore
{
public:
	Semaphore();
	~Semaphore();

	/// Called to create the semaphore. Calling multiple times on the same instance
	// is safe and will not recreate the semaphore. 
	void Create();

	/// Rest the semaphore in readiness to be signalled again. 
	void Reset();

	/// Called to wait on a semaphore to be signaled.
	/// @param[in] timeout - timeout in milliseconds. If less 0, wait time is infinite. 
	/// @return Return true if semaphore signaled, false if timeout occurred. 
	bool Wait(int timeout);

	/// Called to signal a semaphore.
	void Signal();

private:
	// Prevent copying objects
	Semaphore(const Semaphore&) = delete;
	Semaphore& operator=(const Semaphore&) = delete;

	SEMA m_sema;

#if USE_STD_THREADS	
	std::mutex m_lock;
	std::atomic<bool> m_flag;
#endif
};

}

#endif 
