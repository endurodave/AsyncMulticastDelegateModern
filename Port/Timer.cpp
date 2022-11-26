#include "Timer.h"
#include "Fault.h"
#include <chrono>

using namespace std;

LOCK Timer::m_lock;
bool Timer::m_lockInit = false;
bool Timer::m_timerStopped = false;
list<Timer*> Timer::m_timers;

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
	if (m_lockInit == false)
	{
		LockGuard::Create(&m_lock);
		m_lockInit = true;
	}

	LockGuard lockGuard(&m_lock);
	m_enabled = false;
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
void Timer::Start(unsigned long timeout)
{
	LockGuard lockGuard(&m_lock);

	m_timeout = timeout;
    ASSERT_TRUE(m_timeout != 0);
	m_expireTime = GetTime();
	m_enabled = true;

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

	m_enabled = false;
	m_timerStopped = true;
}

//------------------------------------------------------------------------------
// CheckExpired
//------------------------------------------------------------------------------
void Timer::CheckExpired()
{
	if (!m_enabled)
		return;

	// Has the timer expired?
    if (Difference(m_expireTime, GetTime()) < m_timeout)
        return;

    // Increment the timer to the next expiration
	m_expireTime += m_timeout;

	// Is the timer already expired after we incremented above?
    if (Difference(m_expireTime, GetTime()) > m_timeout)
	{
		// The timer has fallen behind so set time expiration further forward.
		m_expireTime = GetTime();
	}

	// Call the client's expired callback function
	if (Expired)
		Expired();
}

//------------------------------------------------------------------------------
// Difference
//------------------------------------------------------------------------------
unsigned long Timer::Difference(unsigned long time1, unsigned long time2)
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
		m_timerStopped = false;
	}

	// Iterate through each timer and check for expirations
	TimersIterator it;
	for (it = m_timers.begin() ; it != m_timers.end(); it++ )
	{
		if ((*it) != NULL)
			(*it)->CheckExpired();
	}
}

unsigned long Timer::GetTime()
{
    unsigned long milliseconds_since_epoch =
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    return (unsigned long)milliseconds_since_epoch;
}

