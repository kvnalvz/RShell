#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <queue>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Parser {
        private:
	    std::queue<int> connectors;
	    vector<string> tester;
 	    vector<string> paren;
	    std::vector<std::string> single_command_list;
	    bool in_quotes = false;

	    void parse_string( std::string &enter, int &counter, std::queue<int> &connectors ) {
		if(enter.at(counter) == '\"') {
		    in_quotes = !in_quotes;
		}
		
		if(in_quotes) {
		    counter++;
		    return;
		}

		if (enter.size() >= 2 && enter.at(counter) == '&' && enter.at(counter + 1) == '&')
		{
		    
		    // Check for &&
		    this->single_command_list.push_back(enter.substr(0, counter -1));
		    enter.erase(0, counter + 3);
		    counter = 0;
		    connectors.push(1);
		    

		}
		else if (enter.size() >= 2 && enter.at(counter) == '|' && enter.at(counter + 1) == '|')
		{

		    // Check for ||
                    this->single_command_list.push_back(enter.substr(0, counter -1));
		    enter.erase(0, counter + 3);
		    counter = 0;

		    connectors.push(0);
		

		}
		else if (enter.at(counter) == '#')
		{
		    // Check for #
		    if(counter != 0)
		    {
			this->single_command_list.push_back(enter.substr(0,counter-1));
			enter.erase(0,counter);
			counter = 0;
		    }
		    else
		    {
			enter.erase(0,enter.size());
			counter = 0;
			connectors.push(0);
		    }
		}

	else if (enter.at(counter) == '(')
	{
		int leftParen = 1;
		int rightParen = 0;
        	int lastRight = 1;
		// I count the number of ( and the number of )
		// Once the number of ( matches the number of ) I know I have the outermost
		// so I just break out of the loop.
		for (int i = 1; i < enter.size() + 1; i++)
		{
		
		    if(i == enter.size() && leftParen != rightParen) {
			std::cout << "Missing closing ')'" << std::endl;
			exit(1);
		    }

		    if(enter.at(i) == '(')
		    {
			leftParen++;
		    }
		    else if (enter.at(i) == ')')
		    {
		    	rightParen++;
		    }
            	    lastRight++;
		
            	    if(leftParen == rightParen) {
                	break;
            	    }

		}
		
		// I add that outermost to the vector and erase it from the string
		single_command_list.push_back(enter.substr(0, lastRight));
        	enter.erase(0, lastRight);
		
		int found_pass = enter.find('&');
		int found_fail = enter.find('|');
		// NOTE (bryan): Here is my aproach on checking if there's a && or an || after the outermost
		// I didn't want to assume that it was just exactlly one space after the command so insted I use the
		// find function
		// If a & is found, found_pass will hold it's index, if not it will be set to -1
		// I check if there is an & and if it comes before any |
		// If so I add a connector and erase that & the following one and the space after that
		//
		// I do the same for the ||
		// The check for -1 is in the case that the rest of the string just has && or ||
		if((found_pass > 0 && found_pass < found_fail) || (found_pass > 0 && found_fail == -1)) {
	  	    enter.erase(0, found_pass + 3); // erase the first &, the second & and the space after                      	
		    connectors.push(1);
		} else if((found_fail > 0 && found_fail < found_pass) || (found_fail > 0 && found_pass == -1)) {
		    enter.erase(0, found_fail + 3); // erase the first |, the second | and the space after	
	            connectors.push(0);
		}
	
        	counter = 0;
		
	}

    else if (enter.at(counter) == '[' && enter.at(counter+1) == ' ')
    {
        for (int i = counter; i < enter.size(); i++)
        {
            if (enter.at(i) == ']' && enter.at(i -1) == ' ')
            {
                
                
               
                if(i == enter.size()-1)
                {
                    single_command_list.push_back(enter.substr(counter,i+1));
                    enter.erase(counter,i+2);
                    break;
                }
                else if (enter.at(i +2) == '&' && enter.at(i+3) == '&')
                {
                    
                    single_command_list.push_back(enter.substr(counter,i+1));
                    enter.erase(counter,i+5);
                    
                    connectors.push(1);
                    
                    break;
                    
                }
                
                else if (enter.at(i+2) == '|' && enter.at(i+3) == '|')
                {
                    
                    single_command_list.push_back(enter.substr(counter,i+1));
                    enter.erase(counter,i+5);
                    
                    connectors.push(0);
                    
                    break;
                    
                }

            }
            
        }
        
        counter = 0;
    }

// ------------
else if (enter.size() >= 4 && (enter.at(counter) == 't' && counter == 0) && (enter.at(counter+1) == 'e') && (enter.at(counter+2) == 's')
             && (enter.at(counter+3) == 't'))
    {


	// will push whatever is after the -f, -d, -e.
	// and before ||, && or ;
	// then erase
        for (int i = 0; i < enter.size(); i++)
        {

            if (i == enter.size() -1)
            {
                single_command_list.push_back(enter.substr(counter,i+1));
                enter.erase(0,i+1);
                break;
            }
            else if (enter.at(i) == '&' && enter.at(i+1) == '&')
            {
                single_command_list.push_back(enter.substr(counter,i-1));
                enter.erase(counter,i+3);
             	connectors.push(1);
                   break;
            }
            else if(enter.at(i) == '|' && enter.at(i+1) == '|')
            {
                single_command_list.push_back(enter.substr(counter,i-1));
                enter.erase(0,i+3);
                connectors.push(0);
                break;
            }
    
        }
        counter = 0;
    }

// -----

    else if (enter.size() >= 4 && (toupper(enter.at(counter)) == 'E' && counter == 0) && toupper(enter.at(counter+1)) == 'X' && toupper(enter.at(counter+2)) == 'I' && toupper(enter.at(counter+3)) == 'T')
    {
        for (int i = 0; i < enter.size(); i++)
        {
            
        if (i == enter.size()-1)
        {
        single_command_list.push_back(enter.substr(counter,4));
        enter.erase(counter, counter+4);
            break;
        }
            else if (enter.at(i) == '&' && enter.at(i+1) == '&')
            {
                single_command_list.push_back(enter.substr(counter,i-1));
                enter.erase(counter,i+3);
		connectors.push(1);
                break;
                
            }
            else if(enter.at(i) == '|' && enter.at(i+1) == '|')
            {
                single_command_list.push_back(enter.substr(counter,i-1));
                enter.erase(counter,i+3);
		connectors.push(0);
                break;
                
            }

            
        }
        counter = 0;
    }






// ------
		else
		{
		    counter++;
		}
	    }
	    
	    void parse_string_commands(std::string &enter, int &counter, std::vector<std::string> &list) {
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
            Parser() {}
	
	    void parse(std::string command_string) {
		
		this->clean(command_string);
		int counter = 0;

		while (counter < command_string.size())
		{
		    this->parse_string(command_string, counter, connectors);
		}
		if (!command_string.empty()) // so it doesn't add a blank string
		{
		    single_command_list.push_back(command_string);
 		}
	    }    
	 
	void check_command(string &command, string &type)
	{
		for (int i = 0; i < command.size(); i++)
		{
			if (command.at(i) == '-' && command.at(i+1) == 'e')
			{
			    command.erase(0,3);
			    type = "-e";
			    break;
			}
			else if (command.at(i) == '-' && command.at(i+1) == 'f')
			{
			    command.erase(0,3);
			    type = "-f";
			    break;
			}
			else if (command.at(i) == '-' && command.at(i+1) == 'd')
			{
			    command.erase(0,3); // just remove the flag
			    type = "-d";
			    break;
			}
			else if (command.at(i) == 't' && command.at(i+1) == 'e' && command.at(i+2) == 's' && command.at(i+3) == 't') 
			{
			    command.erase(0,i+5);
			    type = "test";
			    break;
			}
			else if (command.at(i) == '[') 
			{
			    command.erase(0,1);
			    command.erase(command.size() - 1, 1);
			    this->clean(command);

			    type = "test";
			    break;
			}

			else if (command.at(i) == '(')
			{
			    type = "paren";
			break;
			}
			else
			{
			type = "-e";
			break;
			}
		}	

	}

   
	    // Overload for when passing a vector that will hold command gorups
	    void parse(std::string command_string, std::vector<std::string> &command_g) {
		
		int counter = 0;

		while (counter < command_string.size())
		{
		    this->parse_string_commands(command_string, counter, command_g);
		}
		command_g.push_back(command_string);
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
		return connectors.front();
	    }
	    
	    int get_back() {
		return connectors.back();
	    }
	    
	    int get_size() {
	    	return connectors.size();
	    }
	    
	    void pop() {
	    	this->connectors.pop();
	    }

	    std::vector<std::string> get_individual_commands() {
		return this->single_command_list;
	    }
};

#endif // _PARSER_HPP_

