#include "gtest/gtest.h"

#include "parser_test.hpp"
#include "connectors_test.hpp"
#include "command_group_test.hpp"
#include "test_command_test.hpp"
#include "parenthesis_command_test.hpp"
#include "parser_redirection_test.hpp"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
