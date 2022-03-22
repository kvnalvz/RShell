#ifndef _COMMAND_GROUP_HPP_
#define _COMMAND_GROUP_HPP_

#include "base.hpp"
#include "connectors.hpp"
#include "command.hpp"
#include "test_command.hpp"
#include "parenthesis_command.hpp"
#include <vector>
#include <string>
#include <iostream>

class CommandGroup : public Base {
        private:
            std::vector<Base*> commands;
            Connectors* connectors;

        public:
	    CommandGroup(Connectors* c, std::string command) {
		this->connectors = c;

		this->add_command(command);
	    }	

	    void execute() {
		for(unsigned i = 0; i < this->commands.size(); i++) {
		    commands.at(i)->execute();
		}
	    }

	    void add_command(std::string command_string) {
		// The parser will return a vectror that should just be commands joined by connectors, no ;
	 	std::vector<std::string> command_g;
			
		// Add new Base*  Command or CommandGroup
		Parser* parser = new Parser();
	
		// Parser should add command groups by refference
		// It could be a vector of just one or more
		parser->parse(command_string, command_g);

		// If there is more than one command group then send that down to a new command group
		if(command_g.size() > 1) {
		
		    for(unsigned i = 0; i < command_g.size(); i++) {
			// For the CommandGroup I create new Connectors	
		    	Connectors* add_connectors = new Connectors();
		    	Base* add_command_group = new CommandGroup(add_connectors, command_g.at(i));
			//add_command_group->add_command(command_g.at(i));
		    	this->commands.push_back(add_command_group);
		    }
   
		} else {

		    Parser* individual_parser = new Parser();
		    individual_parser->parse(command_string);

		    this->connectors->add_connectors(command_string);
		    
		    std::vector<std::string> individual_commands = individual_parser->get_individual_commands();
		    for(unsigned i = 0; i < individual_commands.size(); i++) {
		    	
			std::string type;

			individual_parser->check_command(individual_commands.at(i), type);

			Base* add_command;

			if(type == "test") {
		    	    add_command = new TestCommand(this->connectors, individual_commands.at(i));
			} else if(type == "paren") {
		    	    add_command = new ParenthesisCommand(this->connectors, individual_commands.at(i));
			} else {
		    	    add_command = new Command(this->connectors, individual_commands.at(i));
			}

		    	this->commands.push_back(add_command);

		    }
		}
	    }

	    int size() {
	    	return commands.size();	
	    }

};

#endif // _COMMAND_GROUP_HPP_

