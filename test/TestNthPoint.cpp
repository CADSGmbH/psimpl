#include "TestNthPoint.h"
#include "helper.h"
#include "../lib/psimpl.h"
#include <vector>
#include <deque>
#include <list>


namespace psimpl {
    namespace test
{
    TestNthPoint::TestNthPoint () {
        TEST_RUN("incomplete point", TestIncompletePoint ());
        TEST_RUN("not enough points", TestNotEnoughPoints ());
        TEST_RUN("invalid n", TestInvalidN ());
        TEST_RUN("valid n", TestValidN ());
        TEST_RUN("random iterator", TestRandomIterator ());
        TEST_RUN("bidirectional iterator", TestBidirectionalIterator ());
        TEST_DISABLED("forward iterator", TestForwardIterator ());
    }

    // incomplete point: coord count % DIM > 1
    void TestNthPoint::TestIncompletePoint () {
        const unsigned DIM = 2;

        // 4th point incomplete
        std::vector<float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM-1, StraightLine<float, DIM>());
        std::vector <float> result;

        psimpl::simplify_nth_point <DIM> (
            polyline.begin (), polyline.end (), 2,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);

        // 4th point complete
        polyline.push_back (4.f);
        result.clear ();

        psimpl::simplify_nth_point <DIM> (
            polyline.begin (), polyline.end (), 2,
            std::back_inserter (result));

        ASSERT_FALSE(polyline == result);
    }

    // not enough points: point count < 3
    void TestNthPoint::TestNotEnoughPoints () {
        const unsigned DIM = 2;

        // 0 points
        std::vector <float> polyline;
        std::vector <float> result;

        psimpl::simplify_nth_point <DIM> (
            polyline.begin (), polyline.end (), 2,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);

        // 1 point
        polyline.push_back(1.f);
        polyline.push_back(1.f);
        result.clear ();

        psimpl::simplify_nth_point <DIM> (
            polyline.begin (), polyline.end (), 2,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);

        // 2 points
        polyline.push_back(2.f);
        polyline.push_back(2.f);
        result.clear ();

        psimpl::simplify_nth_point <DIM> (
            polyline.begin (), polyline.end (), 2,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);
    }

    // invalid: n < 2
    void TestNthPoint::TestInvalidN () {
        const unsigned DIM = 2;

        // 4 points, n == 0
        std::vector<float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM, StraightLine<float, DIM>());
        std::vector <float> result;

        psimpl::simplify_nth_point <DIM> (
            polyline.begin (), polyline.end (), 0,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);

        // 4 points, n == 1
        result.clear ();

        psimpl::simplify_nth_point <DIM> (
            polyline.begin (), polyline.end (), 1,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);
    }

    // valid: n >= 2
    void TestNthPoint::TestValidN () {
        const unsigned DIM = 2;

        std::vector<float> polyline;
        std::generate_n (std::back_inserter (polyline), 11*DIM, StraightLine<float, DIM>());
        std::vector<float> result;

        // -----------------------
        // large: n >= point count
        // -----------------------
        {
            // 11 points, n == 12
            unsigned n = 12;
            result.clear ();

            psimpl::simplify_nth_point <DIM> (
                polyline.begin (), polyline.end (), n,
                std::back_inserter (result));

            VERIFY_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        {
            // 11 points, n == 11
            result.clear ();
            unsigned n = 11;

            psimpl::simplify_nth_point <DIM> (
                polyline.begin (), polyline.end (), n,
                std::back_inserter (result));
                
            VERIFY_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        // ---------------------------------
        // perfect fit: n + 1 == point count
        // ---------------------------------
        {
            // 11 points, n == 10
            unsigned n = 10;
            result.clear ();

            psimpl::simplify_nth_point <DIM> (
                polyline.begin (), polyline.end (), n,
                std::back_inserter (result));
                
            VERIFY_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        // --------------------------------
        // good fit: m*n + 1 == point count
        // --------------------------------
        {
        // 11 points, n == 5
            unsigned n = 5;
            result.clear ();

            psimpl::simplify_nth_point <DIM> (
                polyline.begin (), polyline.end (), n,
                std::back_inserter (result));
                
            VERIFY_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 5, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
        // -------------------------------
        // bad fit: m*n + 1 != point count
        // -------------------------------
        {
            // 11 points, n == 4
            unsigned n = 4;
            result.clear ();

            psimpl::simplify_nth_point <DIM> (
                polyline.begin (), polyline.end (), n,
                std::back_inserter (result));
                
            VERIFY_TRUE(result.size () == 4*DIM);
            int keys [] = {0, 4, 8, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 4)));
        }
    }

    // different random access iterators, different value types, different dimensions
    void TestNthPoint::TestRandomIterator () {
        const unsigned n = 2;
        const unsigned count = 5;
        {
            const unsigned DIM = 2;

            float polyline [count*DIM];
                std::generate_n (polyline, count*DIM, StraightLine <float, DIM> ());
            float result [count*DIM];

            psimpl::simplify_nth_point <DIM> (
                    polyline, polyline + count*DIM, n,
                    result);

            int keys [] = {0, 2, 4};
            VERIFY_TRUE(ComparePoints <DIM> (polyline, result, std::vector <int> (keys, keys + 3)));
        }
        {
            const unsigned DIM = 3;
            std::vector <double> polyline, result;
                std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <double, DIM>());

            psimpl::simplify_nth_point <DIM> (
                polyline.begin (), polyline.end (), n,
                std::back_inserter (result));

            int keys [] = {0, 2, 4};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
        {
            const unsigned DIM = 4;
            std::deque <int> polyline, result;
                std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <int, DIM> ());

            psimpl::simplify_nth_point <DIM> (
                    polyline.begin (), polyline.end (), n,
                    std::back_inserter (result));

            int keys [] = {0, 2, 4};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
    }

    // bidirectional iterator, different value types, different dimensions
    void TestNthPoint::TestBidirectionalIterator () {
        const unsigned n = 4;
        const unsigned count = 10;
        {
            const unsigned DIM = 2;
            std::list <float> polyline, result;
                std::generate_n (std::inserter (polyline, polyline.begin ()), count*DIM, StraightLine <float, DIM>());

            psimpl::simplify_nth_point <DIM> (
                polyline.begin (), polyline.end (), n,
                std::inserter (result, result.begin ()));

            int keys [] = {0, 4, 8, 9};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 4)));
        }
    }

    // forward iterator, different value types, different dimensions
    void TestNthPoint::TestForwardIterator () {
        ASSERT_TRUE(false); // TODO
    }
}}

