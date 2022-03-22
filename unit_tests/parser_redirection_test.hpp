#ifndef __REDIRECTION_PARSER_TEST_HPP__
#define __REDIRECTION_PARSER_TEST_HPP__

#include "gtest/gtest.h"

#include <string>
#include <vector>
#include "../header/parser_redirection.hpp"

TEST(RedirectionParserTest, BaseParse) {

    std::string command = "cat < existingInputFile | tr A-Z a-z | tee newOutputFile1 | tr a-z A-Z > newOutputFile2 >> hi";
    RedirectionParser* parser = new RedirectionParser();

    parser->parse(command);

    EXPECT_EQ(parser->get_front(), 3);
    EXPECT_EQ(parser->get_back(), 2);
    EXPECT_EQ(parser->get_size(), 6);
    //parser->pop();
    //EXPECT_EQ(parser->get_size(), 5);
	
    std::vector<std::string> redirection_commands = parser->get_individual_commands();

    EXPECT_EQ(redirection_commands.at(0), "cat");
    EXPECT_EQ(redirection_commands.at(1), "existingInputFile");
    EXPECT_EQ(redirection_commands.at(2), "tr A-Z a-z");
    EXPECT_EQ(redirection_commands.at(3), "tee newOutputFile1");
    EXPECT_EQ(redirection_commands.at(4), "tr a-z A-Z");
    EXPECT_EQ(redirection_commands.at(5), "newOutputFile2");
    EXPECT_EQ(redirection_commands.at(6), "hi");
}

#endif //__REDIRECTION_PARSER_TEST_HPP__
