#include "TestMath.h"
#include "test.h"
#include "helper.h"
#include "../psimpl.h"
#include <vector>
#include <deque>
#include <list>
#include <set>


namespace psimpl {
    namespace test
{
    TestMath::TestMath () {
        TEST_RUN("equal random iterator", TestEqual_RandomIterator ());
        TEST_RUN("equal bidirectional iterator", TestEqual_BidirectionalIterator ());
        TEST_RUN("equal forward iterator", TestEqual_ForwardIterator ());
        TEST_RUN("make_vector random iterator", TestMakeVector_RandomIterator ());
        TEST_RUN("make_vector bidirectional iterator", TestMakeVector_BidirectionalIterator ());
        TEST_RUN("make_vector forward iterator", TestMakeVector_ForwardIterator ());
        TEST_RUN("dot random iterator", TestDot_RandomIterator ());
        TEST_RUN("dot bidirectional iterator", TestDot_BidirectionalIterator ());
        TEST_RUN("dot forward iterator", TestDot_ForwardIterator ());
        TEST_RUN("interpolate fractions", TestInterpolate_Fractions ());
        TEST_RUN("interpolate random iterator", TestInterpolate_RandomIterator ());
        TEST_RUN("interpolate bidirectional iterator", TestInterpolate_BidirectionalIterator ());
        TEST_RUN("interpolate forward iterator", TestInterpolate_ForwardIterator ());
    }

    void TestMath::TestEqual_RandomIterator () {
        {
            const unsigned dim = 2;
            double p1 [dim] = {1, 2};
            double p2 [dim] = {1, 2};
            double p3 [dim] = {2, 3};
            VERIFY_TRUE(psimpl::math::equal <dim> (p1, p2));
            VERIFY_FALSE(psimpl::math::equal <dim> (p1, p3));
        }
        {
            const unsigned dim = 3;
            float v [dim] = {1, 2, 3};
            float w [dim] = {2, 3, 4};
            std::vector <float> p1(v, v + dim);
            std::vector <float> p2(v, v + dim);
            std::vector <float> p3(w, w + dim);
            VERIFY_TRUE(psimpl::math::equal <dim> (p1.begin (), p2.begin ()));
            VERIFY_FALSE(psimpl::math::equal <dim> (p1.begin (), p3.begin ()));
        }
        {
            const unsigned dim = 4;
            int v [dim] = {1, 2, 3, 4};
            int w [dim] = {2, 3, 4, 5};
            std::deque <int> p1(v, v + dim);
            std::deque <int> p2(v, v + dim);
            std::deque <int> p3(w, w + dim);
            VERIFY_TRUE(psimpl::math::equal <dim> (p1.begin (), p2.begin ()));
            VERIFY_FALSE(psimpl::math::equal <dim> (p1.begin (), p3.begin ()));
        }
    }

    void TestMath::TestEqual_BidirectionalIterator () {
        {
            const unsigned dim = 2;
            float v [dim] = {1, 2};
            float w [dim] = {2, 3};
            std::list <float> p1(v, v + dim);
            std::list <float> p2(v, v + dim);
            std::list <float> p3(w, w + dim);
            VERIFY_TRUE(psimpl::math::equal <dim> (p1.begin (), p2.begin ()));
            VERIFY_FALSE(psimpl::math::equal <dim> (p1.begin (), p3.begin ()));
        }
        {
            const unsigned dim = 3;
            int v [dim] = {1, 2, 3};
            int w [dim] = {2, 3, 4};
            std::set <int> p1(v, v + dim);
            std::set <int> p2(v, v + dim);
            std::set <int> p3(w, w + dim);
            VERIFY_TRUE(psimpl::math::equal <dim> (p1.begin (), p2.begin ()));
            VERIFY_FALSE(psimpl::math::equal <dim> (p1.begin (), p3.begin ()));
        }
    }

    void TestMath::TestEqual_ForwardIterator () {
        // TODO
        ASSERT_TRUE(false);
    }

