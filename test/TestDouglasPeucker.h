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


#ifndef PSIMPL_TEST_DOUGLAS_PEUCKER
#define PSIMPL_TEST_DOUGLAS_PEUCKER


#include "test.h"


namespace psimpl {
    namespace test
{
    //! Tests function psimpl::simplify_douglas_peucker
    class TestDouglasPeucker
    {
    public:
        TestDouglasPeucker ();

    private:
        void TestIncompletePoint ();
        void TestNotEnoughPoints ();
        void TestInvalidTol ();
        void TestValidTol ();
        void TestBasicSanity ();
        void TestRandomIterator ();
        void TestBidirectionalIterator ();
        void TestForwardIterator ();
        void TestReturnValue ();
    };

    //! Tests function psimpl::simplify_douglas_peucker_n
    class TestDouglasPeuckerN
    {
    public:
        TestDouglasPeuckerN ();

    private:
        void TestIncompletePoint ();
        void TestNotEnoughPoints ();
        void TestInvalidTol ();
        void TestValidTol ();
        void TestBasicSanity ();
        void TestRandomIterator ();
        void TestBidirectionalIterator ();
        void TestForwardIterator ();
        void TestReturnValue ();
    };
}}


#endif // PSIMPL_TEST_DOUGLAS_PEUCKER