#include "Semaphore.h"
#include "Fault.h"

namespace DelegateLib {

//------------------------------------------------------------------------------
// Semaphore
//------------------------------------------------------------------------------
Semaphore::Semaphore()
	: m_flag(false)
{
}

//------------------------------------------------------------------------------
// ~Semaphore
//------------------------------------------------------------------------------
Semaphore::~Semaphore()
{
}

//------------------------------------------------------------------------------
// Create
//------------------------------------------------------------------------------
void Semaphore::Create()
{
}

//------------------------------------------------------------------------------
// Reset
//------------------------------------------------------------------------------
void Semaphore::Reset()
{
}

//------------------------------------------------------------------------------
// Wait
//------------------------------------------------------------------------------
bool Semaphore::Wait(int timeout)
{
	std::unique_lock<std::mutex> lk(m_lock);
	std::cv_status status = std::cv_status::no_timeout;
	if (timeout < 0)
	{
		while (!m_flag)
			m_sema.wait(lk);
	}
	else
	{
		while (!m_flag && status == std::cv_status::no_timeout)
			status = m_sema.wait_for(lk, std::chrono::milliseconds(timeout));
	}

	if (m_flag)
	{
		m_flag = false;
		return true;
	}
	else
		return false;
}

//------------------------------------------------------------------------------
// Signal
//------------------------------------------------------------------------------
void Semaphore::Signal()
{
	std::unique_lock<std::mutex> lk(m_lock);
	m_flag = true;
	m_sema.notify_one();
}

}
