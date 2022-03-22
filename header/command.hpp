#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include <string>
#include <iostream>
#include "connectors.hpp"
#include "base.hpp"
#include "parser.hpp"
#include "parser_redirection.hpp"
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


class Command : public Base {
    
    std::string	command_string;
    Connectors* connectors;	
    std::vector<std::string> execs;
    int counter = 1;


    public:
        Command(Connectors* c, std::string command) { 
	    this->connectors = c;
	    this->add_command(command);
	};
	
	void add_command(std::string command) {
	    Parser* parser = new Parser();
	    parser->clean(command); // Remove extra spaces at the end or begining
	    this->command_string = command;
	}
	
        void execute() {
	    
	    if(this->connectors->get_run()) {

	    if(this->command_string == "exit" ) {
		exit(1);
  	    }



	    RedirectionParser* rp = new RedirectionParser();
	    rp->parse(this->command_string);
	    vector<std::string> r_commands = rp->get_individual_commands();
	    std::string file;
		
	    if(r_commands.size() > 1) {
		file = r_commands.at(1);
		this->command_string = r_commands.at(0);
	    }


	    int status;

	    char* args[100];

	    int quote = this->command_string.find('"');
	    int last_quote = this->command_string.find_last_of('"');

	    if(quote > 0 && last_quote > 0) {
		int space = this->command_string.find(' ');
		execs.push_back(this->command_string.substr(0, space));
		execs.push_back(this->command_string.substr(quote + 1, (last_quote - quote) - 1));
		counter++;
	    } else {

  	    for (int i = 0; i < this->command_string.size(); i++)
  	    {
      		if (this->command_string.at(i) == ' ')
      		{
          	    execs.push_back(this->command_string.substr(0,i));

          	    this->command_string.erase(0,i +1);
          	    i = 0;
          	    counter++;
      		}
     		if (i == (this->command_string.size() -1))
      	    	{
          	    execs.push_back(this->command_string);
      	    	}

  	    }
	    }


	    pid_t child = fork();
	    pid_t child2;

            if (child == 0)
	    {

	  	    for (int i = 0; i < counter; i++)
  	    	    {
     		    args[i] = (char*)(execs.at(i).c_str());
 	 	    }
 	 	    args[counter] = NULL;	

		    
		    int tmpfd_o = dup(1);
		    int tmpfd_i = dup(0);
		    int newFile;

		    if(rp->get_front() == 1) {

	                newFile = open(file.c_str(), O_ASYNC | O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			dup2(newFile, 1);
	                close(newFile);

		    } else if(rp->get_front() == 2) {

			newFile = open(file.c_str(), O_RDWR | O_APPEND);
			dup2(newFile, 1);
	                close(newFile);

		    } else if(rp->get_front() == 3 ) {

			newFile = open(file.c_str(), O_RDONLY);
			dup2(newFile, 0);
	                close(newFile);

		    }
				   	

	    	    if ( execvp (args[0], args) == -1)
            	    {
              	    	perror ("RSHELL failed");
			this->connectors->set_status(0);
			
			exit(2);
				
	    	    }

		 
		    if(rp->get_front() == 1 || rp->get_front() == 2) {
			dup2(tmpfd_o, 1);
		    } else if(rp->get_front() == 3) {
			dup2(tmpfd_i, 0);
		    }

		
	    }
	    else if (child > 0)
	    {
		
		child2 = waitpid(child, &status, 0);
		
	    	if (WEXITSTATUS(status) == 1)
	    	{
	 		this->connectors->set_status(0);
	    	}
	    	else 
	    	{
	     		this->connectors->set_status(1);
	    	}

	   }
	


	    }
	    else
	    {
		this->connectors->set_status(this->connectors->get_status());	
	    }

	}	
	
	int size() {
	    return 1;
	}
};

#endif //__COMMAND_HPP__
