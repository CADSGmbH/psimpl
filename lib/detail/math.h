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

#ifndef PSIMPL_DETAIL_MATH
#define PSIMPL_DETAIL_MATH


#include <algorithm>
#include "util.h"


namespace psimpl {
    namespace math
{

    namespace detail
    {

    }

    // ---------------------------------------------------------------------------------------------

    /*!
        \brief Adds two points from each other.

        \param[in] p1       the first coordinate of the first point
        \param[in] p2       the first coordinate of the second point
        \param[in] result   the resulting vector (p1+p2)
    */
    template
    <
        unsigned DIM,
        typename InputIterator,
        typename RandomAccessIterator
    >
    inline void add (
        InputIterator p1,
        InputIterator p2,
        RandomAccessIterator result)
    {
        // todo candidate for optimization based on random iterator or pointer types
        for (unsigned d = 0; d < DIM; ++d, ++p1, ++p2) {
            result [d] = *p2 + *p1;
        }
    }

    /*!
        \brief Subtracts two points from each other.

        \param[in] p1       the first coordinate of the first point
        \param[in] p2       the first coordinate of the second point
        \param[in] result   the resulting vector (p2-p1)
    */
    template
    <
        unsigned DIM,
        typename InputIterator,
        typename RandomAccessIterator
    >
    inline void subtract (
        InputIterator p1,
        InputIterator p2,
        RandomAccessIterator result)
    {
        // todo candidate for optimization based on random iterator or pointer types
        for (unsigned d = 0; d < DIM; ++d, ++p1, ++p2) {
            result [d] = *p2 - *p1;
        }
    }

    /*!
        \brief Multiplies a point with a value.

        \param[in] p    the first coordinate of the first point
        \param[in] v    the value
    */
    template
    <
        unsigned DIM,
        typename RandomAccessIterator
    >
    inline void multiply (
        RandomAccessIterator p,
        typename std::iterator_traits <RandomAccessIterator>::value_type v)
    {
        // todo candidate for optimization using compile time loop unrolling
        for (unsigned d = 0; d < DIM; ++d) {
            p [d] *= v;
        }
    }

    /*!
        \brief Computes the dot product of two vectors.

        \param[in] v1   the first coordinate of the first vector
        \param[in] v2   the first coordinate of the second vector
        \return         the dot product (v1 * v2)
    */
    template
    <
        unsigned DIM,
        typename InputIterator
    >
    inline typename std::iterator_traits <InputIterator>::value_type dot (
        InputIterator v1,
        InputIterator v2)
    {
        // todo candidate for optimization based on random iterator or pointer types
        typename std::iterator_traits <InputIterator>::value_type result ();
        for (unsigned d = 0; d < DIM; ++d, ++v1, ++v2) {
            result += (*v1) * (*v2);
        }
        return result;
    }

    /*!
        \brief Computes the squared distance of two points

        \param[in] p1   the first coordinate of the first point
        \param[in] p2   the first coordinate of the second point
        \return         the squared distance
    */
    template
    <
        unsigned DIM,
        typename InputIterator1,
        typename InputIterator2
    >
    inline typename std::iterator_traits <InputIterator1>::value_type point_distance2 (
        InputIterator1 p1,
        InputIterator2 p2)
    {
        // todo candidate for optimization based on random iterator or pointer types
        // todo test with dist(float, int) and vice versa
        typename std::iterator_traits <InputIterator1>::value_type result ();

        for (unsigned d = 0; d < DIM; ++d, ++p1, ++p2) {
            result += (*p1 - *p2) * (*p1 - *p2);
        }
        return result;
    }

