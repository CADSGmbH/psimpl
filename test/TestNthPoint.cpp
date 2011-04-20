#include "TestNthPoint.h"
#include "helper.h"
#include "../psimpl.h"
#include <vector>


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
        TEST_RUN("forward iterator", TestForwardIterator ());
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
        //unsigned count = 5;
        //unsigned n = 2;
        //float[] polyline1 = new float[count];
        //    std::generate_n (polyline1, count*2, StraightLine<float, 2>());
        //float[] result1 = new float[count];

        //psimpl::simplify_nth_point (
        //        polyline1, polyline1+count*2, n,
        //        result1);

        //int keys[] = {0, 2, 4};
        //VERIFY_TRUE(ComparePoints <2> (polyline1, polyline1 + count*2, result1, result1 + 3*2, keys, 3));

        //std::vector<double> polyline2, result2;
        //    std::generate_n (std::back_inserter (polyline2), count*3, StraightLine<double, 3>());

        //psimpl::simplify_nth_point (
        //        polyline2.begin (), polyline2.end (), n,
        //        std::back_inserter (result2));

        //int keys[] = {0, 2, 4};
        //VERIFY_TRUE(ComparePoints <2> (polyline2.begin (), polyline2.end (), result2.begin (), result2.end (), keys, 3));

        //std::deque<int> polyline3, result3;
        //    std::generate_n (std::back_inserter (polyline3), count*4, StraightLine<int, 4>());

        //psimpl::simplify_nth_point (
        //        polyline3.begin (), polyline3.end (), n,
        //        std::back_inserter (result3));

        ////todo expect points 0, 2, 4
        //int keys[] = {0, 2, 4};
        //VERIFY_TRUE(ComparePoints <2> (polyline3.begin (), polyline3.end (), result3.begin (), result3.end (), keys, 3));
    }

    // different bidirectional iterators, different value types, different dimensions
    void TestNthPoint::TestBidirectionalIterator () {
        // TODO
        // list<float>
        // set<short>
    }

    // forward iterator, different value types, different dimensions
    void TestNthPoint::TestForwardIterator () {
        // todo
    }
}}

