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

#include "TestUtil.h"
#include "test.h"
#include "../lib/psimpl.h"


namespace psimpl {
    namespace test
{
    TestUtil::TestUtil () {
        TEST_RUN("scoped_array", TestScopedArray ());
    }

    void TestUtil::TestScopedArray () {
        // construction
        psimpl::util::scoped_array <float> a1 (5);
        ASSERT_TRUE(a1.get ());

        // setting / getting values
        a1 [0] = 0.f; ASSERT_TRUE(a1 [0] == 0.f);
        a1 [1] = 1.f; ASSERT_TRUE(a1 [1] == 1.f);
        a1 [2] = 2.f; ASSERT_TRUE(a1 [2] == 2.f);
        a1 [3] = 3.f; ASSERT_TRUE(a1 [3] == 3.f);
        a1 [4] = 4.f; ASSERT_TRUE(a1 [4] == 4.f);

        // const access
        const psimpl::util::scoped_array <float>& a2 = a1;
        ASSERT_TRUE(a2 [0] == 0.f);

        // swap
        psimpl::util::scoped_array <float> a3 (2);
        a3 [0] = 321.f;
        a3 [1] = 654.f;

        a1.swap (a3);
        ASSERT_TRUE(a1.get ());
        ASSERT_TRUE(a3.get ());
        ASSERT_TRUE(a1 [0] == 321.f);
        ASSERT_TRUE(a1 [1] == 654.f);
        ASSERT_TRUE(a3 [0] == 0.f);
        ASSERT_TRUE(a3 [1] == 1.f);
        ASSERT_TRUE(a3 [2] == 2.f);
        ASSERT_TRUE(a3 [3] == 3.f);
        ASSERT_TRUE(a3 [4] == 4.f);

        psimpl::util::swap (a1, a3);
        ASSERT_TRUE(a1.get ());
        ASSERT_TRUE(a3.get ());
        ASSERT_TRUE(a1 [0] == 0.f);
        ASSERT_TRUE(a1 [1] == 1.f);
        ASSERT_TRUE(a1 [2] == 2.f);
        ASSERT_TRUE(a1 [3] == 3.f);
        ASSERT_TRUE(a1 [4] == 4.f);
        ASSERT_TRUE(a3 [0] == 321.f);
        ASSERT_TRUE(a3 [1] == 654.f);
    }
}}

