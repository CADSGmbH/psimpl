psimpl - generic n-dimensional polyline simplification
Copyright (C) 2010 Elmar de Koning, edekoning@gmail.com
--------------------------------------------------------------------------------
28-09-2010 - Initial version
23-10-2010 - Changed license from CPOL to MPL
26-10-2010 - Clarified input (type) requirements, and changed the
             behavior of the algorithms under invalid input.
--------------------------------------------------------------------------------
'psimpl' is licensed under the Mozilla Public License Version 1.1 (MPL), see
the accompanying LICENSE.txt file. Alternatively a copy of this license may be
obtained from http://www.mozilla.org/MPL/.
--------------------------------------------------------------------------------
'psimpl' was originally released on The Code Project as part of the article
'Polyline Simplification':
http://www.codeproject.com/KB/recipes/PolylineSimplification.aspx
================================================================================

'psimpl' provides a polyline simplification interface that is both generic,
easy to use, and supports the following algorithms:

* Vertex Reduction - A very fast algorithm that yields coarse results.
* Douglas-Peucker Approximation - A classic simplification algorithm that is
  combined with vertex reduction to increase its speed.
* A variation on Douglas-Peucker Approximation - Slower, but yields better
  results at lower resolutions.
  
All the algorithms have been implemented in a single C++ header using an
STL-style interface that operates on input and output iterators. Polylines can
be of any dimension and defined using floating point or signed integer data
types.

If you decide to use my code for your (commercial) project, let me know! I would
love to hear where my code ends up and why you chose to use it! If possible, a
voluntarily donation to my PayPal account (edekoning@gmail.com) would be much
appreciated.
================================================================================

=Vertex Reduction=

Vertex Reduction is a brute force O(n) algorithm for polyline simplification.
It reduces successive vertices that are clustered too closely to a single
vertex, called a key. The resulting keys form the simplified polyline.

=Interface=

template <unsigned DIM, class InputIterator, class OutputIterator>
OutputIterator VertexReduce (
    InputIterator first,
    InputIterator last,
    typename std::iterator_traits <InputIterator>::value_type tol,
    OutputIterator result)

Applies Vertex Reduction to the range [first, last) using the specified
tolerance tol. The resulting simplified polyline is copied to the output range
[result, result + m*DIM), where m is the number of vertices of the simplified
polyline. The return value is the end of the output range: result + m*DIM.

Input (Type) requirements:
1- DIM is not zero, where DIM represents the dimension of the polyline.
2- The InputIterator value type is convertible to a value type of the output
   iterator.
3- The range [first, last) contains vertex coordinates in multiples of DIM,
   f.e.: x, y, z, x, y, z, x, y, z when DIM = 3. If this is not the case, the
   last (incomplete) vertex is ignored.
4- The range [first, last) contains at least 2 vertices.
5- tol is not zero.

In case requirements 1, 4 or 5 are not met, the entire input range [first, last)
is copied to the output range [result, result + (last - first)).

=Usage=

float tolerance = 10;            // point-to-point distance tolerance
std::vector <float> polyline;    // original polyline, assume not empty 
std::vector <float> result;      // resulting simplified polyline

// simplify the 2d polyline
psimpl::VertexReduce <2> (polyline.begin (), polyline.end (),
                          tolerance, std::back_inserter (result));
================================================================================

=Douglas-Peucker Approximation=

Instead of using a point-to-point distance tolerance as a rejection criterion
(see Vertex Reduction), the Douglas-Peucker Approximation algorithm uses a
point-to-edge distance tolerance. The algorithm starts with a crude
simplification that is the single edge joining the first and last vertices of
the original polyline. It then computes the distance of all intermediate
vertices to that edge. The vertex that is furthest away from that edge, and that
has a computed distance that is larger than a specified tolerance, will be
marked as a key and added to the simplification. This process will recurse for
each edge in the current simplification, until all vertices of the original
polyline are within tolerance of the simplification results.

