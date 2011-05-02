#ifndef PSIMPL_TEST_RADIAL_DISTANCE
#define PSIMPL_TEST_RADIAL_DISTANCE


#include "test.h"


namespace psimpl {
    namespace test
{
    //! Tests function psimpl::simplify_radial_distance
    class TestRadialDistance
    {
    public:
        TestRadialDistance ();

    private:
        void TestIncompletePoint ();
        void TestNotEnoughPoints ();
        void TestInvalidTol ();
        void TestValidTol ();
        void TestRandomIterator ();
        void TestBidirectionalIterator ();
        void TestForwardIterator ();
    };
}}


#endif // PSIMPL_TEST_RADIAL_DISTANCE
