#include "TestRadialDistance.h"
#include "helper.h"
#include "../lib/psimpl.h"
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
        const unsigned DIM = 3;
        const unsigned count = 10;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;

        float tol = 0;
        psimpl::simplify_radial_distance <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);
    }

    // valid: tol != 0
    void TestRadialDistance::TestValidTol () {
        const unsigned DIM = 3;
        const unsigned count = 11;

        // tiny tol, no points removed
        {
            std::vector <float> polyline;
            std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
            std::vector <float> result;
            float tol = 0.01f;

            psimpl::simplify_radial_distance <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(polyline == result);
        }
        // large tol, all internal points removed
        {
            std::vector <float> polyline;
            std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
            std::vector <float> result;
            float tol = 100.f;

            psimpl::simplify_radial_distance <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            VERIFY_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        // normal tol, does not try to eat the last point
        {
            std::vector <float> polyline;
            std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
            std::vector <float> result;
            float tol = 4.5f;

            psimpl::simplify_radial_distance <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            VERIFY_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 5, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
        // normal tol, tries to eat the last point
        {
            std::vector <float> polyline;
            std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
            std::vector <float> result;
            float tol = 6.5f;

            psimpl::simplify_radial_distance <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            VERIFY_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 7, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
    }

    // different random access iterators, different value types, different dimensions
    void TestRadialDistance::TestRandomIterator () {
        const unsigned count = 10;
        {
            const unsigned DIM = 2;

            float polyline [count*DIM];
            std::generate_n (polyline, count*DIM, StraightLine <float, DIM> ());
            float result [count*DIM];
            float tol = 3.5f;

            psimpl::simplify_radial_distance <DIM> (
                    polyline, polyline + count*DIM, tol,
                    result);

            int keys [] = {0, 4, 8, 9};
            VERIFY_TRUE(ComparePoints <DIM> (polyline, result, std::vector <int> (keys, keys + 4)));
        }
        {
            const unsigned DIM = 3;
            std::vector <double> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <double, DIM> ());
            double tol = 2.5;

            psimpl::simplify_radial_distance <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            int keys [] = {0, 3, 6, 9};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 4)));
        }
        {
            const unsigned DIM = 4;
            std::deque <int> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <int, DIM> ());
            int tol = 5;

            psimpl::simplify_radial_distance <DIM> (
                    polyline.begin (), polyline.end (), tol,
                    std::back_inserter (result));

            int keys [] = {0, 5, 9};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
    }

    // bidirectional iterator, different value types, different dimensions
    void TestRadialDistance::TestBidirectionalIterator () {
        const unsigned count = 10;
        {
            const unsigned DIM = 2;
            std::list <float> polyline, result;
            std::generate_n (std::inserter (polyline, polyline.begin ()), count*DIM, StraightLine <float, DIM> ());
            float tol = 7.5f;

            psimpl::simplify_radial_distance <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::inserter (result, result.begin ()));

            int keys [] = {0, 8, 9};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
    }

    // forward iterator, different value types, different dimensions
    void TestRadialDistance::TestForwardIterator () {
        ASSERT_TRUE(false); // TODO
    }

}}