This algorithm has a worst case running time of O(nm) and O(n log m) on average,
where m is the size of the simplified polyline. As such, this is an output
dependent algorithm, and will be very fast when m is small. To make it even
faster, vertex reduction is applied as a pre-processing step.

=Interface=

template <unsigned DIM, class InputIterator, class OutputIterator>
OutputIterator DouglasPeuckerApproximate (
        InputIterator begin,
        InputIterator end,
        typename std::iterator_traits <InputIterator>::value_type tol,
        OutputIterator result)
        
Applies Vertex Reduction followed by Douglas-Peucker Approximation to the range
[first, last) using the specified tolerance tol. The resulting simplified
polyline is copied to the output range [result, result + m*DIM), where m is the
number of vertices of the simplified polyline. The return value is the end of
the output range: result + m*DIM.

Input (Type) requirements:
1- DIM is not zero, where DIM represents the dimension of the polyline.
2- The InputIterator value type is convertible to a value type of the output
   iterator.
3- The range [first, last) contains vertex coordinates in multiples of DIM,
   f.e.: x, y, z, x, y, z, x, y, z when DIM = 3. If this is not the case, the
   last (incomplete) vertex is ignored.
4- The range [first, last) contains at least 2 vertices.
5- tol is not zero.

In case requirements 4 or 5 are not met, the entire input range [first, last) is
copied to the output range [result, result + (last - first)).

=Usage=

double tolerance = 10;                  // point-to-edge distance tolerance
std::deque <double> polyline;           // original polyline, assume not empty 
double* result = new double [polyline.size ()];  // make sure the result array 
                                        // is large enough

// simplify the 3d polyline
psimpl::DouglasPeuckerApproximate <3> (polyline.begin (), polyline.end (),
                                       tolerance, result);
                                       
This example demonstrates that input and output containers do not have to be the
same.
================================================================================

=Douglas-Peucker Approximation (Variant)=

This algorithm is a variation of the original implementation. Its key
differences are:
* A point count tolerance is used instead of a point-to-edge distance tolerance.
  This allows you to specify the exact number of vertices in the simplified
  polyline. With the original implementation, you can never be sure how many
  vertices will remain.
* Instead of processing a single edge at a time (chosen pseudo random), all
  edges of the current simplified polyline are considered simultaneously. Each
  of these edges may define a new key. From all these possible keys, the one
  with highest point-to-edge distance is chosen as the new key.

A direct result from always choosing the next key based on all possible keys at
any given time, is that the simplification results are of a higher quality. This
is most notably when using a very low point-count tolerance. A downside is that
we cannot use Vertex Reduction as a pre-processing step to speed up the
algorithm.

=Interface=

template <unsigned DIM, class InputIterator, class OutputIterator>
OutputIterator DouglasPeuckerApproximateAlt (
        InputIterator begin,
        InputIterator end,
        unsigned count,
        OutputIterator result)
        
Applies a variant of the Douglas-Peucker Approximation to the range
[first, last). The resulting simplified polyline consists of count vertices and
is copied to the output range [result, result + count). The return value is the
end of the output range: result + count.

Input (Type) requirements:
1- DIM is not zero, where DIM represents the dimension of the polyline.
2- The InputIterator value type is convertible to a value type of the output
   iterator.
3- The range [first, last) contains vertex coordinates in multiples of DIM,
   f.e.: x, y, z, x, y, z, x, y, z when DIM = 3. If this is not the case, the
   last (incomplete) vertex is ignored.
4- The range [first, last) contains a minimum of count vertices.
5- count is at least 2

In case requirements 4 or 5 are not met, the entire input range [first, last) is
copied to the output range [result, result + (last - first))

=Usage=

unsigned tolerance = 25;          // point count tolerance
std::list <long long> polyline;   // original polyline, assume not empty 
std::vector <double> result;      // resulting simplified polyline

// simplify the 4d polyline to 25 points
psimpl::DouglasPeuckerApproximateAlt <4> (polyline.begin (), polyline.end (),
                                          tolerance, std::back_inserter (result));

This example demonstrates the use of a non-random access container with a singed
integer data type.