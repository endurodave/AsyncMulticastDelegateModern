#include "DelegateLib.h"
#include "SysData.h"
#include "SysDataNoLock.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <utility>
#include "WorkerThreadStd.h"

// TODO 
// Explain template meta programming in the article? 
// https://en.wikibooks.org/wiki/C%2B%2B_Programming/Templates/Template_Meta-Programming
// - Fix Win32 threads to use smart pointers
// - Replace asserts with exceptions? noexcept keyword?
// - Try creating common errors. Are the compiler error comprehesible? 
// - All modern C++ pointers for class members? shared, unqiue_ptr, etc... 
// - Modern delegate vs. old speed test
// - std::mem_fn in member versions of delegates? A free function one too? std::bind better?
// - std::function instead of raw function pointers in delegate classes
//   std::function<void (Ts...)> f;
// - use std::mem_fn ? 
// - use std::invoke in operator() functions?
// - Remove Win32 threads? Just C++ threads?
//   have template argument to use a different allocator (just like std::list, ...)
// - Check memory leaks
// - Fix all file header comments to use correct date and article link
// - Compile with GCC

// main.cpp
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Dec 2016.
//
// @see https://www.codeproject.com/Articles/5262271/Remote-Procedure-Calls-using-Cplusplus-Delegates
// David Lafreniere, Mar 2020.

#if 0 // Not reliable
#if WIN32
struct DumpLeaks
{
    ~DumpLeaks() {
        BOOL leaks = _CrtDumpMemoryLeaks();
        ASSERT_TRUE(leaks == FALSE);
    }
};
static DumpLeaks dumpLeaks;
#endif
#endif

using namespace std;
using namespace DelegateLib;

WorkerThread workerThread1("WorkerThread1");

/// @brief Test client to get callbacks from SysData::SystemModeChangedDelgate and 
/// SysDataNoLock::SystemModeChangedDelegate
class SysDataClient
{
public:
	// Constructor
	SysDataClient() :
		m_numberOfCallbacks(0)
	{
		// Register for async delegate callbacks
		SysData::GetInstance().SystemModeChangedDelegate += MakeDelegate(this, &SysDataClient::CallbackFunction, &workerThread1);
		SysDataNoLock::GetInstance().SystemModeChangedDelegate += MakeDelegate(this, &SysDataClient::CallbackFunction, &workerThread1);
	}

	~SysDataClient()
	{
		// Unregister the all registered delegates at once
		SysData::GetInstance().SystemModeChangedDelegate.Clear(); 

		// Alternatively unregister a single delegate
		SysDataNoLock::GetInstance().SystemModeChangedDelegate -= MakeDelegate(this, &SysDataClient::CallbackFunction, &workerThread1);
	}

private:
    void CallbackFunction(const SystemModeChanged& data)
	{
		m_numberOfCallbacks++;
		cout << "CallbackFunction " << data.CurrentSystemMode << endl;
	}

	int m_numberOfCallbacks;
};

struct TestStruct
{
	int x;
    ~TestStruct() {}
};

struct TestStructNoCopy
{
	TestStructNoCopy(int _x) { x = _x;  }
	int x;

private:
	// Prevent copying objects
	TestStructNoCopy(const TestStructNoCopy&) = delete;
	TestStructNoCopy& operator=(const TestStructNoCopy&) = delete;
};

void FreeFunc()
{
	cout << "FreeFunc" << endl;
}

void FreeFuncInt(int value)
{
	cout << "FreeFuncInt " << value << endl;
}

int FreeFuncRetInt()
{
	cout << "FreeFuncRetInt " << endl;
	return 567;
}

int FreeFuncIntRetInt(int value)
{
	cout << "FreeFuncIntRetInt " << value << endl;
	return value;
}

void FreeFuncPtrTestStruct(TestStruct* value)
{
	cout << "FreeFuncTestStruct" << value->x << endl;
}

void FreeFuncPtrPtrTestStruct(TestStruct** value)
{
	cout << "FreeFuncPtrPtrTestStruct " << (*value)->x << endl;
}

void FreeFuncRefTestStruct(const TestStruct& value) 
{
	cout << "FreeFuncRefTestStruct " << value.x << endl;
}

class TestClass
{
public:
	~TestClass()
	{
	}

	void MemberFunc(TestStruct* value)
	{
		cout << "MemberFunc " << value->x << endl;
	}

    void MemberFuncThreeArgs(const TestStruct& value, float f, int** i)
	{
		cout << "MemberFuncThreeArgs " << value.x << " " << f << " " << (**i) << endl;
	}

	void MemberFuncNoCopy(TestStructNoCopy* value)
	{
		cout << "MemberFuncNoCopy " << value->x << endl;
	}

