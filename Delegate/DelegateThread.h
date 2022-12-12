#ifndef _DELEGATE_THREAD_H
#define _DELEGATE_THREAD_H

#include "DelegateMsg.h"

namespace DelegateLib {

/// @brief Each platform specific implementation must inherit from DelegateThread
/// and provide an implementation for DispatchDelegate().
class DelegateThread
{
public:
	/// Destructor
	virtual ~DelegateThread() = default;

	/// Dispatch a DelegateMsg onto this thread. The implementer is responsible
	/// for getting the DelegateMsg into an OS message queue. Once DelegateMsg
	/// is on the correct thread of control, the DelegateInvoker::DelegateInvoke() function
	/// must be called to execute the callback. 
	/// @param[in] msg - a pointer to the callback message that must be created dynamically.
	/// @pre Caller *must* create the DelegateMsg argument dynamically.
	/// @post The destination thread calls DelegateInvoke().
	virtual void DispatchDelegate(std::shared_ptr<DelegateMsgBase> msg) = 0;
};

}

#endif
