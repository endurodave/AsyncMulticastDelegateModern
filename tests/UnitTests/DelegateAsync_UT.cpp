#include "DelegateLib.h"
#include "UnitTestCommon.h"
#include <iostream>
#include "WorkerThreadStd.h"
#ifdef WIN32
#include <Windows.h>
#endif

using namespace DelegateLib;
using namespace std;
using namespace UnitTestData;

static void DelegateFreeTests()
{
    int stackVal = 100;
    std::function<void(int)> LambdaCapture = [stackVal](int i) {
    };

    // TODO
}

void DelegateAsync_UT()
{

}