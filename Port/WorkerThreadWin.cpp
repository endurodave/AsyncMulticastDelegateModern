#include "DelegateOpt.h"
#if USE_WIN32_THREADS

#include "WorkerThreadWin.h"
#include "ThreadMsg.h"
#include "UserMsgs.h"

using namespace DelegateLib;

//----------------------------------------------------------------------------
// WorkerThread
//----------------------------------------------------------------------------
WorkerThread::WorkerThread(const CHAR* threadName) : ThreadWin(threadName)
{
}

//----------------------------------------------------------------------------
// Process
//----------------------------------------------------------------------------
unsigned long WorkerThread::Process(void* parameter)
{
	MSG msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, WM_USER_BEGIN, WM_USER_END)) != 0)
	{
		switch (msg.message)
		{
			case WM_DISPATCH_DELEGATE:
			{
				ASSERT_TRUE(msg.wParam != NULL);

				// Get the ThreadMsg from the wParam value
				ThreadMsg* threadMsg = reinterpret_cast<ThreadMsg*>(msg.wParam);

				// Convert the ThreadMsg void* data back to a DelegateMsg* 
				DelegateMsgBase* delegateMsg = static_cast<DelegateMsgBase*>(threadMsg->GetData()); 

				// Invoke the callback on the target thread
				delegateMsg->GetDelegateInvoker()->DelegateInvoke(&delegateMsg);

				// Delete dynamic data passed through message queue
				delete threadMsg;
				break;
			}

			case WM_EXIT_THREAD:
				return 0;

			default:
				ASSERT();
		}
	}
	return 0;
}

#endif