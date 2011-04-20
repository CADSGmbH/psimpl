#ifndef PSIMPL_HELPER
#define PSIMPL_HELPER


#include <vector>


namespace psimpl {
    namespace test
{

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


    template <unsigned DIM, class InputIterator1, class InputIterator2>
    bool ComparePoint (InputIterator1 p1, InputIterator2 p2) {
        for (unsigned d=0; d <DIM; ++d) {
            if (*p1 != *p2) {
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
        for (int i=0; i<keys.size (); ++i) {
            InputIterator1 it = polyline;
            std::advance (it, keys [i] * DIM);
            if (!ComparePoint <DIM> (it, simplification)) {
                return false;
            }
            ++simplification;
        }
        return true;
    }

}}

#endif // PSIMPL_HELPER
