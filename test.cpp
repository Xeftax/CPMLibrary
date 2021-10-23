#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

#define private public


TEST_GROUP(ConversationHistoryTests)
{
};

TEST(ConversationHistoryTests, FirstTest)
{
    FAIL("Fail me!");
}

int main(int ac, char** av){
    return CommandLineTestRunner::RunAllTests(ac, av);
}