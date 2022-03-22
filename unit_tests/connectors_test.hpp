#ifndef __CONNECTORS_TEST_HPP__
#define __CONNECTORS_TEST_HPP__

#include "gtest/gtest.h"

#include <string>
#include "../header/connectors.hpp"

TEST(ConnectorsTest, CheckCommandsRun ) {

    std::string command = "echo hi && ls -a || cat main.cpp || echo bye && echo bye again";

    Connectors* connectors = new Connectors();
    connectors->add_connectors(command);

    // The connectors class always allows the first command to run
    // set_status will do the logic to let us know if the next command should run
    connectors->set_status(1);

    EXPECT_EQ(connectors->get_status(), 1);
    // This lets us know that ls -a should run
    EXPECT_EQ(connectors->get_run(), true);

    // Since ls -a did run
    connectors->set_status(1);
    // cat main.cpp should not run since the preivious command did run without errors
    EXPECT_EQ(connectors->get_run(), false);

    // This is the opperation that will be preformed once a command should not run
    connectors->set_status(connectors->get_status());
   
    // echo bye should not run
    EXPECT_EQ(connectors->get_run(), false);
    connectors->set_status(connectors->get_status());

    // echo bye again should run
    EXPECT_EQ(connectors->get_run(), true);

}


#endif //__CONNECTORS_TEST_HPP__
