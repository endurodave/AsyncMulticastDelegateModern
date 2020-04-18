#ifndef _LOCK_GUARD_H
#define _LOCK_GUARD_H

#include "DelegateOpt.h"
#include "DataTypes.h"

#if USE_WIN32_THREADS
	// On WIN32 API, the lock is a critical section
	#define LOCK CRITICAL_SECTION
#elif USE_STD_THREADS
	#include <mutex>
	// On std API, the lock is a mutex
	#define LOCK std::mutex
#else
	#error Must implement the LockGuard class on non-Windows platforms
	#define LOCK int
#endif

namespace DelegateLib {

/// @brief A software lock guard that implements RAII (Resourse Acquisition Is Initialization)
/// to control and manage the lock.
class LockGuard
{
public:
	/// Capture a software lock upon construction of LockGuard. 
	/// @param[in] lock - a software lock.
	/// @pre Call Create() to create the lock before using LockGuard constructor. 
	LockGuard(LOCK* lock);

	/// Release a software lock upon destruction of LockGuard. 
	~LockGuard();

	/// Called to create the software lock.
	/// @param[in] lock - a software lock.
	static void Create(LOCK* lock);

	/// Called to destroy the software lock.
	/// @param[in] lock - a software lock.
	static void Destroy(LOCK* lock);

private:
	// Prevent copying objects
	LockGuard(const LockGuard&) = delete;
	LockGuard& operator=(const LockGuard&) = delete;

	LOCK* m_lock;
};

class LockCreateDestroy
{
public:
    LockCreateDestroy(LOCK& lock) : m_lock(lock) { LockGuard::Create(&m_lock); }
    ~LockCreateDestroy() { LockGuard::Destroy(&m_lock); }
private:
    LOCK & m_lock;
};

}

#endif 
