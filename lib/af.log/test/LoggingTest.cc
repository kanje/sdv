import sdv.af.log;
import sdv.af.test.gtest;

#include <gmock/gmock.h>

class LoggingTest : public sdv::af::test::gtest::Test
{
};

TEST_F(LoggingTest, hello)
{
    sdv::af::info("Hello {}", "world");
}
