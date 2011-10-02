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

/*
    psimpl - generic n-dimensional polyline simplification
    Copyright (C) 2010-2011 Elmar de Koning, edekoning@gmail.com

    This file is part of psimpl, and is hosted at SourceForge:
    http://sourceforge.net/projects/psimpl/
*/

#include "TestPositionalError.h"
#include "helper.h"
#include "../lib/psimpl.h"
#include <iterator>
#include <vector>
#include <deque>
#include <list>


namespace psimpl {
    namespace test
{
    TestPositionalError::TestPositionalError () {
        TEST_RUN("incomplete point", TestIncompletePoint ());
        TEST_RUN("not enough points", TestNotEnoughPoints ());
        TEST_RUN("invalid lines", TestInvalidLines ());
        TEST_RUN("valid lines", TestValidLines ());
        TEST_RUN("random iterator", TestRandomIterator ());
        TEST_RUN("bidirectional iterator", TestBidirectionalIterator ());
        TEST_DISABLED("forward iterator", TestForwardIterator ());
        TEST_RUN("return value", TestReturnValue ());
    }

    // incomplete point: coord count % DIM > 1
    void TestPositionalError::TestIncompletePoint () {
        const unsigned DIM = 2;
        bool valid = false;

        // 4th polyline point incomplete
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM, StraightLine <float, DIM> ());
        std::vector <float> simplification = polyline;
        polyline.pop_back ();
        std::vector <float> result;

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        ASSERT_TRUE(result.empty ());
        ASSERT_FALSE(valid);

        // 4th simplification point incomplete
        polyline = simplification;
        simplification.pop_back ();

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        ASSERT_TRUE(result.empty ());
        ASSERT_FALSE(valid);
    }

    // not enough points: point count < 3
    void TestPositionalError::TestNotEnoughPoints () {
        const unsigned DIM = 2;
        bool valid = false;

        // 0 polyline points, 4 simplification points
        std::vector <float> polyline;
        std::vector <float> simplification;
        std::generate_n (std::back_inserter (simplification), 4*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        ASSERT_TRUE(result.empty ());
        ASSERT_FALSE(valid);

        // 1 polyline points, 4 simplification points
        std::generate_n (std::back_inserter (polyline), 1*DIM, StraightLine <float, DIM> ());

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        ASSERT_TRUE(result.empty ());
        ASSERT_FALSE(valid);

        // 4 polyline points, 0 simplification points
        polyline = simplification;
        simplification.clear ();

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        ASSERT_TRUE(result.empty ());
        ASSERT_FALSE(valid);

        // 4 polyline points, 1 simplification points
        std::generate_n (std::back_inserter (simplification), 1*DIM, StraightLine <float, DIM> ());

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        ASSERT_TRUE(result.empty ());
        ASSERT_FALSE(valid);

        // polyline points < simplification points
        simplification.clear ();
        std::generate_n (std::back_inserter (simplification), 10*DIM, StraightLine <float, DIM> ());

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        ASSERT_TRUE(result.empty ());
        ASSERT_FALSE(valid);
    }

    // invalid: tol == 0
    void TestPositionalError::TestInvalidLines () {
        const unsigned DIM = 2;
        bool valid = false;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM, StraightLine <float, DIM> ());
        std::vector <float> simplification = polyline;
        std::vector <float> result;

        // first points don't match
        simplification [0] += 10.f;

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        ASSERT_TRUE(result.empty ());
        ASSERT_FALSE(valid);

        // last points don't match
        simplification = polyline;
        simplification [simplification.size () - 1] += 10.f;

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        // can only be detected at the end of the algo, thus result will contain data
        ASSERT_FALSE(result.empty ());
        ASSERT_FALSE(valid);
    }

    // valid: tol != 0
    void TestPositionalError::TestValidLines () {
        FAIL("TODO");
    }

    // different random access iterators, different value types, different dimensions
    void TestPositionalError::TestRandomIterator () {
        FAIL("TODO");
        //const unsigned count = 10;
        //{
        //    const unsigned DIM = 2;

        //    float polyline [count*DIM];
        //    std::generate_n (polyline, count*DIM, StraightLine <float, DIM> ());
        //    float result [count*DIM];
        //    float tol = 3.5f;

        //    psimpl::simplify_radial_distance <DIM> (
        //            polyline, polyline + count*DIM, tol,
        //            result);

        //    int keys [] = {0, 4, 8, 9};
        //    VERIFY_TRUE(ComparePoints <DIM> (polyline, result, std::vector <int> (keys, keys + 4)));
        //}
        //{
        //    const unsigned DIM = 3;
        //    std::vector <double> polyline, result;
        //    std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <double, DIM> ());
        //    double tol = 2.5;

        //    psimpl::simplify_radial_distance <DIM> (
        //        polyline.begin (), polyline.end (), tol,
        //        std::back_inserter (result));

        //    int keys [] = {0, 3, 6, 9};
        //    VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 4)));
        //}
        //{
        //    const unsigned DIM = 4;
        //    std::deque <int> polyline, result;
        //    std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <int, DIM> ());
        //    int tol = 5;

        //    psimpl::simplify_radial_distance <DIM> (
        //            polyline.begin (), polyline.end (), tol,
        //            std::back_inserter (result));

        //    int keys [] = {0, 5, 9};
        //    VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        //}
    }

    // bidirectional iterator, different value types, different dimensions
    void TestPositionalError::TestBidirectionalIterator () {
        FAIL("TODO");
        //const unsigned count = 10;
        //{
        //    const unsigned DIM = 2;
        //    std::list <float> polyline, result;
        //    std::generate_n (std::inserter (polyline, polyline.begin ()), count*DIM, StraightLine <float, DIM> ());
        //    float tol = 7.5f;

        //    psimpl::simplify_radial_distance <DIM> (
        //        polyline.begin (), polyline.end (), tol,
        //        std::inserter (result, result.begin ()));

        //    int keys [] = {0, 8, 9};
        //    VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        //}
    }

    // forward iterator, different value types, different dimensions
    void TestPositionalError::TestForwardIterator () {
        FAIL("TODO");
    }

    void TestPositionalError::TestReturnValue () {
        const unsigned DIM = 3;
        const unsigned count = 11;
        bool valid = false;

        float polyline [count*DIM];
        std::generate_n (polyline, count*DIM, StraightLine <float, DIM> ());
        float simplification [count*DIM];
        std::generate_n (simplification, count*DIM, StraightLine <float, DIM> ());
        float result [count*DIM];

        // invalid input
        ASSERT_TRUE (
            std::distance (
                result, 
                psimpl::compute_positional_errors2 <DIM> (
                    polyline, polyline + count*DIM,
                    simplification, simplification,
                    result, &valid))
            == 0);
        ASSERT_TRUE (
            std::distance (
                result, 
                psimpl::compute_positional_errors2 <DIM> (
                    polyline, polyline + count*DIM,
                    simplification, simplification,
                    result, NULL))
            == 0);
        ASSERT_FALSE(valid);

        // valid input
        ASSERT_TRUE (
            std::distance (
                result, 
                psimpl::compute_positional_errors2 <DIM> (
                    polyline, polyline + count*DIM,
                    simplification, simplification + count*DIM,
                    result, &valid))
            == count);
        ASSERT_TRUE (
            std::distance (
                result, 
                psimpl::compute_positional_errors2 <DIM> (
                    polyline, polyline + count*DIM,
                    simplification, simplification + count*DIM,
                    result, NULL))
            == count);
        ASSERT_TRUE(valid);
    }

}}
