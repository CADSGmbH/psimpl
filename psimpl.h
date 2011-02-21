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
 * The Original Code is 'psimpl - generic n-dimensional polyline simplification'.
 *
 * The Initial Developer of the Original Code is
 * Elmar de Koning.
 * Portions created by the Initial Developer are Copyright (C) 2010
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * ***** END LICENSE BLOCK ***** */
 
/*!
    psimpl - generic n-dimensional polyline simplification
    Copyright (C) 2010 Elmar de Koning, edekoning@gmail.com

    This file is part of psimpl and was originally released as part
    of the article 'Polyline Simplification' at The Code Project:
    http://www.codeproject.com/KB/recipes/PolylineSimplification.aspx

    28-09-2010 - Initial version
    23-10-2010 - Changed license from CPOL to MPL
*/

#ifndef PSIMPL_GENERIC
#define PSIMPL_GENERIC


#include <queue>
#include <stack>


namespace psimpl
{
    /*!
        \brief Minimal std::auto_ptr for C-style arrays

        Ownership of the array cannot be changed, reset, or transferred in any way. 
    */
    template <typename T>
    class array_ptr
    {
    public:
        array_ptr (unsigned n) {
            array = new T [n];
        }

        ~array_ptr () {
            delete [] array;
        }

        T& operator [] (int offset) {
            return array [offset];
        }

        const T& operator [] (int offset) const {
            return array [offset];
        }

        T* get () const {
            return array;
        }
    
    private:
        array_ptr (const array_ptr&);
        array_ptr& operator= (const array_ptr&);

    private:
        T* array;
    };

    /*!
        \brief Provides various simplification algorithms for n-dimensional simple polylines.

        A polyline is simple when it is non-closed and non-selfintersecting. Currently
        'vertex reduction' and 'Douglas-Peucker appromixation' are provided. All
        algorithms operate on input iterators and output iterators. Note that unisgned
        integer types are NOT supported.

        \sa
            VertexReduction
            DPApproximation
    */
    template <unsigned DIM, class InputIterator, class OutputIterator>
    class PolylineSimplification
    {
        typedef typename std::iterator_traits <InputIterator>::difference_type diff_type;
        typedef typename std::iterator_traits <InputIterator>::value_type value_type;
        typedef typename std::iterator_traits <const value_type*>::difference_type ptr_diff_type;

    public:
        /*!
            \brief Performs vertex reduction.

            Vertex reduction is a brute-force O(n) algorithm for polyline simplification. It
            reduces successive vertices that are clustered too closely to a single vertex,
            called a key. The resulting keys form the simplified polyline.
            
            \param[in] first    the first coordinate of the first polyline point
            \param[in] last     one beyond the last coordinate of the last polyline point
            \param[in] tol      maximum allowed distance of any point to the simplified polyline
            \param[out] result  destination of the simplified polyline
            \return             one beyond beyond the last coordinate of the simplified polyline
            
            \sa
                VertexReduce
        */
        OutputIterator VertexReduction (
                InputIterator first,
                InputIterator last,
                value_type tol,
                OutputIterator result)
        {
            value_type tol2 = tol * tol;                            // squared distance tolerance
            diff_type count = std::distance (first, last) / DIM;    // point count

            // validate input
            if (count < 2 || tol2 == 0) {
                return result;
            }

            InputIterator current = first;  // indicates the current key
            InputIterator next = first;     // used to find the next key

            // the first point is always part of the simplification
            CopyKey (first, result);

            // Skip first and last point, beacuse they are always part of the simplification
            for (diff_type index = 1; index < count - 1; ++index) {
                std::advance (next, DIM);
                if (Distance2 (current, next) < tol2) {
                    continue;
                }
                current = next;
                CopyKey (current, result);
            }
            // the last point is always part of the simplification
            std::advance (next, DIM);
            CopyKey (next, result);

            return result;
        }