    void MemberFuncStdString(const std::string& s, int year) 
	{
		cout << "MemberFuncStdString " << s.c_str() << " " << year << endl;
	}

	int MemberFuncStdStringRetInt(std::string& s)
	{
		s = "Hello world";
		return 2020;
	}

	static void StaticFunc(TestStruct* value)
	{
		cout << "StaticFunc " << value->x << endl;
	}

	int TestFunc()
	{
		cout << "TestFunc " << endl;
		return 987;
	}
	void TestFuncNoRet()
	{
		cout << "TestFuncNoRet " << endl;
	}
};

// An instance of TestStructNoCopy guaranteed to exist when the asynchronous callback occurs.  
static TestStructNoCopy testStructNoCopy(999);

extern void DelegateUnitTests();

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(void)
{
	TestStruct testStruct;
	testStruct.x = 123;
	TestStruct* pTestStruct = &testStruct;

	TestClass testClass;

	// Create the worker threads
	workerThread1.CreateThread();
	SysDataNoLock::GetInstance();

#if USE_WIN32_THREADS
	// Start the worker threads
	ThreadWin::StartAllThreads();
#endif

	// Run all unit tests (uncomment to run unit tests)
	DelegateUnitTests();

#ifdef WIN32
    LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds, TotalElapsedMicroseconds = { 0 };
    LARGE_INTEGER Frequency;

    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartingTime);
