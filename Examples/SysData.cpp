#include "SysData.h"

//----------------------------------------------------------------------------
// GetInstance
//----------------------------------------------------------------------------
SysData& SysData::GetInstance()
{
	static SysData instance;
	return instance;
}

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
SysData::SysData() :
	m_systemMode(SystemMode::STARTING)
{
	LockGuard::Create(&m_lock);
}

//----------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------
SysData::~SysData()
{
	LockGuard::Destroy(&m_lock);
}

//----------------------------------------------------------------------------
// SetSystemMode
//----------------------------------------------------------------------------
void SysData::SetSystemMode(SystemMode::Type systemMode)
{
	LockGuard lockGuard(&m_lock);

	// Create the callback data
	SystemModeChanged callbackData;
	callbackData.PreviousSystemMode = m_systemMode;
	callbackData.CurrentSystemMode = systemMode;

	// Update the system mode
	m_systemMode = systemMode;

	// Callback all registered subscribers
	if (SystemModeChangedDelegate)
		SystemModeChangedDelegate(callbackData);
}