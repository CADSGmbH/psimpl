#ifndef PSIMPL_TEST_SIMPLIFICATION
#define PSIMPL_TEST_SIMPLIFICATION


#include "test.h"
#include "TestNthPoint.h"


namespace psimpl {
    namespace test
{
    class TestSimplification
    {
    public:
        TestSimplification () {
            TEST_RUN("nth point", TestNthPoint ());
        }
    };
}}


#endif // PSIMPL_TEST_SIMPLIFICATION
