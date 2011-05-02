#ifndef PSIMPL_TEST_SIMPLIFICATION
#define PSIMPL_TEST_SIMPLIFICATION


#include "test.h"
#include "TestNthPoint.h"
#include "TestRadialDistance.h"


namespace psimpl {
    namespace test
{
    class TestSimplification
    {
    public:
        TestSimplification () {
            TEST_RUN("nth point", TestNthPoint ());
            TEST_RUN("radial distance", TestRadialDistance ());
        }
    };
}}


#endif // PSIMPL_TEST_SIMPLIFICATION
