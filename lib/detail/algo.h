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

#ifndef PSIMPL_DETAIL_ALGO
#define PSIMPL_DETAIL_ALGO


#include "math.h"
#include "util.h"


namespace psimpl {
    namespace detail
{
    /*!
        \brief Performs the nth point simplification routine (NP).
    */
    template
    <
        unsigned DIM,
        typename ForwardIterator,
        typename Size,
        typename OutputIterator
    >
    struct nth_point
    {
        typedef typename std::iterator_traits <ForwardIterator>::difference_type diff_type;

        static OutputIterator simplify (
            ForwardIterator first,
            ForwardIterator last,
            Size n,
            OutputIterator result)
        {
            diff_type coordCount = std::distance (first, last);
            diff_type pointCount = DIM              // protect against zero DIM
                                   ? coordCount / DIM
                                   : 0;

            // validate input and check if simplification required
            if (coordCount % DIM || pointCount < 3 || n < 2) {
                return std::copy (first, last, result);
            }

            unsigned remaining = pointCount - 1;    // the number of points remaining after key
            ForwardIterator key = first;              // indicates the current key

            // the first point is always part of the simplification
            util::copy_n (key, DIM, result);

            // copy each nth point
            while (util::forward (key, n, remaining)) {
                util::copy_n (key, result);
            }

            return result;
        }
    };

    /*!
        \brief Performs the (radial) distance simplification routine (RD).
    */
    template
    <
        unsigned DIM,
        typename ForwardIterator,
        typename Distance,
        typename OutputIterator
    >
    struct radial_distance
    {
        typedef typename std::iterator_traits <ForwardIterator>::difference_type diff_type;

        static OutputIterator simplify (
            ForwardIterator first,
            ForwardIterator last,
            Distance tol,
            OutputIterator result)
        {
            diff_type coordCount = std::distance (first, last);
            diff_type pointCount = DIM          // protect against zero DIM
                                   ? coordCount / DIM
                                   : 0;
            Distance tol2 = tol * tol;          // squared distance tolerance

            // validate input and check if simplification required
            if (coordCount % DIM || pointCount < 3 || tol2 == 0) {
                return std::copy (first, last, result);
            }

            ForwardIterator current = first;    // indicates the current key
            ForwardIterator next = first;       // used to find the next key

            // the first point is always part of the simplification
            util::copy_key (next, DIM, result);
            std::advance (next, DIM);

            // Skip first and last point, because they are always part of the simplification
            for (diff_type index = 1; index < pointCount - 1; ++index) {
                if (tol2 <= math::point_distance2 <DIM> (current, next)) {
                    current = next;
                    result = util::copy_n (next, DIM, result);
                }
                std::advance (next, DIM);
            }
            // the last point is always part of the simplification
            util::copy_key (next, DIM, result);
            return result;
        }
    };

    /*!
        \brief Performs the perpendicular distance simplification routine (PD).
    */
    template
    <
        unsigned DIM,
        typename ForwardIterator,
        typename Distance,
        typename OutputIterator
    >
    struct perpendicular_distance
    {
        typedef typename std::iterator_traits <ForwardIterator>::difference_type diff_type;
        typedef typename std::iterator_traits <ForwardIterator>::value_type value_type;
        typedef typename util::promote_to_floating_point <value_type>::type calc_type;

        static OutputIterator simplify (
            ForwardIterator first,
            ForwardIterator last,
            Distance tol,
            OutputIterator result)
        {
            diff_type coordCount = std::distance (first, last);
            diff_type pointCount = DIM      // protect against zero DIM
                                   ? coordCount / DIM
                                   : 0;
            Distance tol2 = tol * tol;      // squared distance tolerance

            // validate input and check if simplification required
            if (coordCount % DIM || pointCount < 3 || tol2 == 0) {
                return std::copy (first, last, result);
            }

            ForwardIterator p0 = first;
            ForwardIterator p1 = p0;
            std::advance (p1, DIM);
            ForwardIterator p2 = p1;
            std::advance (p2, DIM);

            // the first point is always part of the simplification
            util::copy_key (p0, DIM, result);

            while (p2 != last) {
                // test p1 against line segment S(p0, p2)
                if (math::segment_distance2 <DIM> (p0, p2, p1) < tol2) {
                    util::copy_key (p2, DIM, result);
                    // move up by two points
                    p0 = p2;
                    std::advance (p1, 2 * DIM);
                    if (p1 == last) {
                        // protect against advancing p2 beyond last
                        break;
                    }
                    std::advance (p2, 2 * DIM);
                }
                else {
                    util::copy_key (p1, DIM, result);
                    // move up by one point
                    p0 = p1;
                    p1 = p2;
                    std::advance (p2, DIM);
                }
            }
            // make sure the last point is part of the simplification
            if (p1 != last) {
                util::copy_key (p1, DIM, result);
            }
            return result;
        }
    };

