#ifndef PSIMPL_TEST_MATH
#define PSIMPL_TEST_MATH


namespace psimpl {
    namespace test
{
    class TestMath
    {
    public:
        TestMath ();

    private:
        void TestEqual_RandomIterator ();
        void TestEqual_BidirectionalIterator ();
        void TestEqual_ForwardIterator ();
        void TestMakeVector_RandomIterator ();
        void TestMakeVector_BidirectionalIterator ();
        void TestMakeVector_ForwardIterator ();
        void TestDot_RandomIterator ();
        void TestDot_BidirectionalIterator ();
        void TestDot_ForwardIterator ();
        void TestInterpolate_Fractions ();
        void TestInterpolate_RandomIterator ();
        void TestInterpolate_BidirectionalIterator ();
        void TestInterpolate_ForwardIterator ();
    };
}}


#endif // PSIMPL_TEST_MATH
