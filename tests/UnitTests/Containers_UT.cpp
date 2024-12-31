#include "DelegateLib.h"
#include "UnitTestCommon.h"
#include <iostream>
#include <set>
#include <cstring>
#include "WorkerThreadStd.h"

using namespace DelegateLib;
using namespace std;
using namespace UnitTestData;

static void UnicastDelegateTests()
{
    UnicastDelegate<void(int)> src;
    src = MakeDelegate(FreeFuncInt1);
    ASSERT_TRUE(src.Size() == 1);
    auto dest = std::move(src);
    ASSERT_TRUE(src.Size() == 0);
    ASSERT_TRUE(dest.Size() == 1);

    UnicastDelegate<void(int)> src2(dest);
    ASSERT_TRUE(src2.Size() == 1);
    ASSERT_TRUE(dest.Size() == 1);

    dest.Clear();
    src = MakeDelegate(&FreeFuncInt1);
    ASSERT_TRUE(src.Size() == 1);
    dest = std::move(src);
    ASSERT_TRUE(src.Size() == 0);
    ASSERT_TRUE(dest.Size() == 1);

    src = dest;
    ASSERT_TRUE(src.Size() == 1);
    ASSERT_TRUE(dest.Size() == 1);

    dest.Clear();
    ASSERT_TRUE(dest.Size() == 0);

    src = MakeDelegate(&FreeFuncInt1);
    ASSERT_TRUE(src.Size() == 1);
    src = nullptr;
    ASSERT_TRUE(src.Size() == 0);

    dest.Clear();
    src = MakeDelegate(&FreeFuncInt1);
    dest = src;
    ASSERT_TRUE(src.Size() == 1);
    ASSERT_TRUE(dest.Size() == 1);

    // Invoke target
    src(TEST_INT);
    src.Broadcast(TEST_INT);
}

static void MulticastDelegateTests()
{
    MulticastDelegate<void(int)> src;
    src += MakeDelegate(&FreeFuncInt1);
    ASSERT_TRUE(src.Size() == 1);
    auto dest = std::move(src);
    ASSERT_TRUE(src.Size() == 0);
    ASSERT_TRUE(dest.Size() == 1);

    MulticastDelegate<void(int)> src2(dest);
    ASSERT_TRUE(src2.Size() == 1);
    ASSERT_TRUE(dest.Size() == 1);

    dest.Clear();
    src += MakeDelegate(&FreeFuncInt1);
    dest = std::move(src);
    ASSERT_TRUE(src.Size() == 0);
    ASSERT_TRUE(dest.Size() == 1);
    
    src = dest;
    ASSERT_TRUE(src.Size() == 1);
    ASSERT_TRUE(dest.Size() == 1);

    dest.Clear();
    ASSERT_TRUE(dest.Size() == 0);

    src += MakeDelegate(&FreeFuncInt1);
    ASSERT_TRUE(src.Size() == 2);
    src = nullptr;
    ASSERT_TRUE(src.Size() == 0);

    dest.Clear();
    src.PushBack(MakeDelegate(&FreeFuncInt1));
    dest = src;
    ASSERT_TRUE(src.Size() == 1);
    ASSERT_TRUE(dest.Size() == 1);

    src += DelegateFree<void(int)>();
    src += DelegateFree<void(int)>();
    ASSERT_TRUE(src.Size() == 3);
    src.Remove(DelegateFree<void(int)>());
    ASSERT_TRUE(src.Size() == 2);

    // Invoke all targets
    src(TEST_INT);
    src.Broadcast(TEST_INT);
}

static void MulticastDelegateSafeTests()
{
    MulticastDelegateSafe<void(int)> src;
    MulticastDelegateSafe<void(int)> dest;
    src += MakeDelegate(&FreeFuncInt1);
    ASSERT_TRUE(src.Size() == 1);
    ASSERT_TRUE(dest.Size() == 0);

    src.Clear();
    dest.Clear();
    src += MakeDelegate(&FreeFuncInt1);
    dest = std::move(src);
    ASSERT_TRUE(src.Size() == 0);
    ASSERT_TRUE(dest.Size() == 1);

    src = dest;
    ASSERT_TRUE(src.Size() == 1);
    ASSERT_TRUE(dest.Size() == 1);

    dest.Clear();
    ASSERT_TRUE(dest.Size() == 0);

    src += MakeDelegate(&FreeFuncInt1);
    ASSERT_TRUE(src.Size() == 2);
    src = nullptr;
    ASSERT_TRUE(src.Size() == 0);

    dest.Clear();
    src.PushBack(MakeDelegate(&FreeFuncInt1));
    dest = src;
    ASSERT_TRUE(src.Size() == 1);
    ASSERT_TRUE(dest.Size() == 1);

    src += DelegateFree<void(int)>();
    src += DelegateFree<void(int)>();
    ASSERT_TRUE(src.Size() == 3);
    src.Remove(DelegateFree<void(int)>());
    ASSERT_TRUE(src.Size() == 2);

    // Invoke all targets
    src(TEST_INT);
    src.Broadcast(TEST_INT);
}

void Containers_UT()
{
    UnicastDelegateTests();
    MulticastDelegateTests();
    MulticastDelegateSafeTests();
}