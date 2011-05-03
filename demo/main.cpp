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
    http://sourceforge.net/projects/psimpl/ Originally psimpl was released
    as part of the article 'Polyline Simplification' at The Code Project:
    http://www.codeproject.com/KB/recipes/PolylineSimplification.aspx

    28-09-2010 - Initial version
    23-10-2010 - Changed license from CPOL to MPL
    26-10-2010 - Clarified input (type) requirements, and changed the
                 behavior of the algorithms under invalid input
    01-12-2010 - Added the nth point, perpendicular distance and Reumann-Witkam routines;
                 moved all functions related to distance calculations to the math namespace
    10-12-2010 - Fixed a bug in the perpendicular distance routine
    27-02-2011 - Added Opheim simplification, and functions for computing positional errors
                 due to simplification; renamed simplify_douglas_peucker_alt to
                 simplify_douglas_peucker_n
*/

#include <QtGui/QApplication>
#include "MainWindow.h"

int main (int argc, char *argv [])
{
    QApplication a (argc, argv);
    psimpl::MainWindow w;
    w.show ();
    return a.exec ();
}
