#include "TestRadialDistance.h"
#include "helper.h"
#include "../psimpl.h"
#include <vector>
#include <deque>
#include <list>


namespace psimpl {
    namespace test
{
    TestRadialDistance::TestRadialDistance () {
        TEST_RUN("incomplete point", TestIncompletePoint ());
        TEST_RUN("not enough points", TestNotEnoughPoints ());
        TEST_RUN("invalid tol", TestInvalidTol ());
        TEST_RUN("valid tol", TestValidTol ());
        TEST_RUN("random iterator", TestRandomIterator ());
        TEST_RUN("bidirectional iterator", TestBidirectionalIterator ());
        TEST_DISABLED("forward iterator", TestForwardIterator ());
    }

    // incomplete point: coord count % DIM > 1
    void TestRadialDistance::TestIncompletePoint () {
        const unsigned DIM = 2;
        const float tol = 2.f;

        // 4th point incomplete
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM-1, StraightLine <float, DIM> ());
        std::vector <float> result;

        psimpl::simplify_radial_distance <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);

        // 4th point complete
        polyline.push_back (4.f);
        result.clear ();

        psimpl::simplify_radial_distance <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        ASSERT_FALSE(polyline == result);
    }

    // not enough points: point count < 3
    void TestRadialDistance::TestNotEnoughPoints () {
        const unsigned DIM = 2;
        const float tol = 2.f;

        // 0 points
        std::vector <float> polyline;
        std::vector <float> result;

        psimpl::simplify_radial_distance <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);

        // 1 point
        polyline.push_back(1.f);
        polyline.push_back(1.f);
        result.clear ();

        psimpl::simplify_radial_distance <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);

        // 2 points
        polyline.push_back(2.f);
        polyline.push_back(2.f);
        result.clear ();

        psimpl::simplify_radial_distance <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);
    }

    // invalid: tol == 0
    void TestRadialDistance::TestInvalidTol () {
    }

    // valid: tol != 0
    void TestRadialDistance::TestValidTol () {
    }

    // different random access iterators, different value types, different dimensions
    void TestRadialDistance::TestRandomIterator () {
    }

    // bidirectional iterator, different value types, different dimensions
    void TestRadialDistance::TestBidirectionalIterator () {
    }

    // forward iterator, different value types, different dimensions
    void TestRadialDistance::TestForwardIterator () {
    }

}}
