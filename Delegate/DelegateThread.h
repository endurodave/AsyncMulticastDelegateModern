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
	virtual ~DelegateThread() {}

	/// Dispatch a DelegateMsg onto this thread. The implementer is responsible
	/// for getting the DelegateMsg into an OS message queue. Once DelegateMsg
	/// is on the correct thread of control, the DelegateInvoker::DelegateInvoke() function
	/// must be called to execute the callback. 
	/// @param[in] msg - a pointer to the callback message that must be created dynamically
	///		using operator new. 
	/// @pre Caller *must* create the DelegateMsg argument dynamically using operator new.
	/// @post The destination thread must delete the msg instance by calling DelegateInvoke().
	virtual void DispatchDelegate(DelegateMsgBase* msg) = 0;
};

}

#endif
