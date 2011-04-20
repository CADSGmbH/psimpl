#include "test.h"
#include "TestUtil.h"
#include "TestMath.h"
#include "TestSimplification.h"
#include "TestError.h"


int main (int argc, char *argv [])
{
    TEST_INIT();

    TEST_RUN("util namspace", psimpl::test::TestUtil ());
    TEST_RUN("math namspace", psimpl::test::TestMath ());
    TEST_RUN("simplification algorithms", psimpl::test::TestSimplification ());
    TEST_RUN("error algorithms", psimpl::test::TestError ());

    return TEST_RESULT();
}
