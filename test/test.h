#ifndef PSIMPL_TEST
#define PSIMPL_TEST

#include <string>
#include <iostream>

#define TEST_INIT() psimpl::test::TestRun::Init()

#define TEST_RUN(name, function)                            \
    try {                                               \
        psimpl::test::TestRun run (name);               \
        function ();                                    \
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


namespace psimpl
{
    namespace test
    {
        struct TestRun
        {
            static int sDepth;
            static int sTestsPassed;
            static int sTestsFailed;
            static int sErrors;
            static int sExceptions;
            
            TestRun (const std::string& name) :
                mCount (sErrors + sExceptions)
            {
                std::cout << Offset () << "-" << name << std::endl;
                ++sDepth;
            }
            
            ~TestRun () {
                --sDepth;

                if (mCount == sErrors + sExceptions)
                    ++sTestsPassed;
                else
                    ++sTestsFailed;

            }

            int mCount;

            static std::string Offset () {
                return std::string (sDepth * 2, ' ');
            }

            static void Error (const char* file, int line, const char* condition) {
                std::cout << Offset () << file << "(" << line << ") error in: " << condition << std::endl;
                ++sErrors;
            }

            static void Exception (const std::string& msg) {
                std::cout << Offset () << "unhandeled exception: " << msg << std::endl;
                ++sExceptions;
            }

            static void Init () {
                std::cout << std::string(80, '_') << std::endl;
                sTestsPassed = sTestsFailed = sErrors = sExceptions = 0;
            }

            static int Result () {
                std::cout << std::string(80, '_') << std::endl;
                std::cout << "Tests passed: " << sTestsPassed << std::endl;
                std::cout << "Tests failed: " << sTestsFailed << std::endl;
                std::cout << "Total errors: " << sErrors << std::endl;
                std::cout << "Total unhandeled exceptions: " << sExceptions << std::endl;
                return sTestsFailed;
            }
        };

        int TestRun::sDepth = 0;
        int TestRun::sTestsPassed = 0;
        int TestRun::sTestsFailed = 0;
        int TestRun::sErrors = 0;
        int TestRun::sExceptions = 0;
    }
}

#endif // PSIMPL_TEST
