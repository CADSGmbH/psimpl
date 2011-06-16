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

#ifndef PSIMPL_TEST_MATH
#define PSIMPL_TEST_MATH


namespace psimpl {
    namespace test
{
    class TestMath
    {
    public:
        TestMath ();

    private:
        void TestEqual_RandomIterator ();
        void TestEqual_BidirectionalIterator ();
        void TestEqual_ForwardIterator ();

        void TestMakeVector_RandomIterator ();
        void TestMakeVector_BidirectionalIterator ();
        void TestMakeVector_ForwardIterator ();

        void TestDot_RandomIterator ();
        void TestDot_BidirectionalIterator ();
        void TestDot_ForwardIterator ();

        void TestInterpolate_Fractions ();
        void TestInterpolate_RandomIterator ();
        void TestInterpolate_BidirectionalIterator ();
        void TestInterpolate_ForwardIterator ();

        void TestPointDistance ();
        void TestPointDistance_RandomIterator ();
        void TestPointDistance_BidirectionalIterator ();
        void TestPointDistance_ForwardIterator ();

        void TestLineDistance ();
        void TestLineDistance_RandomIterator ();
        void TestLineDistance_BidirectionalIterator ();
        void TestLineDistance_ForwardIterator ();

        void TestSegmentDistance ();
        void TestSegmentDistance_RandomIterator ();
        void TestSegmentDistance_BidirectionalIterator ();
        void TestSegmentDistance_ForwardIterator ();

        void TestRayDistance ();
        void TestRayDistance_RandomIterator ();
        void TestRayDistance_BidirectionalIterator ();
        void TestRayDistance_ForwardIterator ();
    };
}}


#endif // PSIMPL_TEST_MATH