    /*!
        \brief Repeatedly performs the perpendicular distance simplification routine (PD).
    */
    template
    <
        unsigned DIM,
        typename ForwardIterator,
        typename Distance,
        typename Size,
        typename OutputIterator
    >
    struct perpendicular_distance_repeat
    {
        typedef typename std::iterator_traits <ForwardIterator>::value_type value_type;
        typedef typename std::iterator_traits <ForwardIterator>::difference_type diff_type;

        static OutputIterator simplify (
            ForwardIterator first,
            ForwardIterator last,
            Distance tol,
            Size repeat,
            OutputIterator result)
        {
            if (repeat == 1) {
                // single pass
                return perpendicular_distance
                    <
                        DIM,
                        ForwardIterator,
                        Distance,
                        OutputIterator
                    >::simplify (first, last, tol, result);
            }
            // only validate repeat; other input is validated by simplify_perpendicular_distance
            if (repeat < 1) {
                return std::copy (first, last, result);
            }
            diff_type coordCount = std::distance (first, last);

            // first pass: [first, last) --> temporary array 'tempPoly'
            util::scoped_array <value_type> tempPoly (coordCount);

            diff_type tempCoordCount = std::distance (
                tempPoly.get (),
                perpendicular_distance
                    <
                        DIM,
                        ForwardIterator,
                        Distance,
                        value_type*
                    >::simplify (first, last, tol, tempPoly.get ()));

            // check if simplification did not improve
            if (coordCount == tempCoordCount) {
                return std::copy (tempPoly.get (), tempPoly.get () + coordCount, result);
            }
            std::swap (coordCount, tempCoordCount);
            --repeat;

            // intermediate passes: temporary array 'tempPoly' --> temporary array 'tempResult'
            if (1 < repeat) {
                util::scoped_array <value_type> tempResult (coordCount);

                while (--repeat) {
                    tempCoordCount = std::distance (
                        tempResult.get (),
                        perpendicular_distance
                            <
                                DIM,
                                value_type*,
                                Distance,
                                value_type*
                            >::simplify (tempPoly.get (), tempPoly.get () + coordCount, tol, tempResult.get ()));

                    // check if simplification did not improve
                    if (coordCount == tempCoordCount) {
                        return std::copy (tempPoly.get (), tempPoly.get () + coordCount, result);
                    }
                    util::swap (tempPoly, tempResult);
                    std::swap (coordCount, tempCoordCount);
                }
            }

            // final pass: temporary array 'tempPoly' --> result
            return perpendicular_distance
                <
                    DIM,
                    value_type*,
                    Distance,
                    OutputIterator
                >::simplify (tempPoly.get (), tempPoly.get () + coordCount, tol, result);
        }
    };

    /*!
        \brief Performs Reumann-Witkam approximation (RW).
    */
    template
    <
        unsigned DIM,
        typename ForwardIterator,
        typename Distance,
        typename OutputIterator
    >
    struct reumann_witkam
    {
        typedef typename std::iterator_traits <ForwardIterator>::difference_type diff_type;
        typedef typename std::iterator_traits <ForwardIterator>::value_type value_type;
        typedef typename util::promote_to_floating_point <value_type>::type calc_type;

        static OutputIterator simplify (
            ForwardIterator first,
            ForwardIterator last,
            Distance tol,
            OutputIterator result)
        {
            diff_type coordCount = std::distance (first, last);
            diff_type pointCount = DIM      // protect against zero DIM
                                   ? coordCount / DIM
                                   : 0;
            Distance tol2 = tol * tol;      // squared distance tolerance

            // validate input and check if simplification required
            if (coordCount % DIM || pointCount < 3 || tol2 == 0) {
                return std::copy (first, last, result);
            }

            // define the line L(p0, p1)
            ForwardIterator p0 = first; // indicates the current key
            ForwardIterator p1 = p0;    // indicates the next point after p0
            std::advance (p1, DIM);

            // keep track of two test points
            ForwardIterator pi = p1;    // the previous test point
            ForwardIterator pj = p1;    // the current test point (pi+1)

            // the first point is always part of the simplification
            util::copy_key (p0, DIM, result);

            // check each point pj against L(p0, p1)
            for (diff_type j = 2; j < pointCount; ++j) {
                pi = pj;
                std::advance (pj, DIM);

                if (math::line_distance2 <DIM> (p0, p1, pj) < tol2) {
                    continue;
                }
                // found the next key at pi
                util::copy_key (pi, DIM, result);
                // define new line L(pi, pj)
                p0 = pi;
                p1 = pj;
            }
            // the last point is always part of the simplification
            util::copy_key (pj, DIM, result);

            return result;
        }
    };

