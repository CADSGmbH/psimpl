#include "TestUtil.h"
#include "test.h"
#include "../psimpl.h"


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

