#ifndef _DELEGATE_INVOKER_H
#define _DELEGATE_INVOKER_H

#include <memory>

namespace DelegateLib {

class DelegateMsg;

/// @brief Abstract base class to support asynchronous delegate function invoke
/// on destination thread of control. 
/// 
/// @detail Inherit form this class and implement DelegateInvoke(). The implementation
/// typically posts a message into the destination thread message queue. The destination
/// thread receives the message and invokes the target bound function. Destintation 
/// thread invoke example: 
/// 
/// // Get pointer to DelegateMsgBase data from queue msg data
/// auto delegateMsg = msg->GetData();
///
/// // Invoke the delegate destination target function
/// delegateMsg->GetDelegateInvoker()->DelegateInvoke(delegateMsg);
class IDelegateInvoker
{
public:
	/// Called to invoke the callback by the destination thread of control. 
	/// @param[in] msg - the incoming delegate message. 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsg> msg) = 0;
};

}

#endif