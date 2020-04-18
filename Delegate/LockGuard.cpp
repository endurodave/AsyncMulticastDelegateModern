#include "LockGuard.h"
#include "Fault.h"

namespace DelegateLib {

//------------------------------------------------------------------------------
// LockGuard
//------------------------------------------------------------------------------
LockGuard::LockGuard(LOCK* lock)
{
#if USE_WIN32_THREADS
	m_lock = lock;
	EnterCriticalSection(m_lock);
#elif USE_STD_THREADS
	m_lock = lock;
	m_lock->lock();
#endif
}

//------------------------------------------------------------------------------
// ~MutexLockGuard
//------------------------------------------------------------------------------
LockGuard::~LockGuard()
{
#if USE_WIN32_THREADS
	LeaveCriticalSection(m_lock);
#elif USE_STD_THREADS
	m_lock->unlock();
#endif
}

//------------------------------------------------------------------------------
// Create
//------------------------------------------------------------------------------
void LockGuard::Create(LOCK* lock)
{
#if USE_WIN32_THREADS
	BOOL lockSuccess = InitializeCriticalSectionAndSpinCount(lock, 0x00000400); 
	ASSERT_TRUE(lockSuccess != 0);
#endif
}

//------------------------------------------------------------------------------
// Destroy
//------------------------------------------------------------------------------
void LockGuard::Destroy(LOCK* lock)
{
#if USE_WIN32_THREADS
	DeleteCriticalSection(lock);
#endif
}

}
