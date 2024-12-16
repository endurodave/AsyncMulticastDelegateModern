#include "DelegateLib.h"
#include "UnitTestCommon.h"
#include <iostream>
#include "WorkerThreadStd.h"

using namespace DelegateLib;
using namespace std;
using namespace UnitTestData;

static WorkerThread workerThread("DelegateAsync_UT");

static void DelegateFreeAsyncTests()
{
    DelegateFreeAsync<void(int)> delegate1(FreeFuncInt1, workerThread);
    delegate1(TEST_INT);

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    DelegateFreeAsync<void(int)> delegate3(FreeFuncInt1, workerThread);
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

    DelegateFreeAsync<void(int)> delegate6(FreeFuncInt1, workerThread);
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());

    DelegateFunction<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));
}

static void DelegateMemberAsyncTests()
{
    TestClass1 testClass1;

    DelegateMemberAsync<TestClass1, void(int)> delegate1(&testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate1(TEST_INT);

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    DelegateMemberAsync<TestClass1, void(int)> delegate3(&testClass1, &TestClass1::MemberFuncInt1, workerThread);
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

    DelegateMemberAsync<TestClass1, void(int)> delegate6(&testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());

    DelegateFunction<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));
}

static void DelegateMemberSpAsyncTests()
{
    auto testClass1 = std::make_shared<TestClass1>();

    DelegateMemberSpAsync<TestClass1, void(int)> delegate1(testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate1(TEST_INT);

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    DelegateMemberSpAsync<TestClass1, void(int)> delegate3(testClass1, &TestClass1::MemberFuncInt1, workerThread);
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

    DelegateMemberSpAsync<TestClass1, void(int)> delegate6(testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());

    DelegateFunction<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));
}

static void DelegateFunctionAsyncTests()
{
    DelegateFunctionAsync<void(int)> delegate1(LambdaNoCapture, workerThread);
    delegate1(TEST_INT);

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    DelegateFunctionAsync<void(int)> delegate3(LambdaNoCapture, workerThread);
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

    DelegateFunctionAsync<void(int)> delegate6(LambdaNoCapture, workerThread);
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());

    DelegateFree<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));
}

void DelegateAsync_UT()
{
    workerThread.CreateThread();

    DelegateFreeAsyncTests();
    DelegateMemberAsyncTests();
    DelegateMemberSpAsyncTests();
    DelegateFunctionAsyncTests();

    workerThread.ExitThread();
}