#endif

    // Create a delegate bound to a free function then invoke
    DelegateFree<void(int)> delegateFree = MakeDelegate(&FreeFuncInt);
    delegateFree(123);

    // Create a delegate bound to a member function then invoke
    DelegateMember<TestClass, void(TestStruct*)> delegateMember = MakeDelegate(&testClass, &TestClass::MemberFunc);
    delegateMember(&testStruct);

    // Create a delegate bound to a member function. Assign and invoke from
    // a base reference. 
    DelegateMember<TestClass, void(TestStruct*)> delegateMember2 = MakeDelegate(&testClass, &TestClass::MemberFunc);
    delegateMember2(&testStruct);

    // Create a multicast delegate container that accepts Delegate1<int> delegates.
    // Any function with the signature "void Func(int)".
    MulticastDelegate<void(int)> delegateA;

    // Add a DelegateFree1<int> delegate to the container 
    delegateA += MakeDelegate(&FreeFuncInt);

    // Invoke the delegate target free function FreeFuncInt()
    if (delegateA)
        delegateA(123);

    // Remove the delegate from the container
    delegateA -= MakeDelegate(&FreeFuncInt);

    // Create a multicast delegate container that accepts Delegate<TestStruct*> delegates
    // Any function with the signature "void Func(TestStruct*)".
    MulticastDelegate<void(TestStruct*)> delegateB;

    // Add a DelegateMember1<TestStruct*> delegate to the container
    delegateB += MakeDelegate(&testClass, &TestClass::MemberFunc);

    // Invoke the delegate target member function TestClass::MemberFunc()
    if (delegateB)
        delegateB(&testStruct);

    // Remove the delegate from the container
    delegateB -= MakeDelegate(&testClass, &TestClass::MemberFunc);

    // Create a thread-safe multicast delegate container that accepts Delegate<TestStruct*> delegates
    // Any function with the signature "void Func(TestStruct*)".
    MulticastDelegateSafe<void(TestStruct*)> delegateC;

    // Add a DelegateMember1<TestStruct*> delegate to the container that will invoke on workerThread1
    delegateC += MakeDelegate(&testClass, &TestClass::MemberFunc, &workerThread1);

    // Asynchronously invoke the delegate target member function TestClass::MemberFunc()
    if (delegateC)
        delegateC(&testStruct);

    // Remove the delegate from the container
    delegateC -= MakeDelegate(&testClass, &TestClass::MemberFunc, &workerThread1);

    // Create a thread-safe multicast delegate container that accepts Delegate<TestStruct&, float, int**> delegates
    // Any function with the signature "void Func(const TestStruct&, float, int**)".
    MulticastDelegateSafe<void(const TestStruct&, float, int**)> delegateD;

    // Add a DelegateMember1<TestStruct*> delegate to the container that will invoke on workerThread1
    delegateD += MakeDelegate(&testClass, &TestClass::MemberFuncThreeArgs, &workerThread1);

    // Asynchronously invoke the delegate target member function TestClass::MemberFuncThreeArgs()
    if (delegateD)
    {
        int i = 555;
        int* pI = &i;
        delegateD(testStruct, 1.23f, &pI);
    }

    // Remove the delegate from the container
    delegateD -= MakeDelegate(&testClass, &TestClass::MemberFuncThreeArgs, &workerThread1);

    // Create a singlecast delegate container that accepts Delegate1<int, int> delegates.
    // Any function with the signature "int Func(int)".
    SinglecastDelegate<int(int)> delegateF;

    // Add a DelegateFree1<int, int> delegate to the container 
    delegateF = MakeDelegate(&FreeFuncIntRetInt);

    // Invoke the delegate target free function FreeFuncInt()
    int retVal = 0;
    if (delegateF)
        retVal = delegateF(123);

    // Remove the delegate from the container
    delegateF.Clear();

    // Create a singlecast delegate container that accepts delegates with 
    // the singature "void Func(TestStruct**)"
    SinglecastDelegate<void(TestStruct**)> delegateG;

    // Make a delegate that points to a free function 
    delegateG = MakeDelegate(&FreeFuncPtrPtrTestStruct);

    // Invoke the delegate target function FreeFuncPtrPtrTestStruct()
    delegateG(&pTestStruct);

    // Remove the delegate from the container
    delegateG = 0;

    // Create delegate with std::string and int arguments then asychronously 
    // invoke on a member function
    MulticastDelegateSafe<void(const std::string&, int)> delegateH;
    delegateH += MakeDelegate(&testClass, &TestClass::MemberFuncStdString, &workerThread1);
    delegateH("Hello world", 2020);
    delegateH.Clear();

    // Create a asynchronous blocking delegate and invoke. This thread will block until the 
    // msg and year stack values are set by MemberFuncStdStringRetInt on workerThread1.
    auto delegateI = MakeDelegate(&testClass, &TestClass::MemberFuncStdStringRetInt, &workerThread1, WAIT_INFINITE);
    std::string msg;
    int year = delegateI(msg);
    if (delegateI.IsSuccess())
    {
        cout << msg.c_str() << " " << year << endl;
    }

    // Create a shared_ptr, create a delegate, then synchronously invoke delegate function
    std::shared_ptr<TestClass> spObject(new TestClass());
    auto delegateMemberSp = MakeDelegate(spObject, &TestClass::MemberFuncStdString);
    delegateMemberSp("Hello world using shared_ptr", 2020);

    // Example of a bug where the testClassHeap is deleted before the asychronous delegate 
    // is invoked on the workerThread1. In other words, by the time workerThread1 calls
    // the bound delegate function the testClassHeap instance is deleted and no longer valid.
    TestClass* testClassHeap = new TestClass();
    auto delegateMemberAsync = MakeDelegate(testClassHeap, &TestClass::MemberFuncStdString, &workerThread1);
    delegateMemberAsync("Function async invoked on deleted object. Bug!", 2020);
    delegateMemberAsync.Clear();
    delete testClassHeap;

    // Example of the smart pointer function version of the delegate. The testClassSp instance 
    // is only deleted after workerThread1 invokes the callback function thus solving the bug.
    std::shared_ptr<TestClass> testClassSp(new TestClass());
    auto delegateMemberSpAsync = MakeDelegate(testClassSp, &TestClass::MemberFuncStdString, &workerThread1);
    delegateMemberSpAsync("Function async invoked using smart pointer. Bug solved!", 2020);
    delegateMemberSpAsync.Clear();
    testClassSp.reset();

    // Create a SysDataClient instance on the stack
    SysDataClient sysDataClient;

    // Set new SystemMode values. Each call will invoke callbacks to all 
    // registered client subscribers.
    SysData::GetInstance().SetSystemMode(SystemMode::STARTING);
    SysData::GetInstance().SetSystemMode(SystemMode::NORMAL);

    // Set new SystemMode values for SysDataNoLock.
    SysDataNoLock::GetInstance().SetSystemMode(SystemMode::SERVICE);
    SysDataNoLock::GetInstance().SetSystemMode(SystemMode::SYS_INOP);

    // Set new SystemMode values for SysDataNoLock using async API
    SysDataNoLock::GetInstance().SetSystemModeAsyncAPI(SystemMode::SERVICE);
    SysDataNoLock::GetInstance().SetSystemModeAsyncAPI(SystemMode::SYS_INOP);

    // Set new SystemMode values for SysDataNoLock using async wait API
    SystemMode::Type previousMode;
    previousMode = SysDataNoLock::GetInstance().SetSystemModeAsyncWaitAPI(SystemMode::STARTING);
    previousMode = SysDataNoLock::GetInstance().SetSystemModeAsyncWaitAPI(SystemMode::NORMAL);

#ifdef WIN32
    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
    std::cout << "Elapsed Time: " << (float)ElapsedMicroseconds.QuadPart / 1000000.0f << " seconds" << std::endl;
#endif

   	workerThread1.ExitThread();

    std::this_thread::sleep_for(std::chrono::seconds(1));

	return 0;
}

