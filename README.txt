psimpl - generic n-dimensional polyline simplification
Copyright (C) 2010 Elmar de Koning, edekoning@gmail.com
--------------------------------------------------------------------------------
28-09-2010 - Initial version
23-10-2010 - Changed license from CPOL to MPL
26-10-2010 - Clarified input (type) requirements, and changed the
             behavior of the algorithms under invalid input.
01-12-2010 - Added the nth point, perpendicular distance and Reumann-Witkam
             routines; moved all functions related to distance calculations to
             the math namespace
10-12-2010 - Fixed a bug in the perpendicular distance routine
--------------------------------------------------------------------------------
'psimpl' is licensed under the Mozilla Public License Version 1.1 (MPL), see
the accompanying LICENSE.txt file. Alternatively a copy of this license may be
obtained from http://www.mozilla.org/MPL/.
--------------------------------------------------------------------------------
'psimpl' was originally released on The Code Project as part of the article
'Polyline Simplification':
http://www.codeproject.com/KB/recipes/PolylineSimplification.aspx

================================================================================

'psimpl' is a polyline simplification library that is generic, easy to use, and
supports the following algorithms:

* Nth point - A naive algorithm that keeps only each nth point
* Distance between points - Removes successive points that are clustered
  together
* Perpendicular distance - Removes points based on their distance to the line
  segment defined by their left and right neighbors
* Reumann-Witkam - Shifts a strip along the polyline and removes points that
  fall outside
* Douglas-Peucker - A classic simplification algorithm that provides an
  excellent approximation of the original line
* A variation on the Douglas-Peucker algorithm - Slower, but yields better
  results at lower resolutions

All the algorithms have been implemented in a single standalone C++ header
using an STL-style interface that operates on input and output iterators.
Polylines can be of any dimension, and defined using floating point or signed
integer data types.

================================================================================

The demo package contains a win32 demo application for 'psimpl'. The demo
application allows you to experiment with the different simplification
algorithms. It can generate pseudo random 2D-polylines of up to 10.000.000
vertices in various types of containers. The boundingbox of the generated
polyline is always n x n, where n is the amount of vertices of the generated
polyline. Use this fact to specify a proper distance tolerance.

Internally, the generated and simplified polyline are always stored in a
QVector<double>. Just before simplification, it is converted to the selected
container type. Afterwards, this temporary container is destructed. Normally you
won't notice this, but it seems that creating and destructing a
std::list(10.000.000) can take a rather long time. The resulting performance
measurements never include these conversion steps. I chose this approach as it
allowed me to quickly add new container types.

Note that the entire application is single threaded (sorry for being lazy),
meaning it could go 'not responding' during a long running simplification.

The demo application was made using Qt 4.7.0, Qt Creator 2.0.1 and Visual Studio
2008 Express. Complete source code is included.

================================================================================

If you decide to use my code for your (commercial) project, let me know! I would
love to hear where my code ends up and why you chose to use it! If possible, a
voluntary donation to my PayPal account (edekoning@gmail.com) would be much
appreciated.

Regards,
Elmar de Koning
edekoning@gmail.com
