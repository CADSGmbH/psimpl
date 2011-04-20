#ifndef PSIMPL_TEST_NTHPOINT
#define PSIMPL_TEST_NTHPOINT


#include "test.h"


namespace psimpl {
    namespace test
{
    class TestNthPoint
    {
    public:
        TestNthPoint ();

    private:
        void TestIncompletePoint ();
        void TestNotEnoughPoints ();
        void TestInvalidN ();
        void TestValidN ();
        void TestRandomIterator ();
        void TestBidirectionalIterator ();
        void TestForwardIterator ();
    };
}}


#endif // PSIMPL_TEST_NTHPOINT
