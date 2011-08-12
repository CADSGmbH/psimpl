// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Linestring Example

//#include <algorithm> // for reverse, unique
//#include <iostream>
//#include <iterator>
//#include <utility>
//#include <vector>

//#include <boost/geometry/geometry.hpp>
//#include <boost/geometry/geometries/linestring.hpp>
//#include <boost/geometry/geometries/point_xy.hpp>
//#include <boost/geometry/domains/gis/io/wkt/read_wkt.hpp>


//int main(void)
//{
//    using namespace boost::geometry;

//    boost::geometry::model::linestring<boost::geometry::model::d2::point_xy<double> > line, simplified;
//    boost::geometry::read_wkt("linestring(1.1 1.1, 2.5 2.1, 3.1 3.1, 4.9 1.1, 3.1 1.9)", line);
//    boost::geometry::simplify(line, simplified, 1);
//    std::cout
//        << "  original line: " << boost::geometry::dsv(line) << std::endl
//        << "simplified line: " << boost::geometry::dsv(simplified) << std::endl;

//    return 0;
//}













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

#include "test.h"
#include "TestUtil.h"
#include "TestMath.h"
#include "TestSimplification.h"
#include "TestError.h"


int main (int /*argc*/, char * /*argv*/ [])
{
    TEST_INIT();

    TEST_RUN("util namspace", psimpl::test::TestUtil ());
    TEST_RUN("math namspace", psimpl::test::TestMath ());
    TEST_RUN("simplification algorithms", psimpl::test::TestSimplification ());
    TEST_RUN("error algorithms", psimpl::test::TestError ());

    return TEST_RESULT();
}
