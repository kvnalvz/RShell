#ifndef __CONNECTORS_HPP__
#define __CONNECTORS_HPP__

#include <string>
#include "parser.hpp"


class Connectors {

    private:
	Parser* parser = new Parser();
	int status = 1;
	bool run = true;

    public:
        Connectors() { };
	
	void add_connectors(std::string command_string) {
	    parser->parse(command_string);
	}
	
	void set_status(int s) {
	    this->status = s;

	    // Conditions for any possible case
	    if(this->parser->get_front() == 0 && s == 0) {
	 	this->run = true;
	    }
	    else if(this->parser->get_front() == 0 && s == 1) {
	    	this->run = false;
	    }
	    else if(this->parser->get_front() == 1 && s == 1) {
	    	this->run = true;
	    }
	    else if(this->parser->get_front() == 1 && s == 0) {
	  	this->run = false;  
	    }

	    // Remove from queue
	    this->parser->pop();
	}
	
	int get_status() {
	    return this->status;
	}

	bool get_run() {
	    return this->run;
	}
};

#endif //__CONNECTORS_HPP__
