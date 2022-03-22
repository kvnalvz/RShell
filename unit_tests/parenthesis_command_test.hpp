#ifndef __PARENTHESIS_COMMAND_TEST_HPP__
#define __PARENTHESIS_COMMAND_TEST_HPP__

#include "gtest/gtest.h"
#include <string>

#include "../header/base.hpp"
#include "../header/parenthesis_command.hpp"
#include "../header/connectors.hpp"

TEST(ParenthesisCommand, FirstTest) {

    std::string command = "(test command)";
    Connectors* start_connectors = new Connectors();
    ParenthesisCommand* test_command = new ParenthesisCommand(start_connectors, command);
    //test_command->execute();

    EXPECT_EQ(test_command->size(), 1);
}

#endif //__PARENTHESIS_COMMAND_TEST_HPP__
