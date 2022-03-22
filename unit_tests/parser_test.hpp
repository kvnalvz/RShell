#ifndef __PARSER_TEST_HPP__
#define __PARSER_TEST_HPP__

#include "gtest/gtest.h"

#include <string>
#include <vector>
#include <iostream>
#include "../header/parser.hpp"

TEST(ParserTest, ParseConnectors ) {

    std::string command = "echo hi || ls -a && cat main.cpp || echo bye && echo \"dont && echo run\" || echo this && echo bye again";
    Parser* parser = new Parser();

    parser->parse(command);
    EXPECT_EQ(parser->get_front(), 0);
    EXPECT_EQ(parser->get_back(), 1);
    EXPECT_EQ(parser->get_size(), 6);
    parser->pop();
    EXPECT_EQ(parser->get_size(), 5);
	
    std::vector<std::string> test_commands = parser->get_individual_commands();
    
    EXPECT_EQ(test_commands.at(4), "echo \"dont && echo run\"");


}

TEST(ParserTest, ParseSingleCommand ) {

    std::string command = "echo hi";
    Parser* parser = new Parser();
    parser->parse(command);

    EXPECT_EQ(parser->get_size(), 0);
}

TEST(ParserTest, ParseCommandGroups ) {

    std::string command_group = "echo hi ; ls -a || echo hola ; cat main.cpp && echo hello world";

    std::vector<std::string> command_g;

    Parser* parser = new Parser();
    // When a vector is passed as a second value, the parser will updated the vector via reference
    parser->parse(command_group, command_g);

    EXPECT_EQ(command_g.size(), 3);
}

TEST(ParserTest, ParenthesisTest ) {

    std::string command = "echo hi && ls -a || (echo hola && ls -a || (cat somefile.txt && echo here))  && echo bye && ( echo hi || ls -a )";
    Parser* parser = new Parser();

    parser->parse(command);

    std::string command2 = "( echo ab && echo cd) || ( echo ef && echo gh) || ( echo lg && echo rh) && echo sfsd";
    Parser* parser2 = new Parser();

    parser2->parse(command2);


   // Size of connectors
    
    EXPECT_EQ(parser->get_size(), 4);

    std::vector<std::string> test_commands = parser->get_individual_commands();
    
    EXPECT_EQ(test_commands.at(2), "(echo hola && ls -a || (cat somefile.txt && echo here))");
    EXPECT_EQ(test_commands.at(3), "echo bye");
    EXPECT_EQ(test_commands.at(4), "( echo hi || ls -a )");
   
    // Size of connectors
    EXPECT_EQ(parser2->get_size(), 3);
	
    std::vector<std::string> test_commands2 = parser2->get_individual_commands();
    
    EXPECT_EQ(test_commands2.at(0), "( echo ab && echo cd)");
    EXPECT_EQ(test_commands2.at(1), "( echo ef && echo gh)");
    EXPECT_EQ(test_commands2.at(2), "( echo lg && echo rh)");
    EXPECT_EQ(test_commands2.at(3), "echo sfsd");

}

TEST(ParserTest, TestParser ) {

    std::string command = "echo hi && ls -a || (echo hola && ls -a || (cat somefile.txt && echo here)) && test -f ./somefile || echo bye";
    std::string command2 = "[ -e test/file/path ] && echo hi && ls -a || (echo hola && ls -a || (cat somefile.txt && echo here)) && [ -d ./somefile ] || echo bye";

    // First test
    Parser* parser = new Parser();
    parser->parse(command);

    // Second test
    Parser* parser2 = new Parser();
    parser2->parse(command2);


    // Size of connectors
    EXPECT_EQ(parser->get_size(), 4);
    EXPECT_EQ(parser2->get_size(), 5);
    std::vector<std::string> test_commands = parser->get_individual_commands();
    std::vector<std::string> test_commands2 = parser2->get_individual_commands();


    EXPECT_EQ(test_commands.at(3), "test -f ./somefile");
    EXPECT_EQ(test_commands2.at(4), "[ -d ./somefile ]");
    EXPECT_EQ(test_commands2.at(0), "[ -e test/file/path ]");
    EXPECT_EQ(test_commands2.at(5), "echo bye");
}

TEST(ParserTest, CommandCheck ) {

    std::string type1;
    std::string command1 = "(echo hi && (echo in || ls -a))";

    std::string type2;
    std::string command2 = "test -e ./somefile.txt";
    
    std::string type3;
    std::string command3 = "./somefile.txt";

    std::string type4;
    std::string command4 = "[ -e ./somefile ]";

    std::string type5;
    std::string command5 = "[-e ./somefile]";



    // First test
    Parser* parser = new Parser();
    parser->check_command(command1, type1);
    parser->check_command(command2, type2);
    parser->check_command(command3, type3);
    parser->check_command(command4, type4);
    parser->check_command(command5, type5);

    EXPECT_EQ(command1, "(echo hi && (echo in || ls -a))");
    EXPECT_EQ(type1, "paren");

    EXPECT_EQ(command2, "-e ./somefile.txt");
    EXPECT_EQ(type2, "test");

    EXPECT_EQ(command3, "./somefile.txt");
    EXPECT_EQ(type3, "-e");

    EXPECT_EQ(command4, "-e ./somefile");
    EXPECT_EQ(type4, "test");

    EXPECT_EQ(command5, "-e ./somefile");
    EXPECT_EQ(type5, "test");


}

TEST(ParserTest, CleanString ) {

    std::string command = "   echo clean   ";
    std::string command2 = "echo clean ";
    std::string command3 = " echo clean ";
    std::string command4 = "echo clean";

    Parser* parser = new Parser();
    parser->clean(command);
    parser->clean(command2);
    parser->clean(command3);
    parser->clean(command4);

    EXPECT_EQ(command, "echo clean");
    EXPECT_EQ(command2, "echo clean");
    EXPECT_EQ(command3, "echo clean");
    EXPECT_EQ(command4, "echo clean");
}


#endif //__PARSER_TEST_HPP__
