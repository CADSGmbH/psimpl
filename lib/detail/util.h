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

#ifndef PSIMPL_DETAIL_UTIL
#define PSIMPL_DETAIL_UTIL


#include <algorithm>


namespace psimpl {
    namespace util
{

    namespace detail
    {
        // ---------- copy_n -----------------------------------------------------------------------
        template
        <
            typename InputIterator,
            typename Size,
            typename OutputIterator
        >
        inline OutputIterator copy_n (
            InputIterator first,
            Size n,
            OutputIterator result,
            std::input_iterator_tag)
        {
            for (; n > 0; --n)
            {
                *result = *first;
                ++first;
                ++result;
            }
            return result;
        }

        template
        <
            typename InputIterator,
            typename Size,
            typename OutputIterator
        >
        inline OutputIterator copy_n (
            InputIterator first,
            Size n,
            OutputIterator result,
            std::random_access_iterator_tag)
        {
            return std::copy (first, first + n, result);
        }
    }

    // ---------------------------------------------------------------------------------------------

    /*!
        \brief Copies the range [first, first+n) to [result, result+n).

        \param[in] first    the first element to copy
        \param[in] n        the number of elements to copy
        \param[in] result   destination of the copied elements
        \return             one beyond the last copied element
    */
    template
    <
        typename InputIterator,
        typename Size,
        typename OutputIterator
    >
    inline OutputIterator copy_n (
        InputIterator first,
        Size n,
        OutputIterator result)
    {
        return detail::copy_n (
            first, n, result, std::iterator_traits <InputIterator>::iterator_category);
    }

    /*!
        \brief Copies the key from [first, first+n) to [result, result+n).

        \param[in]     first    the first coordinate of the key
        \param[in]     n        the number of coordinates to copy
        \param[in,out] result   destination of the copied coordinates
    */
    template
    <
        typename InputIterator,
        typename Size,
        typename OutputIterator
    >
    inline void copy_key (
        InputIterator first,
        Size n,
        OutputIterator& result)
    {
        result = copy_n (first, n, result);
    }

    /*!
        \brief Increments the iterator by n if possible.

        If there are fewer than n elements remaining the iterator will be incremented to the last
        element.

        \param[in,out] it           iterator to be advanced
        \param[in]     n            number of elements to advance
        \param[in,out] remaining    number of elements remaining after it
        \return                     the actual amount of elements that the iterator advanced
    */
    template
    <
        typename InputIterator,
        typename Distance
    >
    inline Distance forward (
        InputIterator& it,
        Distance n,
        Distance& remaining)
    {
        n = std::min (n, remaining);
        std::advance (it, n);
        remaining -= n;
        return n;
    }

    /*!
        \brief Decrements the iterator by n elements.

        \sa Forward

        \param[in,out] it           iterator to be advanced
        \param[in]     n            number of elements to advance
        \param[in,out] remaining    number of points remaining after it
    */
    template
    <
        typename InputIterator,
        typename Distance
    >
    inline void backward (
        InputIterator& it,
        Distance n,
        Distance& remaining)
    {
        std::advance (it, n);
        remaining += n;
    }

    // ---------------------------------------------------------------------------------------------

    /*!
        \brief A smart pointer for holding a dynamically allocated array.

        Similar to boost::scoped_array.
    */
    template <typename T>
    class scoped_array
    {
    public:
        scoped_array (unsigned n) {
            data = new T [n];
        }

        ~scoped_array () {
            delete [] data;
        }

        T& operator [] (int offset) {
            return data [offset];
        }

        const T& operator [] (int offset) const {
            return data [offset];
        }

        T* get () const {
            return data;
        }

        void swap (scoped_array& b) {
            T* tmp = b.data;
            b.data = data;
            data = tmp;
        }

    private:
        scoped_array (const scoped_array&);
        scoped_array& operator= (const scoped_array&);

    private:
        T* data;
    };

    template <typename T> inline void swap (scoped_array <T>& a, scoped_array <T>& b) {
        a.swap (b);
    }

    // ---------------------------------------------------------------------------------------------

    template <typename T>
    struct remove_const
    {
        typedef T type;
    };

    template <typename T>
    struct remove_const <const T>
    {
        typedef T type;
    };

    template <typename T>
    struct remove_volatile
    {
        typedef T type;
    };

    template <typename T>
    struct remove_volatile <volatile T>
    {
        typedef T type;
    };

    template <typename T>
    struct remove_cv
    {
        typedef typename remove_const <typename remove_volatile <T>::type>::type type;
    };

    template <typename T>
    struct promote_to_floating_point_helper
    {
        typedef T type;
    };

    template <typename T>
    struct promote_to_floating_point
    {
        typedef typename promote_to_floating_point_helper <typename remove_cv <T>::type>::type type;
    };

#define PSIMPL_DEF_FLOATING_POINT_PROMOTION (intType, fpType) \
    namespace util { \
        template <> \
        struct promote_to_floating_point_helper <intType> \
        { \
            typedef fpType type; \
        }; \
    } \

    template <typename Iterator>
    struct select_calculation_type
    {
        typedef typename promote_to_floating_point
                            <
                                typename std::iterator_traits <Iterator>::value_type
                            >::type type;
    };

}}


//        /*!
//            \brief Increments the iterator by n points.

//            \sa AdvanceCopy

//            \param[in,out] it  iterator to be advanced
//            \param[in]     n   number of points to advance
//        */
//        inline void Advance (
//            InputIterator& it,
//            diff_type n = 1)
//        {
//            std::advance (it, n * static_cast <diff_type> (DIM));
//        }

//        /*!
//            \brief Increments a copy of the iterator by n points.

//            \sa Advance

//            \param[in] it   iterator to be advanced
//            \param[in] n    number of points to advance
//            \return         an incremented copy of the input iterator
//        */
//        inline InputIterator AdvanceCopy (
//            InputIterator it,
//            diff_type n = 1)
//        {
//            Advance (it, n);
//            return it;
//        }


//        /*!
//            \brief Decrements the iterator by 1 point.

//            \sa Forward

//            \param[in,out] it            iterator to be advanced
//            \param[in,out] remaining     number of points remaining after it
//        */
//        inline void Backward (
//            InputIterator& it,
//            unsigned& remaining)
//        {
//            Advance (it, -1);
//            ++remaining;
//        }


#endif // PSIMPL_DETAIL_UTIL
