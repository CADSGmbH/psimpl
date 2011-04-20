#ifndef PSIMPL_TEST
#define PSIMPL_TEST

#include <string>
#include <iostream>

#define TEST_INIT() psimpl::test::TestRun::Init()

#define TEST_RUN(name, function)                        \
    try {                                               \
        psimpl::test::TestRun run (name);               \
        function;                                       \
    }                                                   \
    catch (std::exception& e) {                         \
        psimpl::test::TestRun::Exception (e.what ());   \
    }

#define TEST_RESULT() psimpl::test::TestRun::Result()

#define PASS() // do nothing
#define FAIL(condition) psimpl::test::TestRun::Error (__FILE__, __LINE__, condition);
#define ABORT(condition) psimpl::test::TestRun::Error (__FILE__, __LINE__, condition); return;

#define VERIFY_TRUE(condition)  if (condition) { PASS() } else { FAIL(#condition) }
#define VERIFY_FALSE(condition) if (condition) { FAIL(#condition) } else { PASS() }

#define ASSERT_TRUE(condition)  if (condition) { PASS() } else { ABORT(#condition) }
#define ASSERT_FALSE(condition) if (condition) { ABORT(#condition) } else { PASS() }


namespace psimpl {
    namespace test
{
    class TestRun
    {
    public:
        TestRun (const std::string& name);
        ~TestRun ();

        static void Error (const char* file, int line, const char* condition);
        static void Exception (const std::string& msg);
        static void Init ();
        static int Result ();

    private:
        static std::string Offset ();

    private:
        static int sDepth;
        static int sTestsPassed;
        static int sTestsFailed;
        static int sErrors;
        static int sExceptions;

        int mCount;
    };
}}


#endif // PSIMPL_TEST
