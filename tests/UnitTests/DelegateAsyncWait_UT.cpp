#include "DelegateLib.h"
#include "UnitTestCommon.h"
#include <iostream>
#include <set>
#include "WorkerThreadStd.h"

using namespace DelegateLib;
using namespace std;
using namespace UnitTestData;

static WorkerThread workerThread("DelegateAsyncWait_UT");

namespace AsyncWait
{
    struct TestReturn
    {
        ~TestReturn()
        {
            val++;
        }
        static int val;
    };

    int TestReturn::val = 0;

    class TestReturnClass
    {
    public:
        TestReturn Func() { return TestReturn{}; }
    };
}
using namespace AsyncWait;

static void DelegateFreeAsyncWaitTests()
{
    using Del = DelegateFreeAsyncWait<void(int)>;

    Del delegate1(FreeFuncInt1, workerThread);
    delegate1(TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());
    std::invoke(delegate1, TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    Del delegate3(FreeFuncInt1, workerThread);
    delegate3 = delegate1;
    ASSERT_TRUE(delegate3 == delegate1);
    ASSERT_TRUE(delegate3);

    delegate3.Clear();
    ASSERT_TRUE(delegate3.Empty());
    ASSERT_TRUE(!delegate3);

    auto* delegate4 = delegate1.Clone();
    ASSERT_TRUE(*delegate4 == delegate1);

    auto delegate5 = std::move(delegate1);
    ASSERT_TRUE(!delegate5.Empty());
    ASSERT_TRUE(delegate1.Empty());

    Del delegate6(FreeFuncInt1, workerThread);
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());
    ASSERT_TRUE(delegate6 != nullptr);
    ASSERT_TRUE(nullptr != delegate6);
    ASSERT_TRUE(delegate2 == nullptr);
    ASSERT_TRUE(nullptr == delegate2);

    DelegateFunction<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));

    Del delegate7(FreeFuncInt1, workerThread, std::chrono::milliseconds(0));
    auto success = delegate7.AsyncInvoke(TEST_INT);
    //ASSERT_TRUE(!success.has_value());

    DelegateFreeAsyncWait<std::uint16_t(void)> d;
    ASSERT_TRUE(!d);
    auto r = d();
    using ArgT = decltype(r);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::max() == 0xffff);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_signed == false);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_exact == true);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_integer == true);
    ASSERT_TRUE(r == 0);

    // Make sure we get a default constructed return value.
    TestReturn::val = 0;
    DelegateFreeAsyncWait<TestReturn()> testRet;
    ASSERT_TRUE(TestReturn::val == 0);
    testRet();
    ASSERT_TRUE(TestReturn::val == 1);

#if 0  // AsyncWait cannot return a unique_ptr
    DelegateFreeAsyncWait<std::unique_ptr<int>(int)> delUnique;
    auto tmp = delUnique(10);
    ASSERT_TRUE(tmp == nullptr);
    delUnique.Bind(&FuncUnique, workerThread);
    std::unique_ptr<int> up = delUnique(12);
    ASSERT_TRUE(*up == 12);
#endif

    auto delS1 = MakeDelegate(FreeFuncInt1, workerThread, WAIT_INFINITE);
    auto delS2 = MakeDelegate(FreeFuncInt1_2, workerThread, WAIT_INFINITE);
    ASSERT_TRUE(!(delS1 == delS2));

    std::set<Del> setDel;
    setDel.insert(delS1);
    setDel.insert(delS2);
    ASSERT_TRUE(setDel.size() == 2);
}

static void DelegateMemberAsyncWaitTests()
{
    using Del = DelegateMemberAsyncWait<TestClass1, void(int)>;

    TestClass1 testClass1;

    Del delegate1(&testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate1(TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());
    std::invoke(delegate1, TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    Del delegate3(&testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate3 = delegate1;
    ASSERT_TRUE(delegate3 == delegate1);
    ASSERT_TRUE(delegate3);

    delegate3.Clear();
    ASSERT_TRUE(delegate3.Empty());
    ASSERT_TRUE(!delegate3);

    auto* delegate4 = delegate1.Clone();
    ASSERT_TRUE(*delegate4 == delegate1);

    auto delegate5 = std::move(delegate1);
    ASSERT_TRUE(!delegate5.Empty());
    ASSERT_TRUE(delegate1.Empty());

    Del delegate6(&testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());
    ASSERT_TRUE(delegate6 != nullptr);
    ASSERT_TRUE(nullptr != delegate6);
    ASSERT_TRUE(delegate2 == nullptr);
    ASSERT_TRUE(nullptr == delegate2);

    DelegateFunction<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));

    delegate6 = nullptr;
    ASSERT_TRUE(delegate6.Empty());
    ASSERT_TRUE(delegate6 == nullptr);

    Del delegate7(&testClass1, &TestClass1::MemberFuncInt1, workerThread, std::chrono::milliseconds(0));
    auto success = delegate7.AsyncInvoke(TEST_INT);
    //ASSERT_TRUE(!success.has_value());

    DelegateMemberAsyncWait<Class, std::uint16_t(void)> d;
    ASSERT_TRUE(!d);
    auto r = d();
    using ArgT = decltype(r);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::max() == 0xffff);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_signed == false);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_exact == true);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_integer == true);
    ASSERT_TRUE(r == 0);

    // Check for const correctness
    const Class c;
    DelegateMemberAsyncWait<const Class, std::uint16_t(void)> dConstClass;
    //dConstClass.Bind(&c, &Class::Func, workerThread);     // Not OK. Should fail compile.
    dConstClass.Bind(&c, &Class::FuncConst, workerThread);  // OK
    auto rConst = dConstClass();

    // Make sure we get a default constructed return value.
    TestReturn::val = 0;
    DelegateMemberAsyncWait<TestReturnClass, TestReturn()> testRet;
    ASSERT_TRUE(TestReturn::val == 0);
    testRet();
    ASSERT_TRUE(TestReturn::val == 1);

