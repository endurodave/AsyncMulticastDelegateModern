#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include "DelegateOpt.h"
#include <condition_variable>
#include <mutex>
#include <atomic>

// Fix compiler error on Windows
#undef max

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
	bool Wait(std::chrono::milliseconds timeout)
	{
        std::unique_lock<std::mutex> lk(m_lock);
        if (timeout == std::chrono::milliseconds::max())
        {
            m_sema.wait(lk, [this] { return m_signaled; });
        }
        else
        {
            if (!m_sema.wait_for(lk, timeout, [this] { return m_signaled; }))
            {
                return false; // Timeout occurred
            }
        }

        if (m_signaled)
        {
            m_signaled = false;
            return true;
        }
        else
        {
            return false;
        }
	}

	/// Called to signal a semaphore.
    void Signal()
    {
        {
            std::unique_lock<std::mutex> lk(m_lock);
            m_signaled = true;
        }
        m_sema.notify_one();
    }

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