    void TestMath::TestMakeVector_RandomIterator () {
        {
            const unsigned dim = 2;
            double p1 [dim] = {1, 2};
            double p2 [dim] = {3, 5};
            double vec [dim] = {0, 0};
            psimpl::math::make_vector <dim> (p1, p2, vec);

            double expect [dim] = {2, 3};
            VERIFY_TRUE(ComparePoint <dim> (vec, expect));
        }
        {
            const unsigned dim = 3;
            float v [dim] = {1, 2, 3};
            float w [dim] = {4, 6, 8};
            std::vector <float> p1 (v, v + dim);
            std::vector <float> p2 (w, w + dim);
            std::vector <float> vec;
            psimpl::math::make_vector <dim> (p1.begin (), p2.begin (), std::back_inserter (vec));

            VERIFY_TRUE(vec.size () == dim);
            float e [dim] = {3, 4, 5};
            std::vector <float> expect (e, e + dim);
            VERIFY_TRUE(ComparePoint <dim> (vec.begin (), expect.begin ()));
        }
        {
            const unsigned dim = 4;
            int v [dim] = {1, 2, 3, 4};
            int w [dim] = {4, 6, 8, 10};
            std::deque <int> p1 (v, v + dim);
            std::deque <int> p2 (w, w + dim);
            std::deque <int> vec;
            psimpl::math::make_vector <dim> (p1.begin (), p2.begin (), std::back_inserter (vec));

            VERIFY_TRUE(vec.size () == dim);
            int e [dim] = {3, 4, 5, 6};
            std::deque <int> expect (e, e + dim);
            VERIFY_TRUE(ComparePoint <dim> (vec.begin (), expect.begin ()));
        }
    }

    void TestMath::TestMakeVector_BidirectionalIterator () {
        {
            const unsigned dim = 2;
            float v [dim] = {1, 2};
            float w [dim] = {3, 5};
            std::list <float> p1 (v, v + dim);
            std::list <float> p2 (w, w + dim);
            std::list <float> vec;
            psimpl::math::make_vector <dim> (p1.begin (), p2.begin (), std::back_inserter (vec));

            VERIFY_TRUE(vec.size () == dim);
            float e [dim] = {2, 3};
            std::list <float> expect (e, e + dim);
            VERIFY_TRUE(ComparePoint <dim> (vec.begin (), expect.begin ()));
        }
        {
            const unsigned dim = 4;
            int v [dim] = {1, 2, 3, 4};
            int w [dim] = {4, 6, 8, 10};
            std::set <int> p1 (v, v + dim);
            std::set <int> p2 (w, w + dim);
            std::set <int> vec;
            psimpl::math::make_vector <dim> (p1.begin (), p2.begin (), std::inserter (vec, vec.end ()));

            VERIFY_TRUE(vec.size () == dim);
            int e [dim] = {3, 4, 5, 6};
            std::set <int> expect (e, e + dim);
            VERIFY_TRUE(ComparePoint <dim> (vec.begin (), expect.begin ()));
        }
    }

    void TestMath::TestMakeVector_ForwardIterator () {
        // TODO
        ASSERT_TRUE(false);
    }

    void TestMath::TestDot_RandomIterator () {
        {
            const unsigned dim = 2;
            double p1 [dim] = {1, 2};
            double p2 [dim] = {2, 3};
            VERIFY_TRUE(psimpl::math::dot <dim> (p1, p2) == 8.0);
        }
        {
            const unsigned dim = 3;
            float v [dim] = {1, 2, 3};
            float w [dim] = {2, 3, 4};
            std::vector <float> p1(v, v + dim);
            std::vector <float> p2(w, w + dim);
            VERIFY_TRUE(psimpl::math::dot <dim> (p1.begin (), p2.begin ()) == 20.f);
        }
        {
            const unsigned dim = 4;
            int v [dim] = {1, 2, 3, 4};
            int w [dim] = {2, 3, 4, 5};
            std::deque <int> p1(v, v + dim);
            std::deque <int> p2(w, w + dim);
            VERIFY_TRUE(psimpl::math::dot <dim> (p1.begin (), p2.begin ()) == 40);
        }
    }

    void TestMath::TestDot_BidirectionalIterator () {
        {
            const unsigned dim = 2;
            float v [dim] = {1, 2};
            float w [dim] = {2, 3};
            std::list <float> p1(v, v + dim);
            std::list <float> p2(w, w + dim);
            VERIFY_TRUE(psimpl::math::dot <dim> (p1.begin (), p2.begin ()) == 8.f);
        }
        {
            const unsigned dim = 3;
            int v [dim] = {1, 2, 3};
            int w [dim] = {2, 3, 4};
            std::set <int> p1(v, v + dim);
            std::set <int> p2(w, w + dim);
            VERIFY_TRUE(psimpl::math::dot <dim> (p1.begin (), p2.begin ()) == 20);
        }
    }

