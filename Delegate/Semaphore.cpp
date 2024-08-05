#include "Semaphore.h"
#include "Fault.h"

namespace DelegateLib {

//------------------------------------------------------------------------------
// Wait
//------------------------------------------------------------------------------
bool Semaphore::Wait(std::chrono::milliseconds timeout)
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

//------------------------------------------------------------------------------
// Signal
//------------------------------------------------------------------------------
void Semaphore::Signal()
{
	{
		std::unique_lock<std::mutex> lk(m_lock);
		m_signaled = true;
	}
	m_sema.notify_one();
}

}
