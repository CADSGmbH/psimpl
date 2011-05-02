#include "test.h"


namespace psimpl {
    namespace test
{
    int TestRun::sDepth = 0;
    int TestRun::sTestsPassed = 0;
    int TestRun::sTestsFailed = 0;
    int TestRun::sErrors = 0;
    int TestRun::sExceptions = 0;
    int TestRun::sTestsDisabled = 0;

    TestRun::TestRun (const std::string& name) :
        mCount (sErrors + sExceptions),
        mDisabled (false)
    {
        std::cout << Offset () << "- " << name << std::endl;
        ++sDepth;
    }
    
    TestRun::~TestRun () {
        --sDepth;

        if (mDisabled)
            ++sTestsDisabled;
        else if (mCount == sErrors + sExceptions)
            ++sTestsPassed;
        else
            ++sTestsFailed;
    }

    void TestRun::Disable () {
        std::cout << Offset () << "- DISABLED!!!" << std::endl;
        mDisabled = true;
    }

    std::string TestRun::Offset () {
        return std::string (sDepth * 2, ' ');
    }

    void TestRun::Error (const char* file, int line, const char* condition) {
        std::cout << Offset () << file << "(" << line << ") error in: " << condition << std::endl;
        ++sErrors;
    }

    void TestRun::Exception (const std::string& msg) {
        std::cout << Offset () << "unhandeled exception: " << msg << std::endl;
        ++sExceptions;
    }

    void TestRun::Init () {
        std::cout << "starting unit tests" << std::endl;
        std::cout << std::string(80, '_') << std::endl;
        sTestsPassed = sTestsFailed = sErrors = sExceptions = 0;
    }

    int TestRun::Result () {
        std::cout << std::string(80, '_') << std::endl;
        std::cout << "Tests passed: " << sTestsPassed << std::endl;
        std::cout << "Tests failed: " << sTestsFailed << std::endl;
        std::cout << "Tests disabled: " << sTestsDisabled << std::endl;
        std::cout << "Total errors: " << sErrors << std::endl;
        std::cout << "Total unhandeled exceptions: " << sExceptions << std::endl;
        return sTestsFailed;
    }
}}