    void TestMath::TestDot_ForwardIterator () {
        // TODO
        ASSERT_TRUE(false);
    }


    void TestMath::TestInterpolate_Fractions () {
        const unsigned dim = 2;
        double p1 [dim] = {1, 2};
        double p2 [dim] = {2, 3};
        double p3 [dim];
        {
            float fraction = -1.f;
            psimpl::math::interpolate <dim> (p1, p2, fraction, p3);
            float expect [dim] = {0.f, 1.f};
            VERIFY_TRUE(ComparePoint <dim> (p3, expect));
        }
        {
            float fraction = 0.f;
            psimpl::math::interpolate <dim> (p1, p2, fraction, p3);
            VERIFY_TRUE(ComparePoint <dim> (p3, p1));
        }
        {
            float fraction = 1.f;
            psimpl::math::interpolate <dim> (p1, p2, fraction, p3);
            VERIFY_TRUE(ComparePoint <dim> (p3, p2));
        }
        {
            float fraction = 2.f;
            psimpl::math::interpolate <dim> (p1, p2, fraction, p3);
            float expect [dim] = {3.f, 4.f};
            VERIFY_TRUE(ComparePoint <dim> (p3, expect));
        }
    }

    void TestMath::TestInterpolate_RandomIterator () {
        {
            const unsigned dim = 2;
            double p1 [dim] = {1, 2};
            double p2 [dim] = {2, 3};
            double p3 [dim];
            float fraction = 0.1f;
            psimpl::math::interpolate <dim> (p1, p2, fraction, p3);

            double expect [dim] = {1.1, 2.1};
            VERIFY_TRUE(ComparePoint <dim> (p3, expect));
        }
        {
            const unsigned dim = 3;
            float v [dim] = {1, 2, 3};
            float w [dim] = {2, 3, 4};
            std::vector <float> p1(v, v + dim);
            std::vector <float> p2(w, w + dim);
            std::vector <float> p3;
            float fraction = 0.1f;
            psimpl::math::interpolate <dim> (p1.begin (), p2.begin (), fraction, std::back_inserter (p3));
            
            float e [dim] = {1.1f, 2.1f, 3.1f};
            std::vector <float> expect (e, e + dim);
            VERIFY_TRUE(ComparePoint <dim> (p3.begin (), expect.begin ()));
        }
        {
            const unsigned dim = 4;
            int v [dim] = {1, 2, 3, 4};
            int w [dim] = {4, 6, 8, 10};
            std::deque <int> p1(v, v + dim);
            std::deque <int> p2(w, w + dim);
            std::deque <int> p3;
            float fraction = 0.5f;
            psimpl::math::interpolate <dim> (p1.begin (), p2.begin (), fraction, std::back_inserter (p3));
            
            int e [dim] = {2, 4, 5, 7};
            std::deque <int> expect (e, e + dim);
            VERIFY_TRUE(ComparePoint <dim> (p3.begin (), expect.begin ()));
        }
    }

    void TestMath::TestInterpolate_BidirectionalIterator () {
        {
            const unsigned dim = 3;
            float v [dim] = {1, 2, 3};
            float w [dim] = {2, 3, 4};
            std::list <float> p1(v, v + dim);
            std::list <float> p2(w, w + dim);
            std::list <float> p3;
            float fraction = 0.9f;
            psimpl::math::interpolate <dim> (p1.begin (), p2.begin (), fraction, std::back_inserter (p3));
            
            float e [dim] = {1.9f, 2.9f, 3.9f};
            std::list <float> expect (e, e + dim);
            VERIFY_TRUE(ComparePoint <dim> (p3.begin (), expect.begin ()));
        }
        {
            const unsigned dim = 4;
            int v [dim] = {1, 2, 3, 4};
            int w [dim] = {4, 6, 8, 10};
            std::set <int> p1(v, v + dim);
            std::set <int> p2(w, w + dim);
            std::set <int> p3;
            float fraction = 0.5f;
            psimpl::math::interpolate <dim> (p1.begin (), p2.begin (), fraction, std::inserter (p3, p3.end ()));
            
            int e [dim] = {2, 4, 5, 7};
            std::set <int> expect (e, e + dim);
            VERIFY_TRUE(ComparePoint <dim>(p3.begin (), expect.begin ()));
        }
    }

    void TestMath::TestInterpolate_ForwardIterator () {
        // TODO
        ASSERT_TRUE(false);
    }

}}
