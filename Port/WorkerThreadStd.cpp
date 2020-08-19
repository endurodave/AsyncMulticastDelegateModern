#include "DelegateOpt.h"
#include "WorkerThreadStd.h"
#include "ThreadMsg.h"

using namespace std;
using namespace DelegateLib;

#define MSG_DISPATCH_DELEGATE	1
#define MSG_EXIT_THREAD			2

//----------------------------------------------------------------------------
// WorkerThread
//----------------------------------------------------------------------------
WorkerThread::WorkerThread(const CHAR* threadName) : m_thread(nullptr), THREAD_NAME(threadName)
{
}

//----------------------------------------------------------------------------
// ~WorkerThread
//----------------------------------------------------------------------------
WorkerThread::~WorkerThread()
{
	ExitThread();
}

//----------------------------------------------------------------------------
// CreateThread
//----------------------------------------------------------------------------
BOOL WorkerThread::CreateThread()
{
	if (!m_thread)
		m_thread = std::unique_ptr<std::thread>(new thread(&WorkerThread::Process, this));
	return TRUE;
}

//----------------------------------------------------------------------------
// GetThreadId
//----------------------------------------------------------------------------
std::thread::id WorkerThread::GetThreadId()
{
	ASSERT_TRUE(m_thread != nullptr);
	return m_thread->get_id();
}

//----------------------------------------------------------------------------
// GetCurrentThreadId
//----------------------------------------------------------------------------
std::thread::id WorkerThread::GetCurrentThreadId()
{
	return this_thread::get_id();
}

//----------------------------------------------------------------------------
// ExitThread
//----------------------------------------------------------------------------
void WorkerThread::ExitThread()
{
	if (!m_thread)
		return;

	// Create a new ThreadMsg
	std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(MSG_EXIT_THREAD, 0));

	// Put exit thread message into the queue
	{
		lock_guard<mutex> lock(m_mutex);
		m_queue.push(threadMsg);
		m_cv.notify_one();
	}

    m_thread->join();
    m_thread = nullptr;
}

//----------------------------------------------------------------------------
// DispatchDelegate
//----------------------------------------------------------------------------
void WorkerThread::DispatchDelegate(std::shared_ptr<DelegateLib::DelegateMsgBase> msg)
{
	ASSERT_TRUE(m_thread);

	// Create a new ThreadMsg
    std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(MSG_DISPATCH_DELEGATE, msg));

	// Add dispatch delegate msg to queue and notify worker thread
	std::unique_lock<std::mutex> lk(m_mutex);
	m_queue.push(threadMsg);
	m_cv.notify_one();
}

//----------------------------------------------------------------------------
// Process
//----------------------------------------------------------------------------
void WorkerThread::Process()
{
	while (1)
	{
		std::shared_ptr<ThreadMsg> msg;
		{
			// Wait for a message to be added to the queue
			std::unique_lock<std::mutex> lk(m_mutex);
			while (m_queue.empty())
				m_cv.wait(lk);

			if (m_queue.empty())
				continue;

			msg = m_queue.front();
			m_queue.pop();
		}

		switch (msg->GetId())
		{
			case MSG_DISPATCH_DELEGATE:
			{
				ASSERT_TRUE(msg->GetData() != NULL);

				// Convert the ThreadMsg void* data back to a DelegateMsg* 
                auto delegateMsg = msg->GetData();

				// Invoke the callback on the target thread
				delegateMsg->GetDelegateInvoker()->DelegateInvoke(delegateMsg);
				break;
			}

			case MSG_EXIT_THREAD:
			{
				return;
			}

			default:
				ASSERT();
		}
	}
}

