#ifndef _REDIRECTION_PARSER_HPP_
#define _REDIRECTION_PARSER_HPP_

#include <queue>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class RedirectionParser {
        private:
	    std::queue<int> redirection_connectors;
		// 1 = >
		// 2 = >>
		// 3 = <
		// 4 = |
	    std::vector<std::string> redirection_vector;

	void parse_string(string &enter, int &counter, queue<int> &redirection_connectors)
	{
	
		if (enter.at(counter) == '>' && enter.at(counter+1) == '>')
		{
	
		    // Check for >>
                    this->redirection_vector.push_back(enter.substr(0, counter -1));
		    enter.erase(0, counter + 3);
		    counter = 0;

		    redirection_connectors.push(2);
		

		}

	
		else if (enter.at(counter) == '>')
		{		    
		    // Check for >
		    this->redirection_vector.push_back(enter.substr(0, counter -1));
		    enter.erase(0, counter + 2);
		    counter = 0;
		    redirection_connectors.push(1);
		    

		}

		else if (enter.at(counter) == '<')
		{

		// CHECK FOR <
		    this->redirection_vector.push_back(enter.substr(0, counter -1));
                    enter.erase(0, counter + 2);
                    counter = 0;

                    redirection_connectors.push(3);


		}

	else if (enter.at(counter) == '|')
	{
		// CHECK FOR |

                    this->redirection_vector.push_back(enter.substr(0, counter -1));
                    enter.erase(0, counter + 2);
                    counter = 0;

                    redirection_connectors.push(4);

		
	}


		else
		{
		    counter++;
		}
}

            void parse_string_commands(std::string &enter, int &counter, std::vector<std::string> &list) 
         {
                if (enter.at(counter) == ';')
                {
                    list.push_back(enter.substr(0, counter));

                    enter.erase(0, counter + 2);

                    counter = 0;
                }
                else
                {
                    counter++;
                }
        }

        public:
            RedirectionParser() {}
	
	    void parse(std::string command_string) {
		
		this->clean(command_string);
		int counter = 0;

		while (counter < command_string.size())
		{

		    this->parse_string(command_string, counter, redirection_connectors);
		}
		if (!command_string.empty()) // so it doesn't add a blank string
		{
			this->clean(command_string);
		    redirection_vector.push_back(command_string);
 		}
	    }  

	    void clean(std::string &command) {
		
		while(command.at(command.size() - 1) == ' ') {
		    command.erase(command.size() - 1, 1);
		}

		while(command.at(0) == ' ') {
		    command.erase(0,1);
		}
		
	    }


	    int get_front() {
		return redirection_connectors.front();
	    }
	    
	    int get_back() {
		return redirection_connectors.back();
	    }
	    
	    int get_size() {
	    	return redirection_connectors.size();
	    }
	    
	    void pop() {
	    	this->redirection_connectors.pop();
	    }

	    std::vector<std::string> get_individual_commands() {
		return this->redirection_vector;
	    }
};

#endif // _REDIRECTION_PARSER_HPP_