        /*!
            \brief Performs Douglas-Peucker approximation.

            The approximation algorithm uses vertex reduction O(n) as a preprocessing step.
            After vertex reduction the algorithm is O (n m) in worst case and O(n log m) on
            average, where m < n (m is the number of points after vertex reduction).
            
            The Douglas-Peucker algorithm starts with a simplification that is the single edge
            joining the first and last vertices of the polyline. The distance of the remaining
            vertices to that edge are computed. The vertex that is furthest away from the
            edge (called a key), and has a computed distance that is larger than a specified
            tolerance, will be added to the simplification. This process will recurse for each
            edge in the current simplification, untill all vertices of the original polyline are
            within tolerance.

            Note that this algorithm will create a copy of the input polyline during the vertex
            reduction step.

            \param[in] first    the first coordinate of the first polyline point
            \param[in] last     one beyond the last coordinate of the last polyline point
            \param[in] tol      maximum allowed distance of any point to the simplified polyline
            \param[out] result  destination of the simplified polyline
            \return             one beyond beyond the last coordinate of the simplified polyline
           
            \sa
                DPApproximationAlt
                VertexReduction
                DouglasPeuckerApproximate
        */
        OutputIterator DPApproximation (
                InputIterator first,
                InputIterator last,
                value_type tol,
                OutputIterator result)
        {
            diff_type coordCount = std::distance (first, last);
            diff_type pointCount = coordCount / DIM;

            // validate input
            if (pointCount < 2 || tol == 0) {
                return result;
            }
            // vertex reduction
            array_ptr <value_type> reduced (coordCount);    // vertex reduction results
            PolylineSimplification <DIM, InputIterator, value_type*> ps;
            ptr_diff_type reducedCoordCount =
                    std::distance (reduced.get (), ps.VertexReduction (first, last, tol, reduced.get ()));
            ptr_diff_type reducedPointCount = reducedCoordCount / DIM;

            // douglas-peucker approximation
            array_ptr <unsigned char> keys (pointCount);    // douglas-peucker results
            DPHelper::Approximate (reduced.get (), reducedCoordCount, tol, keys.get ());

            // copy all keys
            const value_type* current = reduced.get ();
            for (ptr_diff_type p=0; p<reducedPointCount; ++p, current += DIM) {
                if (keys [p]) {
                    for (unsigned d = 0; d < DIM; ++d) {
                        *result++ = current [d];
                    }
                }
            }
            return result;
        }

        /*!
            \brief Performs Douglas-Peucker approximation.

            This algorithm is a variation of the original implementation. Instead of considering
            one polyline segment at a time, all segments of the current simplified polyline are 
            evaluated at each step. Only the vertex with the maximum distance from its edge is
            added to the simplification. This process will recurse untill the the simplification
            contains the desired amount of vertices.
            
            The algorithm, which does not use vertex reduction as a preprocessing step, is O(n2)
            in worst case and O(n log n) on average.

            Note that this algorithm will create a copy of the input polyline for performance
            reasons. 
            
            \param[in] first    the first coordinate of the first polyline point
            \param[in] last     one beyond the last coordinate of the last polyline point
            \param[in] count    the maximum number of points of the simplified polyline
            \param[out] result  destination of the simplified polyline
            \return             one beyond beyond the last coordinate of the simplified polyline
           
            \sa
                DPApproximation
                DouglasPeuckerApproximateAlt
        */
        OutputIterator DPApproximationAlt (
                InputIterator first,
                InputIterator last,
                unsigned count,
                OutputIterator result)
        {
            diff_type coordCount = std::distance (first, last);
            diff_type pointCount = coordCount / DIM;

            // validate input
            if (count < 2 || pointCount < static_cast <diff_type> (count)) {
                return result;
            }

            // copy coords
            array_ptr <value_type> coords (coordCount);
            for (ptr_diff_type c=0; c<coordCount; ++c) {
                coords [c] = *first++;
            }

            // douglas-peucker approximation
            array_ptr <unsigned char> keys (pointCount);    // douglas-peucker results
            DPHelper::ApproximateAlt (coords.get (), coordCount, count, keys.get ());

            // copy keys
            const value_type* current = coords.get ();
            for (ptr_diff_type p=0; p<pointCount; ++p, current += DIM) {
                if (keys [p]) {
                    for (unsigned d = 0; d < DIM; ++d) {
                        *result++ = current [d];
                    }
                }
            }
            return result;
        }

