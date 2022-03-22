#ifndef __TEST_COMMAND_TEST_HPP__
#define __TEST_COMMAND_TEST_HPP__

#include "gtest/gtest.h"
#include <string>

#include "../header/base.hpp"
#include "../header/test_command.hpp"
#include "../header/connectors.hpp"

TEST(TestCommand, FirstTest) {

    std::string command = "./names.txt";

    Connectors* start_connectors = new Connectors();
    TestCommand* test_command = new TestCommand(start_connectors, command);
    test_command->execute();

    EXPECT_EQ(test_command->get_exist(), true);
}

#endif //___COMMAND_GROUP_TEST_HPP__