#if 0  // AsyncWait cannot return a unique_ptr
    Class c2;
    DelegateMemberAsyncWait<Class, std::unique_ptr<int>(int)> delUnique;
    auto tmp = delUnique(10);
    ASSERT_TRUE(tmp == nullptr);
    delUnique.Bind(&c2, &Class::FuncUnique, workerThread);
    std::unique_ptr<int> up = delUnique(12);
    ASSERT_TRUE(*up == 12);
#endif

    auto delS1 = MakeDelegate(&testClass1, &TestClass1::MemberFuncInt1, workerThread, WAIT_INFINITE);
    auto delS2 = MakeDelegate(&testClass1, &TestClass1::MemberFuncInt1_2, workerThread, WAIT_INFINITE);
    ASSERT_TRUE(!(delS1 == delS2));

#if 0  // DelegateMemberAsyncWait can't be inserted into ordered container
    std::set<Del> setDel;
    setDel.insert(delS1);
    setDel.insert(delS2);
    ASSERT_TRUE(setDel.size() == 2);
#endif
}

static void DelegateMemberSpAsyncWaitTests()
{
    using Del = DelegateMemberAsyncWait<TestClass1, void(int)>;

    auto testClass1 = std::make_shared<TestClass1>();

    Del delegate1(testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate1(TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());
    std::invoke(delegate1, TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    Del delegate3(testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate3 = delegate1;
    ASSERT_TRUE(delegate3 == delegate1);
    ASSERT_TRUE(delegate3);

    delegate3.Clear();
    ASSERT_TRUE(delegate3.Empty());
    ASSERT_TRUE(!delegate3);

    auto* delegate4 = delegate1.Clone();
    ASSERT_TRUE(*delegate4 == delegate1);

    auto delegate5 = std::move(delegate1);
    ASSERT_TRUE(!delegate5.Empty());
    ASSERT_TRUE(delegate1.Empty());

    Del delegate6(testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());
    ASSERT_TRUE(delegate6 != nullptr);
    ASSERT_TRUE(nullptr != delegate6);
    ASSERT_TRUE(delegate2 == nullptr);
    ASSERT_TRUE(nullptr == delegate2);

    DelegateFunction<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));

    delegate6 = nullptr;
    ASSERT_TRUE(delegate6.Empty());
    ASSERT_TRUE(delegate6 == nullptr);

    Del delegate7(testClass1, &TestClass1::MemberFuncInt1, workerThread, std::chrono::milliseconds(0));
    auto success = delegate7.AsyncInvoke(TEST_INT);
    //ASSERT_TRUE(!success.has_value());

    DelegateMemberAsyncWait<Class, std::uint16_t(void)> d;
    ASSERT_TRUE(!d);
    auto r = d();
    using ArgT = decltype(r);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::max() == 0xffff);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_signed == false);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_exact == true);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_integer == true);
    ASSERT_TRUE(r == 0);

    // Check for const correctness
    auto c = std::make_shared<const Class>();
    DelegateMemberAsyncWait<const Class, std::uint16_t(void)> dConstClass;
    //dConstClass.Bind(c, &Class::Func, workerThread);     // Not OK. Should fail compile.
    dConstClass.Bind(c, &Class::FuncConst, workerThread);  // OK
    auto rConst = dConstClass();

    // Make sure we get a default constructed return value.
    TestReturn::val = 0;
    DelegateMemberAsyncWait<TestReturnClass, TestReturn()> testRet;
    ASSERT_TRUE(TestReturn::val == 0);
    testRet();
    ASSERT_TRUE(TestReturn::val == 1);

