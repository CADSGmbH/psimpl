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


#ifndef PSIMPL_HELPER
#define PSIMPL_HELPER


#include <vector>
#include <cmath>


namespace psimpl {
    namespace test
{
    static float float_epsilon = 0.001f;
    static double double_epsilon = 0.00001;

    template <typename T, unsigned DIM>
    class StraightLine {
    public:
        StraightLine () :
            mCoord (0),
            mDimension (0)
        {}
        
        T operator () () {
            mDimension = mDimension % DIM;
            T value = 0;
            if (mDimension == 0) {
                value = mCoord;
                mCoord += 1;
            }
            ++mDimension;
            return value;
        }
        
    private:
        T mCoord;
        unsigned mDimension;
    };

    template <typename T, unsigned DIM>
    class SawToothLine {
    public:
        SawToothLine () :
            mCoord (0),
            mToothSize (0),
            mDimension (0),
            mToggle (true)
        {}
        
        T operator () () {
            mDimension = mDimension % DIM;
            T value = 0;
            if (mDimension == 0) {
                mToggle = !mToggle;
                value = mCoord;
                mCoord += 1;
            }
            else if (mToggle && mDimension == 1) {
                mToothSize += 1;
                value = mToothSize;
            }
            ++mDimension;
            return value;
        }
        
    private:
        T mCoord;
        T mToothSize;
        unsigned mDimension;
        bool mToggle;
    };

    template <class T>
    inline bool CompareValue (T a, T b) {
        return a == b;
    }

    inline bool CompareValue (float a, float b) {
        return fabs (a - b) < float_epsilon;
    }

    inline bool CompareValue (double a, double b) {
        return fabs (a - b) < double_epsilon;
    }

    template <unsigned DIM, class InputIterator1, class InputIterator2>
    bool ComparePoint (InputIterator1 p1, InputIterator2 p2) {
        for (unsigned d=0; d <DIM; ++d) {
            if (!CompareValue (*p1, *p2)) {
                return false;
            }
            ++p1;
            ++p2;
        }
        return true;
    }

    template <unsigned DIM, class InputIterator1, class InputIterator2>
    bool CompareEndPoints (InputIterator1 polyline_begin, InputIterator1 polyline_end, InputIterator2 simplification_begin, InputIterator1 simplification_end) {
        // compare first point
        if (!ComparePoint <DIM> (polyline_begin, simplification_begin)) {
            return false;
        }

        // compare last point
        std::advance (polyline_begin, std::distance (polyline_begin, polyline_end) - DIM);
        std::advance (simplification_begin, std::distance (simplification_begin, simplification_end) - DIM);

        return ComparePoint <DIM> (polyline_begin, simplification_begin);
    }

    template <unsigned DIM, class InputIterator1, class InputIterator2>
    bool ComparePoints(InputIterator1 polyline, InputIterator2 simplification, const std::vector<int>& keys) {
        for (size_t i=0; i<keys.size (); ++i) {
            InputIterator1 it = polyline;
            std::advance (it, keys [i] * DIM);
            if (!ComparePoint <DIM> (it, simplification)) {
                return false;
            }
            std::advance (simplification, DIM);
        }
        return true;
    }

}}

#endif // PSIMPL_HELPER