    /*!
        \brief Computes the squared distance between an infinite line (l1, l2) and a point p

        \param[in] l1   the first coordinate of the first point on the line
        \param[in] l2   the first coordinate of the second point on the line
        \param[in] p    the first coordinate of the test point
        \return         the squared distance
    */
    template
    <
        unsigned DIM,
        typename InputIterator
    >
    typename util::select_calculation_type <InputIterator>::type line_distance2 (
        InputIterator l1,
        InputIterator l2,
        InputIterator p)
    {
        typedef typename util::select_calculation_type <InputIterator>::type calc_type;

        calc_type v [DIM];                 // vector l1 --> l2
        calc_type w [DIM];                 // vector l1 --> p

        subtract <DIM> (l1, l2, v);
        subtract <DIM> (l1, p,  w);

        calc_type cv = dot <DIM> (v, v);   // squared length of v
        calc_type cw = dot <DIM> (w, v);   // project w onto v

        // avoid problems with divisions when calc_type is an integer type
        calc_type fraction = cv == 0 ? 0 : cw / cv;
        multiply <DIM> (v, fraction);

        calc_type proj [DIM];    // p projected onto segement (s1, s2)
        add <DIM> (l1, v, proj);

        return point_distance2 <DIM> (proj, p);
    }

    /*!
        \brief Computes the squared distance between a ray (r1, r2) and a point p

        \param[in] r1   the first coordinate of the start point of the ray
        \param[in] r2   the first coordinate of a point on the ray
        \param[in] p    the first coordinate of the test point
        \return         the squared distance
    */
    template
    <
        unsigned DIM,
        typename InputIterator
    >
    typename util::select_calculation_type <InputIterator>::type ray_distance2 (
        InputIterator r1,
        InputIterator r2,
        InputIterator p)
    {
        typedef typename util::select_calculation_type <InputIterator>::type calc_type;

        calc_type v [DIM];        // vector r1 --> r2
        calc_type w [DIM];        // vector r1 --> p

        subtract <DIM> (r1, r2, v);
        subtract <DIM> (r1, p,  w);

        calc_type cv = dot <DIM> (v, v);    // squared length of v
        calc_type cw = dot <DIM> (w, v);    // project w onto v

        if (cw <= 0) {
            // projection of w lies to the left of r1 (not on the ray)
            return point_distance2 <DIM> (p, r1);
        }

        // avoid problems with divisions when calc_type is an integer type
        calc_type fraction = cv == 0 ? 0 : cw / cv;
        multiply <DIM> (v, fraction);

        calc_type proj [DIM];    // p projected onto segement (s1, s2)
        add <DIM> (r1, v, proj);

        return point_distance2 <DIM> (proj, p);
    }

    /*!
        \brief Computes the squared distance between a line segment (s1, s2) and a point p

        \param[in] s1   the first coordinate of the start point of the segment
        \param[in] s2   the first coordinate of the end point of the segment
        \param[in] p    the first coordinate of the test point
        \return         the squared distance
    */
    template
    <
        unsigned DIM,
        typename InputIterator
    >
    typename util::select_calculation_type <InputIterator>::type segment_distance2 (
        InputIterator s1,
        InputIterator s2,
        InputIterator p)
    {
        typedef typename util::select_calculation_type <InputIterator>::type calc_type;

        calc_type v [DIM];        // vector s1 --> s2
        calc_type w [DIM];        // vector s1 --> p

        subtract <DIM> (s1, s2, v);
        subtract <DIM> (s1, p,  w);

        calc_type cw = dot <DIM> (w, v);   // project w onto v
        if (cw <= 0) {
            // projection of w lies to the left of s1
            return point_distance2 <DIM> (p, s1);
        }

        calc_type cv = dot <DIM> (v, v);   // squared length of v
        if (cv <= cw) {
            // projection of w lies to the right of s2
            return point_distance2 <DIM> (p, s2);
        }

        // avoid problems with divisions when calc_type is an integral type
        calc_type fraction = cv == 0 ? 0 : cw / cv;
        multiply <DIM> (v, fraction);

        calc_type proj [DIM];    // p projected onto segement (s1, s2)
        add <DIM> (s1, v, proj);

        return point_distance2 <DIM> (proj, p);
    }

}}

#endif // PSIMPL_DETAIL_MATH