    private:
        /*!
            \brief Copies a key to the output destination.

            \param[in] key      the first coordinate of the key
            \param[out] result  destination of the copied key
            \return             one beyond beyond the last coordinate of the copied key
        */
        inline OutputIterator CopyKey (
            InputIterator key,
            OutputIterator& result)
        {
            for (unsigned d = 0; d < DIM; ++d) {
                *result++ = *key++;
            }
            return result;
        }

        /*!
            \brief Computes the squared distance of two points

            \param[in] p1   the first coordinate of the first point
            \param[in] p2   the first coordinate of the second point
            \return         the squared distance
        */
        inline value_type Distance2 (
            InputIterator p1,
            InputIterator p2)
        {
            value_type result = 0;
            for (unsigned d = 0; d < DIM; ++d, ++p1, ++p2) {
                result += (*p1 - *p2) * (*p1 - *p2);
            }
            return result;
        }

    private:
        /*!
            \brief Douglas-Peucker approximation helper class.
            
            Contains implentations for Douglas-Peucker approximation that operate solely on
            value_type arrays and value_type pointers. Note that the PolylineSimplification
            class only operates on iterators.

            This class was introduced to prevent problems with functions that differentiate only
            in their argument types, f.e.: func(InputIterator) vs func(const value_type*)
        */
        class DPHelper
        {
            //! \brief Defines a sub polyline.
            struct SubPoly {
                SubPoly (ptr_diff_type first=0, ptr_diff_type last=0) :
                    first (first), last (last) {}

                ptr_diff_type first;    //! coord index of the first point
                ptr_diff_type last;     //! coord index of the last point
            };

            //! \brief Defines the key of a polyline.
            struct KeyInfo {
                KeyInfo (ptr_diff_type index=0, value_type dist2=0) :
                    index (index), dist2 (dist2) {}

                ptr_diff_type index;    //! coord index of the key
                value_type dist2;       //! squared distance of the key to a segment
            };

            //! \brief Defines a sub polyline including its key.
            struct SubPolyAlt {
                SubPolyAlt (ptr_diff_type first=0, ptr_diff_type last=0) :
                    first (first), last (last) {}

                ptr_diff_type first;    //! coord index of the first point
                ptr_diff_type last;     //! coord index of the last point
                KeyInfo keyInfo;        //! key of this sub poly

                bool operator< (const SubPolyAlt& other) const {
                    return keyInfo.dist2 < other.keyInfo.dist2;
                }
            };

        public:
            /*!
                \brief Performs Douglas-Peucker approximation.

                \param[in] coords       array of polyline coordinates
                \param[in] coordCount   number of coordinates in coords []
                \param[in] tol          approximation tolerance
                \param[out] keys        indicates for each polyline point if it is a key

                \sa
                    ApproximateAlt
                    PolylineSimplification::DPApproximation
            */
            static void Approximate (
                    const value_type* coords,
                    ptr_diff_type coordCount,
                    value_type tol,
                    unsigned char* keys)
            {
                value_type tol2 = tol * tol;    // squared distance tolerance
                ptr_diff_type pointCount = coordCount / DIM;

                memset (keys, 0, pointCount);   // zero out keys
                keys [0] = 1;                   // the first point is always a key
                keys [pointCount - 1] = 1;      // the last point is always a key

                typedef std::stack <SubPoly> Stack;
                Stack stack;                    // LIFO job-queue containing sub-polylines

                SubPoly subPoly (0, coordCount-DIM);
                stack.push (subPoly);           // add complete poly

                while (!stack.empty ()) {
                    subPoly = stack.top ();     // take a sub poly
                    stack.pop ();               // and find its key
                    KeyInfo keyInfo = FindKey (coords, subPoly.first, subPoly.last);
                    if (keyInfo.index != subPoly.last && tol2 < keyInfo.dist2) {
                        // store the key if valid
                        keys [keyInfo.index / DIM] = 1;
                        // split the polyline at the key and recurse
                        stack.push (SubPoly (keyInfo.index, subPoly.last));
                        stack.push (SubPoly (subPoly.first, keyInfo.index));
                    }
                }
            }

