#include "DelegateLib.h"
#include "UnitTestCommon.h"
#include <iostream>
#include "WorkerThreadStd.h"

using namespace DelegateLib;
using namespace std;
using namespace UnitTestData;

static WorkerThread workerThread("DelegateAsyncWait_UT");

static void DelegateFreeAsyncWaitTests()
{
    using Del = DelegateFreeAsyncWait<void(int)>;

    Del delegate1(FreeFuncInt1, workerThread);
    delegate1(TEST_INT);
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
}

static void DelegateMemberAsyncWaitTests()
{
    using Del = DelegateMemberAsyncWait<TestClass1, void(int)>;

    TestClass1 testClass1;

    Del delegate1(&testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate1(TEST_INT);
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
    Class c;
    DelegateMemberAsyncWait<const Class, std::uint16_t(void)> dConstClass;
    //dConstClass.Bind(&c, &Class::Func, workerThread);     // Not OK. Should fail compile.
    dConstClass.Bind(&c, &Class::FuncConst, workerThread);  // OK
    auto rConst = dConstClass();
}

static void DelegateMemberSpAsyncWaitTests()
{
    using Del = DelegateMemberAsyncWait<TestClass1, void(int)>;

    auto testClass1 = std::make_shared<TestClass1>();

    Del delegate1(testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate1(TEST_INT);
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
}

static void DelegateFunctionAsyncWaitTests()
{
    using Del = DelegateFunctionAsyncWait<void(int)>;

    Del delegate1(LambdaNoCapture, workerThread);
    delegate1(TEST_INT);
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