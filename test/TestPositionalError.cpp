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

        psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

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

        psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

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

        psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

        ASSERT_FALSE(valid);

        // 1 polyline points, 4 simplification points
        std::generate_n (std::back_inserter (polyline), 1*DIM, StraightLine <float, DIM> ());

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        ASSERT_TRUE(result.empty ());
        ASSERT_FALSE(valid);

        psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

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

        psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

        ASSERT_FALSE(valid);

        // 4 polyline points, 1 simplification points
        std::generate_n (std::back_inserter (simplification), 1*DIM, StraightLine <float, DIM> ());

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        ASSERT_TRUE(result.empty ());
        ASSERT_FALSE(valid);

        psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

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

        psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

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

        psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

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

        psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

        ASSERT_FALSE(valid);

        // intermediate point don't match
        simplification = polyline;
        simplification [simplification.size () / 2] += 10.f;

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        // can only be detected half way through the algo, thus result will contain data
        ASSERT_FALSE(result.empty ());
        ASSERT_FALSE(valid);

        psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

        ASSERT_FALSE(valid);
    }

    // valid: all simplification points match an original polyline point
    void TestPositionalError::TestValidLines () {
        const unsigned DIM = 3;
        const unsigned count = 11;
        bool valid = false;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <float, DIM> ());

        std::vector <float> simplification;
        psimpl::simplify_nth_point <DIM> (polyline.begin (), polyline.end (), 4,
            std::back_inserter (simplification));
        // note that simplification is a straightline with keys: 0, 4, 8, 10
        ASSERT_TRUE(simplification.size () == 4*DIM);

        // add two more points to the polyline
        std::generate_n (std::back_inserter (polyline), 2*DIM, SawToothLine <float, DIM> ());

        std::vector <float> result;
        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);
        VERIFY_TRUE(valid);

        // errors should only be calculated for that part of the polyline that corresponds
        // to the simplification, meaning the extra added two points should be ignored
        ASSERT_TRUE(result.size () == count);
        VERIFY_TRUE(result [0] == 0.f);
        VERIFY_TRUE(result [1] == 1.f);
        VERIFY_TRUE(result [2] == 0.f);
        VERIFY_TRUE(result [3] == 4.f);
        VERIFY_TRUE(result [4] == 0.f);
        VERIFY_TRUE(result [5] == 9.f);
        VERIFY_TRUE(result [6] == 0.f);
        VERIFY_TRUE(result [7] == 16.f);
        VERIFY_TRUE(result [8] == 0.f);
        VERIFY_TRUE(result [9] == 25.f);
        VERIFY_TRUE(result [10] == 0.f);

        psimpl::math::Statistics stats =
            psimpl::compute_positional_error_statistics <DIM> (
                polyline.begin (), polyline.end (),
                simplification.begin (), simplification.end (),
                &valid);

        VERIFY_TRUE(valid);
        VERIFY_TRUE(stats.max == 5.0);
        VERIFY_TRUE(stats.sum == 15.0);
    }

    // different random access iterators, different value types, different dimensions
    void TestPositionalError::TestRandomIterator () {
        bool valid = false;
        {
            const unsigned DIM = 2;

            const unsigned pcount = 25;
            float polyline [pcount*DIM];
            std::generate_n (polyline, pcount*DIM, SquareToothLine <float, DIM> ());

            const unsigned scount = 4;
            float simplification [scount*DIM];
            std::generate_n (simplification, scount*DIM, StraightLine <float, DIM> (4));

            float result [pcount*DIM];

            psimpl::compute_positional_errors2 <DIM> (
                polyline, polyline + pcount*DIM,
                simplification, simplification + scount*DIM,
                result, &valid);

            VERIFY_TRUE(valid);

            VERIFY_TRUE(result [0] == 0.f);
            VERIFY_TRUE(result [1] == 0.f);
            VERIFY_TRUE(result [2] == 1.f);
            VERIFY_TRUE(result [3] == 1.f);
            VERIFY_TRUE(result [4] == 0.f);
            VERIFY_TRUE(result [5] == 0.f);
            VERIFY_TRUE(result [6] == 0.f);
            VERIFY_TRUE(result [7] == 1.f);
            VERIFY_TRUE(result [8] == 4.f);
            VERIFY_TRUE(result [9] == 4.f);

            psimpl::math::Statistics stats =
                psimpl::compute_positional_error_statistics <DIM> (
                    polyline, polyline + pcount*DIM,
                    simplification, simplification + scount*DIM,
                    &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(stats.max == 3.0);
            VERIFY_TRUE(stats.sum == 28.0);
        }
        {
            const unsigned DIM = 3;

            const unsigned pcount = 25;
            std::vector <double> polyline;
            std::generate_n (std::back_inserter (polyline), pcount*DIM, SquareToothLine <float, DIM> ());

            const unsigned scount = 4;
            std::vector <double> simplification;
            std::generate_n (std::back_inserter (simplification), scount*DIM, StraightLine <float, DIM> (4));

            std::vector <double> result;

            psimpl::compute_positional_errors2 <DIM> (
                polyline.begin (), polyline.end (),
                simplification.begin (), simplification.end (),
                std::back_inserter (result), &valid);

            VERIFY_TRUE(valid);

            ASSERT_TRUE(result.size () == pcount);
            VERIFY_TRUE(result [10] == 4.f);
            VERIFY_TRUE(result [11] == 1.f);
            VERIFY_TRUE(result [12] == 0.f);
            VERIFY_TRUE(result [13] == 0.f);
            VERIFY_TRUE(result [14] == 0.f);
            VERIFY_TRUE(result [15] == 0.f);
            VERIFY_TRUE(result [16] == 1.f);
            VERIFY_TRUE(result [17] == 4.f);
            VERIFY_TRUE(result [18] == 9.f);
            VERIFY_TRUE(result [19] == 9.f);

            psimpl::math::Statistics stats =
                psimpl::compute_positional_error_statistics <DIM> (
                    polyline.begin (), polyline.end (),
                    simplification.begin (), simplification.end (),
                    &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(stats.max == 3.0);
            VERIFY_TRUE(stats.sum == 28.0);
        }
        {
            const unsigned DIM = 4;

            const unsigned pcount = 25;
            std::deque <int> polyline;
            std::generate_n (std::back_inserter (polyline), pcount*DIM, SquareToothLine <float, DIM> ());

            const unsigned scount = 4;
            std::deque <int> simplification;
            std::generate_n (std::back_inserter (simplification), scount*DIM, StraightLine <float, DIM> (4));

            std::deque <int> result;

            psimpl::compute_positional_errors2 <DIM> (
                polyline.begin (), polyline.end (),
                simplification.begin (), simplification.end (),
                std::back_inserter (result), &valid);

            VERIFY_TRUE(valid);

            ASSERT_TRUE(result.size () == pcount);
            VERIFY_TRUE(result [20] == 9.f);
            VERIFY_TRUE(result [21] == 9.f);
            VERIFY_TRUE(result [22] == 4.f);
            VERIFY_TRUE(result [23] == 1.f);
            VERIFY_TRUE(result [24] == 0.f);

            psimpl::math::Statistics stats =
                psimpl::compute_positional_error_statistics <DIM> (
                    polyline.begin (), polyline.end (),
                    simplification.begin (), simplification.end (),
                    &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(stats.max == 3.0);
            VERIFY_TRUE(stats.sum == 28.0);
        }
    }

    // bidirectional iterator, different value types, different dimensions
    void TestPositionalError::TestBidirectionalIterator () {
        bool valid = false;
        {
            const unsigned DIM = 2;

            const unsigned pcount = 25;
            std::list <float> polyline;
            std::generate_n (std::back_inserter (polyline), pcount*DIM, SquareToothLine <float, DIM> ());

            const unsigned scount = 4;
            std::list <float> simplification;
            std::generate_n (std::back_inserter (simplification), scount*DIM, StraightLine <float, DIM> (4));

            std::list <float> result;

            psimpl::compute_positional_errors2 <DIM> (
                polyline.begin (), polyline.end (),
                simplification.begin (), simplification.end (),
                std::inserter (result, result.begin ()), &valid);

            VERIFY_TRUE(valid);

            ASSERT_TRUE(result.size () == pcount);

            std::list <float>::const_iterator it = result.begin ();
            VERIFY_TRUE(*it == 0.f); std::advance (it, 3);
            VERIFY_TRUE(*it == 1.f); std::advance (it, 3);
            VERIFY_TRUE(*it == 0.f); std::advance (it, 3);
            VERIFY_TRUE(*it == 4.f); std::advance (it, 3);
            VERIFY_TRUE(*it == 0.f); std::advance (it, 3);
            VERIFY_TRUE(*it == 0.f); std::advance (it, 3);
            VERIFY_TRUE(*it == 9.f); std::advance (it, 3);
            VERIFY_TRUE(*it == 9.f); std::advance (it, 3);
            VERIFY_TRUE(*it == 0.f);

            psimpl::math::Statistics stats =
                psimpl::compute_positional_error_statistics <DIM> (
                    polyline.begin (), polyline.end (),
                    simplification.begin (), simplification.end (),
                    &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(stats.max == 3.0);
            VERIFY_TRUE(stats.sum == 28.0);
        }
    }

    // forward iterator, different value types, different dimensions
    void TestPositionalError::TestForwardIterator () {
        FAIL("TODO");
    }

    void TestPositionalError::TestReturnValue () {
        const unsigned DIM = 3;
        const int count = 11;
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
        ASSERT_TRUE (
            std::distance (
                result, 
                psimpl::compute_positional_errors2 <DIM> (
                    polyline, polyline + count*DIM,
                    simplification, simplification,
                    result))
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
        ASSERT_TRUE (
            std::distance (
                result, 
                psimpl::compute_positional_errors2 <DIM> (
                    polyline, polyline + count*DIM,
                    simplification, simplification + count*DIM,
                    result))
            == count);
        ASSERT_TRUE(valid);
    }

}}
