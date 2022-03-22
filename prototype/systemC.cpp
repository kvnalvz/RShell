#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <unistd.h>
using namespace std;


int main(int argv, char** argc)
{
  string tester = "ls -a";
  vector<string> s;
  int counter = 1;
  int status;
  

  char* args[50];

  for (int i = 0; i < tester.size(); i++)
  {
      if (tester.at(i) == ' ')
      {
          s.push_back(tester.substr(0,i));

          tester.erase(0,i +1);
          i = 0;
          counter++;
      }
      else if (i == (tester.size() -1))
      {
          s.push_back(tester);
          break;
      }

  }

  for (int i = 0; i < counter; i++)
  {
      args[i] = (char*)(s.at(i).c_str());
  }
  args[counter] = NULL;


  pid_t child;
  pid_t check;
  child = fork();


// CHILD PROCESS


  if (child == 0)
  {
 
cout << "CHILD PROCESS: " << child << endl;

 if ( execvp (args[0], args) == -1)
          {
              perror ("exec");
          }
}

// PARENT PROCESS
 if (child > 0)
  {

   check =  waitpid(child, &status, WNOHANG);

if (check == 0)
{

   tester = "cal";
    s.clear();
    counter = 1;
    
    for (int i = 0; i < tester.size(); i++)
    {
        if (tester.at(i) == ' ')
        {
            s.push_back(tester.substr(0,i));

            tester.erase(0,i +1);
            i = 0;
            counter++;
        }
        else if (i == (tester.size() -1))
        {
            s.push_back(tester);
            break;
        }
}
    cout << endl << endl;

    for (int i = 0; i < counter; i++)
    {
        args[i] = (char*)(s.at(i).c_str());
    }
    args[counter] = NULL;

cout << "PARENT PROCESS: " << child << endl;

    if ( execvp (args[0], args) == -1)
    {
        perror ("exec");
    }
}

  }


  return 0;

}
