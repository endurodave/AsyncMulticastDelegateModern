#include "stdafx.h"
#include "Timer.h"
#include "Fault.h"

using namespace std;

LOCK Timer::m_lock;
BOOL Timer::m_lockInit = FALSE;
BOOL Timer::m_timerStopped = FALSE;
list<Timer*> Timer::m_timers;
const DWORD Timer::MS_PER_TICK = (1000 / CLOCKS_PER_SEC);

//------------------------------------------------------------------------------
// TimerDisabled
//------------------------------------------------------------------------------
static bool TimerDisabled (Timer* value)
{
	return !(value->Enabled());
}

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Timer::Timer() 
{
	// Create the thread mutex
	if (m_lockInit == FALSE)
	{
		LockGuard::Create(&m_lock);
		m_lockInit = TRUE;
	}

	LockGuard lockGuard(&m_lock);
	m_enabled = FALSE;
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
Timer::~Timer()
{
	LockGuard lockGuard(&m_lock);
	m_timers.remove(this);
}

//------------------------------------------------------------------------------
// Start
//------------------------------------------------------------------------------
void Timer::Start(DWORD timeout)
{
	LockGuard lockGuard(&m_lock);

	m_timeout = timeout / MS_PER_TICK;
    ASSERT_TRUE(m_timeout != 0);
	m_expireTime = GetTickCount();
	m_enabled = TRUE;

	// Remove the existing entry, if any, to prevent duplicates in the list
	m_timers.remove(this);

	// Add this timer to the list for servicing
	m_timers.push_back(this);
}

//------------------------------------------------------------------------------
// Stop
//------------------------------------------------------------------------------
void Timer::Stop()
{
	LockGuard lockGuard(&m_lock);

	m_enabled = FALSE;
	m_timerStopped = TRUE;
}

//------------------------------------------------------------------------------
// CheckExpired
//------------------------------------------------------------------------------
void Timer::CheckExpired()
{
	if (!m_enabled)
		return;

	// Has the timer expired?
    if (Difference(m_expireTime, GetTickCount()) < m_timeout)
        return;

    // Increment the timer to the next expiration
	m_expireTime += m_timeout;

	// Is the timer already expired after we incremented above?
    if (Difference(m_expireTime, GetTickCount()) > m_timeout)
	{
		// The timer has fallen behind so set time expiration further forward.
		m_expireTime = GetTickCount();
	}

	// Call the client's expired callback function
	if (Expired)
		Expired();
}

//------------------------------------------------------------------------------
// Difference
//------------------------------------------------------------------------------
DWORD Timer::Difference(DWORD time1, DWORD time2)
{
	return (time2 - time1);
}

//------------------------------------------------------------------------------
// ProcessTimers
//------------------------------------------------------------------------------
void Timer::ProcessTimers()
{
	LockGuard lockGuard(&m_lock);

	// Remove disabled timer from the list if stopped
	if (m_timerStopped)
	{
		m_timers.remove_if(TimerDisabled);
		m_timerStopped = FALSE;
	}

	// Iterate through each timer and check for expirations
	TimersIterator it;
	for (it = m_timers.begin() ; it != m_timers.end(); it++ )
	{
		if ((*it) != NULL)
			(*it)->CheckExpired();
	}
}

