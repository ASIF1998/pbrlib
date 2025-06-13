#include <gtest/gtest.h>

#include <backend/logger/logger.hpp>

using namespace testing;

int main(int argc, char* argv[])
{
    try
    {
        InitGoogleTest();
        return RUN_ALL_TESTS();
    }
    catch (std::exception& ex)
    {
        pbrlib::backend::log::error("[pbrlib-tests] failed tests: {}", ex.what());
    }
    catch(...)
    {
        pbrlib::backend::log::error("[pbrlib-tests] unknown error");
    }

    return std::numeric_limits<int>::max();
}
