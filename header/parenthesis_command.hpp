#ifndef _PARENTHESIS_COMMAND_GROUP_HPP_
#define _PARENTHESIS_COMMAND_GROUP_HPP_

#include "base.hpp"
#include "connectors.hpp"
#include "command.hpp"
#include "parser.hpp"
#include <vector>
#include <string>

class ParenthesisCommand : public Base {
        private:
            std::vector<Base*> commands;
            Connectors* parent_connectors;
            Connectors* connectors;

        public:
	    ParenthesisCommand(Connectors* c, std::string command) {
		
		// Remove the parenthesis on the outside
		command.erase(0,1);
		command.erase(command.size() -1);

		this->parent_connectors = c;
		this->connectors = new Connectors();

		// This set of connectors is just for what's in the ()
		this->connectors->add_connectors(command);

		this->add_command(command);
	    }	

	    void execute() {
		if(this->parent_connectors->get_run()) {

		    for(unsigned i = 0; i < this->commands.size(); i++) {

		   	if(this->connectors->get_run()) {
		    	    commands.at(i)->execute();
		    	} else {
			    this->connectors->set_status(this->connectors->get_status());
		    	}
		    }

		    // I return to the parent the last command status from the group
		    this->parent_connectors->set_status(this->connectors->get_status());

		} else {
		    this->parent_connectors->set_status(this->parent_connectors->get_status());
		}
	    }

	    void add_command(std::string command_string) {
			
		Parser* parser = new Parser();
	
		// If there is more than one command group then send that down to a new command group

	    	Parser* individual_parser = new Parser();
		individual_parser->parse(command_string);
		
		std::vector<std::string> individual_commands = individual_parser->get_individual_commands();
			
			
		for(unsigned i = 0; i < individual_commands.size(); i++) {
		    
		    std::string type;
                    individual_parser->check_command(individual_commands.at(i), type);

 	    	    Base* add_command;

		   if( type == "paren" ) {

			Connectors* paren_connectors = new Connectors();
		   	add_command = new ParenthesisCommand(this->connectors, individual_commands.at(i));

		    } else if ( type == "test" ) {
			add_command = new TestCommand(this->connectors, individual_commands.at(i));
		    } else {
		    	add_command = new Command(this->connectors, individual_commands.at(i));
		    }

		    this->commands.push_back(add_command);
		}
	    }

	    int size() {
	    	return commands.size();	
	    }

};

#endif // _PARENTHESIS_COMMAND_GROUP_HPP_

