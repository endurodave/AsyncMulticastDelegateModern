#include "Semaphore.h"
#include "Fault.h"

namespace DelegateLib {

//------------------------------------------------------------------------------
// Semaphore
//------------------------------------------------------------------------------
Semaphore::Semaphore()
#if USE_WIN32_THREADS
	: m_sema(INVALID_HANDLE_VALUE)
#elif USE_STD_THREADS
	: m_flag(false)
#endif
{
}

//------------------------------------------------------------------------------
// ~Semaphore
//------------------------------------------------------------------------------
Semaphore::~Semaphore()
{
#if USE_WIN32_THREADS
	if (m_sema != INVALID_HANDLE_VALUE)
	{
		BOOL val = CloseHandle(m_sema);
		ASSERT_TRUE(val != 0);
		m_sema = INVALID_HANDLE_VALUE;
	}
#endif
}

//------------------------------------------------------------------------------
// Create
//------------------------------------------------------------------------------
void Semaphore::Create()
{
#if USE_WIN32_THREADS
	if (m_sema == INVALID_HANDLE_VALUE)
	{
		m_sema = CreateEvent(NULL, TRUE, FALSE, TEXT("Semahore"));
		ASSERT_TRUE(m_sema != NULL);
	}
#endif
}

//------------------------------------------------------------------------------
// Reset
//------------------------------------------------------------------------------
void Semaphore::Reset()
{
#if USE_WIN32_THREADS
	BOOL val = ResetEvent(m_sema);
	ASSERT_TRUE(val != 0);
#endif
}

//------------------------------------------------------------------------------
// Wait
//------------------------------------------------------------------------------
bool Semaphore::Wait(int timeout)
{
#if USE_WIN32_THREADS
	ASSERT_TRUE(m_sema != INVALID_HANDLE_VALUE);
	if (timeout < 0)
		timeout = INFINITE;

	DWORD err = WaitForSingleObject(m_sema, timeout); 
	ASSERT_TRUE(err == WAIT_OBJECT_0 || err == WAIT_TIMEOUT);
	if (err == WAIT_OBJECT_0)
		return true;
	else
		return false;
#elif USE_STD_THREADS
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
#else
	return true;
#endif
}

//------------------------------------------------------------------------------
// Signal
//------------------------------------------------------------------------------
void Semaphore::Signal()
{
#if USE_WIN32_THREADS
	ASSERT_TRUE(m_sema != INVALID_HANDLE_VALUE);
	BOOL val = SetEvent(m_sema);
	ASSERT_TRUE(val != 0);
#elif USE_STD_THREADS
	std::unique_lock<std::mutex> lk(m_lock);
	m_flag = true;
	m_sema.notify_one();
#endif
}

}