    /*!
        \brief Performs Opheim approximation (OP).
    */
    template
    <
        unsigned DIM,
        typename ForwardIterator,
        typename Distance,
        typename OutputIterator
    >
    struct opheim
    {
        typedef typename std::iterator_traits <ForwardIterator>::difference_type diff_type;
        typedef typename std::iterator_traits <ForwardIterator>::value_type value_type;
        typedef typename util::promote_to_floating_point <value_type>::type calc_type;

        static OutputIterator simplify (
            ForwardIterator first,
            ForwardIterator last,
            Distance min_tol,
            Distance max_tol,
            OutputIterator result)
        {
            diff_type coordCount = std::distance (first, last);
            diff_type pointCount = DIM              // protect against zero DIM
                                   ? coordCount / DIM
                                   : 0;
            Distance min_tol2 = min_tol * min_tol;  // squared minimum distance tolerance
            Distance max_tol2 = max_tol * max_tol;  // squared maximum distance tolerance

            // validate input and check if simplification required
            if (coordCount % DIM || pointCount < 3 || min_tol2 == 0 || max_tol2 == 0) {
                return std::copy (first, last, result);
            }

            // define the ray R(r0, r1)
            ForwardIterator r0 = first;  // indicates the current key and start of the ray
            ForwardIterator r1 = first;  // indicates a point on the ray
            bool rayDefined = false;

            // keep track of two test points
            ForwardIterator pi = r0;     // the previous test point
            ForwardIterator pj = pi;     // the current test point (pi+1)
            std::advance (pj, DIM);

            // the first point is always part of the simplification
            util::copy_key (r0, DIM, result);

            for (diff_type j = 2; j < pointCount; ++j) {
                pi = pj;
                std::advance (pj, DIM);

                if (!rayDefined) {
                    // discard each point within minimum tolerance
                    if (math::point_distance2 <DIM> (r0, pj) < min_tol2) {
                        continue;
                    }
                    // the last point within minimum tolerance pi defines the ray R(r0, r1)
                    r1 = pi;
                    rayDefined = true;
                }

                // check each point pj against R(r0, r1)
                if (math::point_distance2 <DIM> (r0, pj) < max_tol2 &&
                    math::ray_distance2 <DIM> (r0, r1, pj) < min_tol2)
                {
                    continue;
                }
                // found the next key at pi
                util::copy_key (pi, DIM, result);
                // define new ray R(pi, pj)
                r0 = pi;
                rayDefined = false;
            }
            // the last point is always part of the simplification
            util::copy_key (pj, DIM, result);

            return result;
        }
    };

    /*!
        \brief Performs Lang approximation (LA).
    */
    template
    <
        unsigned DIM,
        typename BidirectionalIterator,
        typename Distance,
        typename Size,
        typename OutputIterator
    >
    struct lang
    {
        typedef typename std::iterator_traits <BidirectionalIterator>::difference_type diff_type;
        typedef typename std::iterator_traits <BidirectionalIterator>::value_type value_type;
        typedef typename util::promote_to_floating_point <value_type>::type calc_type;

        static OutputIterator simplify (
            BidirectionalIterator first,
            BidirectionalIterator last,
            Distance tol,
            Size look_ahead,
            OutputIterator result)
        {
            diff_type coordCount = std::distance (first, last);
            diff_type pointCount = DIM              // protect against zero DIM
                                   ? coordCount / DIM
                                   : 0;
            Distance tol2 = tol * tol;              // squared minimum distance tolerance

            // validate input and check if simplification required
            if (coordCount % DIM || pointCount < 3 || look_ahead < 2 || tol2 == 0) {
                return std::copy (first, last, result);
            }

            BidirectionalIterator current = first;  // indicates the current key
            BidirectionalIterator next = first;     // used to find the next key

            Distance remaining = coordCount - DIM;    // the number of points remaining after current
            Distance moved = util::forward (next, look_ahead*DIM, remaining);

            // the first point is always part of the simplification
            util::copy_key (current, DIM, result);

            while (moved) {
                value_type d2 = 0;
                BidirectionalIterator p = current;
                std::advance (p, DIM);

                while (p != next) {
                    d2 = std::max (d2, math::segment_distance2 <DIM> (current, next, p));
                    if (tol2 < d2) {
                        break;
                    }
                    std::advance (p, DIM);
                }
                if (d2 < tol2) {
                    current = next;
                    util::copy_key (current, DIM, result);
                    moved = util::forward (next, look_ahead*DIM, remaining);
                }
                else {
                    util::backward (next, DIM, remaining);
                }
            }
            return result;
        }
    };

}}

#endif // PSIMPL_DETAIL_ALGO
