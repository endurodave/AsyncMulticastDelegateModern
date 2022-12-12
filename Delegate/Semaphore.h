#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include "DelegateOpt.h"
#include <condition_variable>
#include <mutex>
#include <atomic>

#define SEMA std::condition_variable

namespace DelegateLib {

/// @brief A semaphore wrapper class. 
class Semaphore
{
public:
	Semaphore() = default;
	~Semaphore() = default;

	/// Called to wait on a semaphore to be signaled.
	/// @param[in] timeout - timeout in milliseconds
	/// @return Return true if semaphore signaled, false if timeout occurred. 
	bool Wait(std::chrono::milliseconds timeout);

	/// Called to signal a semaphore.
	void Signal();

private:
	// Prevent copying objects
	Semaphore(const Semaphore&) = delete;
	Semaphore& operator=(const Semaphore&) = delete;

	SEMA m_sema;
	std::mutex m_lock;
	std::atomic<bool> m_flag;
};

}

#endif 