#if 0  // AsyncWait cannot return a unique_ptr
    Class c2;
    DelegateMemberAsyncWait<Class, std::unique_ptr<int>(int)> delUnique;
    auto tmp = delUnique(10);
    ASSERT_TRUE(tmp == nullptr);
    delUnique.Bind(&c2, &Class::FuncUnique, workerThread);
    std::unique_ptr<int> up = delUnique(12);
    ASSERT_TRUE(*up == 12);
#endif

    auto delS1 = MakeDelegate(testClass1, &TestClass1::MemberFuncInt1, workerThread, WAIT_INFINITE);
    auto delS2 = MakeDelegate(testClass1, &TestClass1::MemberFuncInt1_2, workerThread, WAIT_INFINITE);
    ASSERT_TRUE(!(delS1 == delS2));

#if 0  // DelegateMemberAsyncWait can't be inserted into ordered container
    std::set<Del> setDel;
    setDel.insert(delS1);
    setDel.insert(delS2);
    ASSERT_TRUE(setDel.size() == 2);
#endif
}

static void DelegateFunctionAsyncWaitTests()
{
    using Del = DelegateFunctionAsyncWait<void(int)>;

    Del delegate1(LambdaNoCapture, workerThread);
    delegate1(TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());
    std::invoke(delegate1, TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    DelegateFunctionAsyncWait<void(int)> delegate3(LambdaNoCapture, workerThread);
    delegate3 = delegate1;
    ASSERT_TRUE(delegate3 == delegate1);
    ASSERT_TRUE(delegate3);

    delegate3.Clear();
    ASSERT_TRUE(delegate3.Empty());
    ASSERT_TRUE(!delegate3);

    auto* delegate4 = delegate1.Clone();
    ASSERT_TRUE(*delegate4 == delegate1);

    auto delegate5 = std::move(delegate1);
    ASSERT_TRUE(!delegate5.Empty());
    ASSERT_TRUE(delegate1.Empty());

    DelegateFunctionAsyncWait<void(int)> delegate6(LambdaNoCapture, workerThread);
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());
    ASSERT_TRUE(delegate6 != nullptr);
    ASSERT_TRUE(nullptr != delegate6);
    ASSERT_TRUE(delegate2 == nullptr);
    ASSERT_TRUE(nullptr == delegate2);

    DelegateFree<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));

    delegate6 = nullptr;
    ASSERT_TRUE(delegate6.Empty());
    ASSERT_TRUE(delegate6 == nullptr);

    Del delegate7(LambdaNoCapture, workerThread, std::chrono::milliseconds(0));
    auto success = delegate7.AsyncInvoke(TEST_INT);
    //ASSERT_TRUE(!success.has_value());

    auto delegate8{ delegate5 };
    ASSERT_TRUE(delegate8 == delegate5);
    ASSERT_TRUE(!delegate8.Empty());

    auto l = []() -> std::uint16_t { return 0; };
    DelegateFunctionAsyncWait<std::uint16_t(void)> d;
    ASSERT_TRUE(!d);
    auto r = d();
    using ArgT = decltype(r);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::max() == 0xffff);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_signed == false);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_exact == true);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_integer == true);
    ASSERT_TRUE(r == 0);

    // Make sure we get a default constructed return value.
    TestReturn::val = 0;
    DelegateFunctionAsyncWait<TestReturn()> testRet;
    ASSERT_TRUE(TestReturn::val == 0);
    testRet();
    ASSERT_TRUE(TestReturn::val == 1);

#if 0  // AsyncWait cannot return a unique_ptr
    auto c2 = std::make_shared<Class>();
    DelegateMemberAsyncWait<Class, std::unique_ptr<int>(int)> delUnique;
    auto tmp = delUnique(10);
    ASSERT_TRUE(tmp == nullptr);
    delUnique.Bind(c2, &Class::FuncUnique, workerThread);
    std::unique_ptr<int> up = delUnique(12);
    ASSERT_TRUE(*up == 12);
#endif

    auto delS1 = MakeDelegate(LambdaNoCapture, workerThread, WAIT_INFINITE);
    auto delS2 = MakeDelegate(LambdaNoCapture2, workerThread, WAIT_INFINITE);
    //ASSERT_TRUE(!(delS1 == delS2));  // std::function can't distriguish difference

    std::set<Del> setDel;
    setDel.insert(delS1);
    setDel.insert(delS2);
    ASSERT_TRUE(setDel.size() == 1);
}

void DelegateAsyncWait_UT()
{
    workerThread.CreateThread();

    DelegateFreeAsyncWaitTests();
    DelegateMemberAsyncWaitTests();
    DelegateMemberSpAsyncWaitTests();
    DelegateFunctionAsyncWaitTests();

    workerThread.ExitThread();
}