            /*!
                \brief Performs Douglas-Peucker approximation.

                \param[in] coords       array of polyline coordinates
                \param[in] coordCount   number of coordinates in coords []
                \param[in] countTol     point count tolerance
                \param[out] keys        indicates for each polyline point if it is a key

                \sa
                    ApproximateAlt
                    PolylineSimplification::DPApproximation
            */
            static void ApproximateAlt (
                    const value_type* coords,
                    ptr_diff_type coordCount,
                    unsigned countTol,
                    unsigned char* keys)
            {
                ptr_diff_type pointCount = coordCount / DIM;

                memset (keys, 0, pointCount);   // zero out keys
                keys [0] = 1;                   // the first point is always a key
                keys [pointCount - 1] = 1;      // the last point is always a key
                unsigned keyCount = 2;

                if (countTol == 2) {
                    return;
                }

                typedef std::priority_queue <SubPolyAlt> PriorityQueue;
                PriorityQueue queue;    // sorted (max dist2) job queue containing sub-polylines

                SubPolyAlt subPoly (0, coordCount-DIM);
                subPoly.keyInfo = FindKey (coords, subPoly.first, subPoly.last);
                queue.push (subPoly);           // add complete poly

                while (!queue.empty ()) {
                    subPoly = queue.top ();     // take a sub poly
                    queue.pop ();
                    if (subPoly.keyInfo.index != subPoly.last) {
                        // store the key if valid
                        keys [subPoly.keyInfo.index / DIM] = 1;
                        // check point count tolerance
                        keyCount++;
                        if (keyCount == countTol) {
                            break;
                        }
                        // split the polyline at the key and recurse
                        SubPolyAlt left (subPoly.first, subPoly.keyInfo.index);
                        left.keyInfo = FindKey (coords, left.first, left.last);
                        queue.push (left);
                        SubPolyAlt right (subPoly.keyInfo.index, subPoly.last);
                        right.keyInfo = FindKey (coords, right.first, right.last);
                        queue.push (right);
                    }
                }
            }

        private:
            /*!
                \brief Finds the key for the given sub polyline.
                
                Finds the point in the range [first, last] that is furthest away from the
                segment (first, last). This point is called the key.
                
                \param[in] coords   array of polyline coordinates
                \param[in] first    the first coordinate of the first polyline point
                \param[in] last     the first coordinate of the last polyline point
                \return             the index of the key and its distance, or last when a key
                                    could not be found
            */
            static KeyInfo FindKey (
                    const value_type* coords,
                    ptr_diff_type first,
                    ptr_diff_type last)
            {
                KeyInfo keyInfo;

                value_type u [DIM];             // segment (first, last)
                value_type w [DIM];             // segment (first, current)
                value_type p [DIM];             // w projected onto u

                MakeVector (coords + first, coords + last, u);
                value_type cu = Dot (u, u);     // squared length of segment u
                value_type dv2 = 0;             // squared distance of current point to u

                for (ptr_diff_type current = first + DIM; current < last; current += DIM) {
                    MakeVector (coords + first, coords + current, w);
                    // calculate squared distance between w and u (project w onto u)
                    value_type cw = Dot (w, u);

                    if (cw <= 0) {
                        // projection of w lies to the left of (first)
                        dv2 = Distance2 (coords + current, coords + first);
                    }
                    else if (cu <= cw) {
                        // projection of w lies to the right of (last)
                        dv2 = Distance2 (coords + current, coords + last);
                    }
                    else {
                        // projection of w lies between (first) and (last)
                        // float cast is used to prevent problems with integer types
                        float fraction = static_cast <float> (cw) / static_cast <float> (cu);
                        Interpolate (coords + first, coords + last, fraction, p);
                        dv2 = Distance2 (coords + current, p);
                    }
                    if (dv2 < keyInfo.dist2) {
                        continue;
                    }
                    // update maximum squared distance and the point it belongs to
                    keyInfo.index = current;
                    keyInfo.dist2 = dv2;
                }
                return keyInfo;
            }

            /*!
                \brief Creates a vector from p1 to p2.
                
                \param[in] p1       the first coordinate of the first point
                \param[in] p2       the first coordinate of the second point
                \param[out] vector  the resulting vector
            */
            static inline void MakeVector (
                const value_type* p1,
                const value_type* p2,
                value_type* vector)
            {
                for (unsigned d = 0; d < DIM; ++d) {
                    vector [d] = p2 [d] - p1 [d];
                }
            }

