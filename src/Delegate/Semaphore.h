#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include "DelegateOpt.h"
#include <condition_variable>
#include <mutex>
#include <atomic>

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

	std::condition_variable m_sema;
	std::mutex m_lock;
	bool m_signaled = false;
};

}

#endif 
