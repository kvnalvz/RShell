#ifndef __TEST_COMMAND_HPP__
#define __TEST_COMMAND_HPP__

#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "connectors.hpp"
#include "base.hpp"
#include "parser.hpp"

class TestCommand : public Base {
    
    std::string	command_string;
    Connectors* connectors;	
    bool exist = true; // default to true
    // This is made possible because of stat.h
    struct stat sb;

    public:
        TestCommand(Connectors* c, std::string command) { 
	    this->connectors = c;
	    this->command_string = command;
	};
	
        void execute() {

	    if(this->connectors->get_run()) {
	 	
		std::string command_type;
		Parser* parser = new Parser();
		parser->check_command(this->command_string, command_type);

		if (stat(this->command_string.c_str(), &sb) == -1) {
		    this->connectors->set_status(0);
		    this->exist = false;
		    printf("(False)\n");
		    return;
		}

	    if(exist) {
		if(command_type == "-e") {
		    if(  S_ISDIR(sb.st_mode) == 1 || S_ISREG(sb.st_mode) == 1 ) {
			this->exist = true;
		    } else {
			this->exist = false;
		    }
		} else if( command_type == "-f" ) {
		    if( S_ISREG(sb.st_mode) == 1 ) {
			this->exist = true;
		    } else {
			this->exist = false;
		    }

		} else if( command_type == "-d") {
		    if( S_ISDIR(sb.st_mode) == 1 ) {
			this->exist = true;
		    } else {
			this->exist = false;
		    }
		}
		
		if(this->exist) {
		    printf("(True)\n");
		    this->connectors->set_status(1);
		} else {
		    printf("(False)\n");
		    this->connectors->set_status(0);
		}

		}	
	    }
	    else
	    {
		this->connectors->set_status(this->connectors->get_status());	
	    }

	}

	bool get_exist() {
	    return this->exist;
	}	
	
	int size() {
	    return 1;
	}
};

#endif //__TEST_COMMAND_HPP__
