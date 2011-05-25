/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is
 * 'psimpl - generic n-dimensional polyline simplification'.
 *
 * The Initial Developer of the Original Code is
 * Elmar de Koning.
 * Portions created by the Initial Developer are Copyright (C) 2010-2011
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * ***** END LICENSE BLOCK ***** */


#include "TestLang.h"
#include "helper.h"
#include "../lib/psimpl.h"
#include <vector>
#include <deque>
#include <list>


namespace psimpl {
    namespace test
{
    TestLang::TestLang () {
        TEST_RUN("incomplete point", TestIncompletePoint ());
        TEST_RUN("not enough points", TestNotEnoughPoints ());
        TEST_RUN("invalid tol", TestInvalidTol ());
        TEST_RUN("valid tol", TestValidTol ());
        TEST_RUN("invalid look ahead", TestInvalidLookAhead ());
        TEST_RUN("valid look ahead", TestValidLookAhead ());
        TEST_RUN("basic sanity", TestBasicSanity ());
        TEST_RUN("random iterator", TestRandomIterator ());
        TEST_RUN("bidirectional iterator", TestBidirectionalIterator ());
        TEST_RUN("return value", TestReturnValue ());
    }

    // incomplete point: coord count % DIM > 1
    void TestLang::TestIncompletePoint () {
        const unsigned DIM = 2;
        const float tol = 2.f;
        const unsigned lookAhead = 7;

        // 4th point incomplete
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM-1, StraightLine <float, DIM> ());
        std::vector <float> result;

        psimpl::simplify_lang <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);

