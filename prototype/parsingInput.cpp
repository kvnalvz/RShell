#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <sstream>
#include <queue>
#include <iomanip>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
vector<string> lis;
queue<string> connectors;
vector<string> tester;

int semiColon = 0;
int andAND = 0;
int orOR = 0;
int hashTag = 0;

void parsingS(string &enter, int &counter, vector<string> &list, queue<string> &connectors)
{
 

     if (enter.at(counter) == ';')
     {
                   
                lis.push_back(enter.substr(0, counter));
                
                enter.erase(0, counter + 2);
                counter = 0;
                semiColon++;
           
                   connectors.push(";");
                   
     }
    
       else if (enter.at(counter) == '&' && enter.at(counter +1) == '&')
       {
                    
                 lis.push_back(enter.substr(0, counter -1));
                    
                 enter.erase(0, counter + 3);
                 counter = 0;
                 andAND++;
                   
                    connectors.push("&&");
           
       }
    
      else if (enter.at(counter) == '|' && enter.at(counter +1) == '|')
      {
                    
                 lis.push_back(enter.substr(0, counter -1));
                    
                 enter.erase(0, counter + 3);
                 counter = 0;
                 orOR++;
                    
                    connectors.push("||");
          
      }
    
       else if (enter.at(counter) == '#')
       {
                    if (counter != 0)
                    {
                    lis.push_back(enter.substr(0, counter));
                        enter.erase(0, counter );
                        counter = 0;
                    }
                    else
                    {
                    lis.push_back(enter);
                    enter = "";
                    }
                    connectors.push("#");
           
       }

	else if (enter.at(counter) == '(')
	{

	int frontP = 1;
	int backP = 0;
	int lastPRightSide = 0;
	int lastPLeftSide = 0;

	for (int i = 1; i < enter.size(); i++)
	{
		if (enter.at(i) == '(')
		{
		frontP++;
		lastPLeftSide = i;
		}
		else if (enter.at(i) == ')')
		{
		backP++;
		lastPRightSide = i;
		}
	}

	if (frontP == backP)
	{
	
	if (frontP > 2 && backP > 2)
	{
	enter = enter.substr(counter+1,lastPRightSide-1);
	int counter2 = 0;
	
	while(counter2 < enter.size())
	{
	parsingS(enter, counter2, lis, connectors);
	}
	lis.push_back(enter);

	}

	

	else
	{

	for (int i = counter; i < enter.size(); i++)
	{
	if (enter.at(i) == ')')
	{
	lis.push_back(enter.substr(counter+1, i -1));
                    
                    if (i == enter.size() -1)
                    {
                    
                    enter.erase(0, i+1);
                    }
                    else if (enter.at(i+2) == '&' && enter.at(i+3) == '&')
                    {
                        connectors.push("&&");
                        andAND++;
                        enter.erase(0, i+5);
                    }
                    else if (enter.at(i+2) == '|' && enter.at(i+3) == '|')
                    {
                        connectors.push("||");
                        orOR++;
                        enter.erase(0, i+5);
                    }
                    else if (enter.at(i+2) == ';')
                    {
                        connectors.push(";");
                        semiColon++;
                        enter.erase(0,i+4);
                    }

	break;
	}
	}
	}

	else
	{
	cout << "ERROR: parenthesis don't match" << endl;
	exit(1);
	}

	counter = 0;
	}

	else if (enter.at(counter) == '[' && enter.at(counter+1) == ' ')
	{
	for (int i = counter; i < enter.size(); i++)
	{
	if (enter.at(i) == ']' && enter.at(i-1) == ' ')
	{
	//string temp = enter.substr(counter+2, i-3);
	//int counter2 = 0;
	
	/*
	while(counter2 < temp.size())
	{
	parsingS(temp, counter2, lis, connectors);
	}
	*/
 
	         if(i == enter.size()-1)
                {
                    tester.push_back(enter.substr(2,i-3));
                    
                }
                else if (enter.at(i +2) == '&' && enter.at(i+3) == '&')
                {
                    
                    tester.push_back(enter.substr(2,i-3));
                    
                    
                    connectors.push("&&");
                    andAND++;
                    
                }
                
                else if (enter.at(i+2) == '|' && enter.at(i+3) == '|')
                {
                    
                    tester.push_back(enter.substr(2,i-3));
                   
                    
                    connectors.push("||");
                    orOR++;
                    
                }
                
                
                else if (enter.at(i+2) == ';')
                {
                    
                    tester.push_back(enter.substr(2,i-2));
                    
                    
                    connectors.push(";");
                    semiColon++;
                }	

   		if (i == enter.size()-1)
                {
                    enter.erase(0,enter.size()+1);
                }
                else if(enter.at(i+2) == ';')
                {
                    enter.erase(0, i+4);
                }
                else{
                enter.erase(0, i +5);
                }
                
                break;
		}
	}
	counter = 0;
	}


    else if ((enter.at(counter) == 't') && (enter.at(counter+1) == 'e') && (enter.at(counter+2) == 's')
             && (enter.at(counter+3) == 't') && (enter.at(counter+5) == '-'))
    {
        for (int i = 0; i < enter.size(); i++)
        {
            if (i == enter.size() -1)
            {
                tester.push_back(enter.substr(0,i+1));
                enter.erase();
                break;
            }
            else if (enter.at(i) == '&' && enter.at(i+1) == '&')
            {
                tester.push_back(enter.substr(0,i-1));
                enter.erase(0,i+3);
                
                connectors.push("&&");
                andAND++;
                break;
            }
            else if(enter.at(i) == '|' && enter.at(i+1) == '|')
            {
                tester.push_back(enter.substr(0,i-1));
                enter.erase(0,i+3);
                
                connectors.push("||");
                orOR++;
                break;
            }
            else if (enter.at(i) == ';')
            {
                tester.push_back(enter.substr(0,i-1));
                enter.erase(0,i+2);
                
                connectors.push(";");
                semiColon++;
                break;
                
            }
     
        }
        counter = 0;
    }




       else
       {
              counter++;
              
       }
    

    
}

int main(int argv, char** argc)
{
    string enter = argc[1];
    int counter = 0;

    cout << "enter: ";
    cout << endl;
   // getline(cin,enter);



    while (counter < enter.size()) //(count < enter.size())
    {

            parsingS(enter, counter, lis, connectors);
        
    }
    lis.push_back(enter);
    
    
    cout << "OUTPUT OF STRINGS SEPARATED BY CONNECTORS: ";
    cout << endl;
    for (auto one : lis)
    {
        cout << one << endl;
        
    }
    
    cout << endl;

    cout << "CONNECTORS IN ORDER THEY APPEARED: ";
    cout << endl;   
    while (!connectors.empty())
    {
        cout << connectors.front() << endl;
        connectors.pop();
    }
    

    

    cout << endl;
    cout << "semicolom: " << semiColon << endl;
    cout << "&&: " << andAND << endl;
    cout << "||: " << orOR << endl;
    cout << "#: " << hashTag << endl;
    
    return 0;
}