            /*!
                \brief Computes the squared distance of two points.

                \param[in] p1   the first coordinate of the first point
                \param[in] p2   the first coordinate of the second point
                \return         the squared distance
            */
            static inline value_type Distance2 (
                const value_type* p1,
                const value_type* p2)
            {
                value_type result = 0;
                for (unsigned d = 0; d < DIM; ++d) {
                    result += (p1[d] - p2[d]) * (p1[d] - p2[d]);
                }
                return result;
            }

            /*!
                \brief Computes the dot product of two vectors.

                \param[in] v1   the first coordinate of the first vector
                \param[in] v2   the first coordinate of the second vector
                \return         the dot product
            */
            static inline value_type Dot (
                const value_type* v1,
                const value_type* v2)
            {
                value_type result = 0;
                for (unsigned d = 0; d < DIM; ++d) {
                    result += v1 [d] * v2 [d];
                }
                return result;
            }

            /*!
                \brief Peforms linear interpolation between two given points.

                \param[in] p1           the first coordinate of the first point
                \param[in] p2           the first coordinate of the second point
                \param[in] fraction     the fraction used during interpolation
                \param[out] result      the interpolation result
            */
            static inline void Interpolate (
                const value_type* p1,
                const value_type* p2,
                float fraction,
                value_type* result)
            {
                for (unsigned d = 0; d < DIM; ++d) {
                    result [d] = p1 [d] + (p2 [d] - p1 [d]) * fraction;
                }
            }
        };
    };

    /*!
        \brief Performs vertex reduction
        
        This is a convenience function that provides template type deduction for 
        PolylineSimplification::VertexReduction.

        \param[in] first    the first coordinate of the first polyline point
        \param[in] last     one beyond the last coordinate of the last polyline point
        \param[in] tol      maximum allowed distance of any point to the simplified polyline
        \param[out] result  destination of the simplified polyline
        \return             one beyond beyond the last coordinate of the simplified polyline
    */
    template <unsigned DIM, class InputIterator, class OutputIterator>
    OutputIterator VertexReduce (
            InputIterator first,
            InputIterator last,
            typename std::iterator_traits <InputIterator>::value_type tol,
            OutputIterator result)
    {
        PolylineSimplification <DIM, InputIterator, OutputIterator> ps;
        return ps.VertexReduction (first, last, tol, result);
    }

    /*!
        \brief Performs Douglas-Peucker approximation
        
        This is a convenience function that provides template type deduction for 
        PolylineSimplification::DPApproximation.

        \param[in] first    the first coordinate of the first polyline point
        \param[in] last     one beyond the last coordinate of the last polyline point
        \param[in] tol      maximum allowed distance of any point to the simplified polyline
        \param[out] result  destination of the simplified polyline
        \return             one beyond beyond the last coordinate of the simplified polyline
    */
    template <unsigned DIM, class InputIterator, class OutputIterator>
    OutputIterator DouglasPeuckerApproximate (
            InputIterator first,
            InputIterator last,
            typename std::iterator_traits <InputIterator>::value_type tol,
            OutputIterator result)
    {
        PolylineSimplification <DIM, InputIterator, OutputIterator> ps;
        return ps.DPApproximation (first, last, tol, result);
    }

    /*!
        \brief Performs Douglas-Peucker approximation

        This is a convenience function that provides template type deduction for 
        PolylineSimplification::DPApproximationAlt.

        \param[in] first    the first coordinate of the first polyline point
        \param[in] last     one beyond the last coordinate of the last polyline point
        \param[in] count    the maximum number of points of the simplified polyline
        \param[out] result  destination of the simplified polyline
        \return             one beyond beyond the last coordinate of the simplified polyline
    */
    template <unsigned DIM, class InputIterator, class OutputIterator>
    OutputIterator DouglasPeuckerApproximateAlt (
            InputIterator first,
            InputIterator last,
            unsigned count,
            OutputIterator result)
    {
        PolylineSimplification <DIM, InputIterator, OutputIterator> ps;
        return ps.DPApproximationAlt (first, last, count, result);
    }
}

#endif // PSIMPL_GENERIC