        // 4th point complete
        polyline.push_back (4.f);
        result.clear ();

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        ASSERT_FALSE(polyline == result);
    }

    // not enough points: point count < 3
    void TestLang::TestNotEnoughPoints () {
        const unsigned DIM = 2;
        const float tol = 2.f;
        const unsigned lookAhead = 7;

        // 0 points
        std::vector <float> polyline;
        std::vector <float> result;

        psimpl::simplify_lang <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);

        // 1 point
        polyline.push_back(1.f);
        polyline.push_back(1.f);
        result.clear ();

        psimpl::simplify_lang <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);

        // 2 points
        polyline.push_back(2.f);
        polyline.push_back(2.f);
        result.clear ();

        psimpl::simplify_lang <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);
    }

    // invalid: tol == 0
    void TestLang::TestInvalidTol () {
        const unsigned DIM = 3;
        const unsigned count = 10;
        const unsigned lookAhead = 7;
        float tol = 0;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;
        
        psimpl::simplify_lang <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);
    }

    // valid: tol != 0
    void TestLang::TestValidTol () {
        const unsigned DIM = 2;
        const unsigned count = 9;
        const unsigned lookAhead = 10;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        // add a spike at the 5th point
        polyline [4*DIM+1] = 2.f;
        {
            float tol = 1.1f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            VERIFY_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 3, 4, 6, 8};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));
        }
        {
            float tol = 1.9f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            VERIFY_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 4, 8};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
        {
            float tol = 2.1f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            VERIFY_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
    }

    // invalid: look ahead == 0
    void TestLang::TestInvalidLookAhead () {
        const unsigned DIM = 3;
        const unsigned count = 10;
        const unsigned lookAhead = 0;
        float tol = 2.f;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;

        psimpl::simplify_lang <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        ASSERT_TRUE(polyline == result);
    }

    // valid: look ahead != 0
    void TestLang::TestValidLookAhead () {
        const unsigned DIM = 2;
        const unsigned count = 7;
        const float tol = 2.f;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());

        // large: lookahead >= point count
        {
            std::vector <float> result;
            const unsigned lookAhead = 7;

            psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (), tol, lookAhead,
                    std::back_inserter (result));

            VERIFY_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        {
            std::vector <float> result;
            const unsigned lookAhead = 8;

            psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (), tol, lookAhead,
                    std::back_inserter (result));

            VERIFY_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        // perfect fit: lookahead + 1 == point count
        {
            std::vector <float> result;
            const unsigned lookAhead = 6;

            psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (), tol, lookAhead,
                    std::back_inserter (result));

            VERIFY_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        // good fit: m*look ahead + 1 == point count
        {
            std::vector <float> result;
            const unsigned lookAhead = 3;

            psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (), tol, lookAhead,
                    std::back_inserter (result));

            VERIFY_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 3, 6};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
        // bad fit: m*look ahead + 1 != point count
        {
            std::vector <float> result;
            const unsigned lookAhead = 4;

            psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (), tol, lookAhead,
                    std::back_inserter (result));

            VERIFY_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 4, 6};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
        {
            std::vector <float> result;
            const unsigned lookAhead = 5;

            psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (), tol, lookAhead,
                    std::back_inserter (result));

            VERIFY_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 5, 6};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
    }

    void TestLang::TestBasicSanity () {
        FAIL("TODO");
        // test with
        //    spike at the start of the search window
        //    spike at the end of the search window
        //    spike one after the start of the polyline
        //    spike at the start of the polyline
        //    spike one before the end of the polyline
        //    spike at the end of the polyline
    }

    void TestLang::TestRandomIterator () {
        FAIL("TODO");
        //const unsigned count = 7;
        //{
        //    const unsigned DIM = 2;

        //    float polyline [count*DIM];
        //    std::generate_n (polyline, count*DIM, SawToothLine <float, DIM> ());
        //    float result [count*DIM];
        //    float tol = 2.5f;

        //    psimpl::simplify_perpendicular_distance <DIM> (
        //            polyline, polyline + count*DIM, tol,
        //            result);

        //    int keys [] = {0, 2, 4, 5, 6};
        //    VERIFY_TRUE(ComparePoints <DIM> (polyline, result, std::vector <int> (keys, keys + 5)));
        //}
        //{
        //    const unsigned DIM = 3;
        //    std::vector <double> polyline, result;
        //    std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <double, DIM> ());
        //    double tol = 2.5;

        //    psimpl::simplify_perpendicular_distance <DIM> (
        //        polyline.begin (), polyline.end (), tol,
        //        std::back_inserter (result));

        //    VERIFY_TRUE(result.size () == 5*DIM);
        //    int keys [] = {0, 2, 4, 5, 6};
        //    VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));
        //}
        //{
        //    const unsigned DIM = 4;
        //    std::deque <int> polyline, result;
        //    std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <int, DIM> ());
        //    int tol = 2;

        //    psimpl::simplify_perpendicular_distance <DIM> (
        //            polyline.begin (), polyline.end (), tol,
        //            std::back_inserter (result));

        //    VERIFY_TRUE(result.size () == 6*DIM);
        //    int keys [] = {0, 2, 3, 4, 5, 6};
        //    VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 6)));
        //}
    }

    void TestLang::TestBidirectionalIterator () {
        FAIL("TODO");
        //const unsigned count = 10;
        //{
        //    const unsigned DIM = 2;
        //    std::list <float> polyline, result;
        //    std::generate_n (std::inserter (polyline, polyline.begin ()), count*DIM, SawToothLine <float, DIM> ());
        //    float tol = 3.5f;

        //    psimpl::simplify_perpendicular_distance <DIM> (
        //        polyline.begin (), polyline.end (), tol,
        //        std::inserter (result, result.begin ()));

        //    VERIFY_TRUE(result.size () == 7*DIM);
        //    int keys [] = {0, 2, 4, 6, 7, 8, 9};
        //    VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 7)));
        //}
    }

    void TestLang::TestReturnValue () {
        const unsigned DIM = 3;
        const unsigned count = 11;

        float polyline [count*DIM];
        std::generate_n (polyline, count*DIM, StraightLine <float, DIM> ());
        float result [count*DIM];

        // invalid input
        ASSERT_TRUE(
            std::distance (
                result, 
                psimpl::simplify_lang <DIM> (
                    polyline, polyline + count*DIM, 0, 0,
                    result))
            == count*DIM);

        // valid input
        ASSERT_TRUE(
            std::distance (
                result, 
                psimpl::simplify_lang <DIM> (
                    polyline, polyline + count*DIM, 10.f, 10,
                    result))
            == 2*DIM);
    }
}}