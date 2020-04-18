#ifndef _DELEGATE_INVOKER_H
#define _DELEGATE_INVOKER_H

namespace DelegateLib {

class DelegateMsgBase;

class IDelegateInvoker
{
public:
	/// Called to invoke the callback by the destination thread of control. 
	/// @param[in] msg - the incoming delegate message. 
	virtual void DelegateInvoke(DelegateMsgBase** msg) = 0;
};

}

#endif