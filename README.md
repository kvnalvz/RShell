# CS 100 Programming Project

# Project Information
2019 Fall Quarter

Bryan Orozco SID: `862142700`  
Kevin Alvarez SID: `862131623`

# Introduction  
In this assignment we will be creating our own command shell called rshell. It will take commands and connectors as the input and then use `fork`, `execvp` and `waitpid` accordingly. A command input can be a single command or a group of command.  

### Example:  
Single command:  
```shell
echo "Single Command"
```

Group of commands:  
```shell
echo "First Command" && echo "Second Command" || ls -a
```

### Design Patter  
For this assignment we will be using a Composite Design Pattern. We will have a `CommandGroup` and a `Command` class which both inherit from a `Base` class that has an `execute()` pure virtual function. `CommandGroup` will hold a vector of `Command`'s and will act as the entry point of any input. `CommandGroup` will have a reference to a `Connectors` class in which we will store a queue of connectors represented as `int` values. The idea behind this is that we will treat the entire input as a `CommandGroup` and break it down to just single commands and store any connectors in an instance of `Connectors`. Whenever a `;` is found we will tret everything after that as a new `CommandGroup`. The `Connectors` class will provide the logic that will determine if we need to run a command or not.

# Diagram
## UML
![Image of UML](images/UML-assignment-4.jpeg?raw=true)

# Classes
## `Parser` Class
We created this class so that we can manage, test and implement any queue functionality from it. The overarching idea is that we will be able to split command groups into vector of commands and parse single commands as just commands and connectors.  
Example  

_Command Groups_
```
echo multiple || ls -a ; cat some_file.txt && echo bye || echo bye2 ; echo lastone
```
Should return a vector with three commands
```
echo multiple || ls -a
```
```
cat some_file.txt && echo bye || echo bye2 
```
```
echo last
```

For each of these commands we then split the connectors and add them to a queue, and each single command get's added to a `single_command_list` vector. The queue holds a 1 for `&&` or a 0 for `||`.  
Example:
```
cat some_file.txt && echo bye || echo bye2 
```
_Will have the following queue_
```
0
1
```
_Will have the vector of strings_
```
cat some_file.txt
echo bye
echo bye2 
```

All of the other methods from this class are general queue methods, if you look at the implementation of `get_front`, we just defer the functionality to the queue itself. One thing to keep not of is that we have two `parse` functions, one that accepts just a string and another that accepts a vector of strings and modifies it by reference. The idea is that the `CommandGroup` will first check if a string has multiple commands, if so add them to a vector, then we loop through that vector and parse commands and connectors for each one.

Other functions:
`check_command`: Will return if a command is a test command, a parenthesis command or if a test is `-f`,`-d`,`-e`. If nothing matches it sets the type to `-e`.
`clean`: Takes in a string and just remove any extra spaces at the beginning or end of a command. We call this function before we execute anything.

## `RedirectionParser` Class
Becuase of how the current `Parser` class works, we didn't want to change anyting else in it. We decided to make a copy of the parser class and have it just handdle redirection logic. The class will look through a string and if it finds a `>`,`>>`, or a`|` it will separaed things into a vecor as well as create a queue that represents the corresponding redirectors.

## `Connectors` Class
This is the class that will handle if a command should run or not. It will hold a `status` integer that is either 0 or 1, a `run` boolean and a `Parser` pointer for the queue of connectors that are added with `add_connector` which will defer to the parser's `parse` function (with just one parameter). The `set_status` function will take an `int` that is either `1` for pass or `0` for fail, then compare that with the bottom of the queue. Essentially we are comparing the previous result with the following connector. If the next command should run (depending on the connector) we set  `run` to `true` else we set it to `false`. It is this `run` variable that is checked (via `get_run`) before a command is executed. The `Connector` class acts as the visitor pattern in which a reference to it is always passed down to every class in our class group.

## Class Group  

### `Base` Class

The base is just a interface that has a pure virtual function called `execute`. 

### `Command` Class
This class will be where the shell command actually gets executed. The execute function will check `get_run()` from the `Connector` reference and decide if this command should run or not. If it runs it will call `set_status()` with the value of `1` if it passed and `0` if it failed. If it doesn't have to run it will just run `set_status()` with the value returned from `get_status()` we do this because `set_status()` actually pops connectors from the queue whenever it's called, and we don't want to alter the order of connectors.

### `CommandGroup` 

This class will have two private members. A vector of `Base` pointers and a reference of a `Connector` class. The connector member will be assigned in the constructor. There will be two functions, `execute` and `add_command`. When adding commands, we first check if there are multiple groups of commands (any `;`). We do this by passing a vector string to the Parser's `parse` function, if there are multiple command groups, the parser will add them to the vector. We then check if the vector size is > 1, if so we loop through them and add them as a new `CommandGroup` and create a new `Connector` class for it, then add them to the vector of Base pointers. If the vector is <= 1 then we just parse the command group without the vector parameter, get the list of each individual command (see Parser class definition for reference) and add it as a `Command` to the vector of Base pointers. The `Command` will reference the parent's `Connector` class. The execute function in this class will just loop through the Base pointers and call execute on each of them.

### `ParenthesisCommand`
This class is very similar to the `CommandGroup` then only major difference is that we take in a reference to the parent's connectors. When we execute the commands inside the parenthesis we return to the parent the status of the last command that was ran. 

### `TestCommand`
This class in in charge for checking if a file or directory exists. Using the `stat` function and with the `check_command` function in the parser, we test if a file or directory exists. We use the flags, `-f`, `-e`, `-d` and operate accordingly. If no flag is set, `-e` is taken as default.


# Prototypes/Research

For our RSHELL we will be using three system calls, fork(), execvp(), and waitpid(). Each one of them have their own unique actions, the fork() system call creates two copies of the process being executed. The processes are the child process and the parent process. The moment when you call fork() is when it splits up, and you can run the same program twice at the same time. Depending on what commands are entered and what connectors are found, we can run two different processes at the same time. However, if we want one process to finish before we start the next one, we can use the system call wait(). Otherwise, if we don’t want to wait and just run the processes at the same time, then we should use the waitpid() system call. It takes in three arguments, which will give us the status information of the terminating process. Then depending on what the return value is, the process can run at the same time. Lastly, the execvp() system call is what is going to enable us to perform the commands that are entered in the input. For example, if the user types in “ls”, the execvp() call will display all the directories.

The parsing function that we are going to be using is one where whatever the user inputs will be stored into a string variable. Once the input is in the string variable, then we will run a while loop where it checks for any connectors in the input. Whenever it finds a connector it separates and inserts every word before the connector into a string vector, except the connector, that will be inserted into a queue. When we execute the commands it will grab the first two element from the vector and the connector from the queue. Depending on what the commands are and what the particularly connector is, the appropriate execution will be performed. Also, depending on how many commands were entered and how many connectors were found, the order in which they execute may change. Another thing that might change the execution order will be if any parenthesis or quotation marks are found.

To run parsing function, you must compile parsingInput.cpp in terminal then run it.
To run the system calls function, you must compile systemC.cpp in terminal then run it.
To test out these function you can run the shell scripts "testParsing.sh" & "systemCallTest.sh".

# Development and Testing Roadmap

## Tests
### Parser Test
For the parser we had 7 different test
 - `ParseConnectors`: Here when given a string like `echo hi || ls -a && cat main.cpp || echo bye && echo bye` we tested that the size of the connectors queue was 4 and that the front and back connectors are as expected (represented by 1 or 0). In this test we also ensured that when returning the vector of string commands, they are all in there. For example at the position 4 of the vector we should have `echo bye again`. 

 - `ParseSingleCommand`: Here we just test that no connectors are added but ensuring that the queue of connectors is indeed 0.

 - `ParseCommandGroups`: In this test we split a string like `echo hi ; ls -a || echo hola ; cat main.cpp && echo hello world` into 3 items and then add them to a local vector variable (those vector items on execution will then be parsed once again). Since here we have 3 command groups, we just ensure that the size of the vector is 3.

  - `ParenthesisTest`: In this test we make sure that we always get the outermost parenthesis command. Example:
  ```
  (echo hola && ls -a || (cat somefile.txt && echo here))  && echo bye && ( echo hi || ls -a )
  ```
  Will return a vector with the following items
   ```
  (echo hola && ls -a || (cat somefile.txt && echo here))
  echo bye
  ( echo hi || ls -a )
  ```
  - `TestParser`: Returns the test commands with or without the brakets.
  ```
  [ -e test/file/path ] && test -f ./somefile || [ -d ./somefile ] || echo bye
  ```
  Will return a vector with the following items
   ```
  [ -e test/file/path ]
  test -f ./somefile
  [ -d ./somefile ]
  echo bye
  ```
  - `CommandCheck`: We use this test to differentiate what kind of command each one is as well as what kind of file test we would do (`-f`,`-d`, or `-e`).

  - `CleanString`: This test just checks the Parser's `clean` function and makes sure there's no extra spaces before or after a command.

### Connectors Test
Since most of the queue logic is inside the parser class. For the connectors test we just run 1 test that simulates what a series of commands separated by connectors.

- `CheckCommandsRun`: If we give the command `echo hi && ls -a || cat main.cpp || echo bye && echo bye again`. The first two commands should run (`echo hi` and `ls -a`) then the following two should not run, then the last `echo bye again` should indeed run. In our test you need to keep in mind that the first command will always run, this is why we start our check with the second one.

### CommandGroup Test
For the command groups we just make sure that commands are added properly to the vector of Base pointers. This is why I've added a size function that returns the seize of the vector.

- `SingleCommandGroup`: This will just test that a command like `echo hi || ls -a && cat main.cpp || echo bye && echo byeee` will return a size of 5 ( 5 `Commands`);

- `MultipleCommandsGroup`: This will just test that a command group like `echo hi || ls -a && cat main.cpp ; echo bye && echo bye again` will return a size of 2 ( 2 `CommandGroup`);

### Command Test
Since the single command works because of the integration of all other pice, we test this out inside integrations_test. We tested for single commands, exit command, commented commands and multiple commands. Inside `multiple_commands_tests.sh` I first show what a regular bash would output, then I show what our rshell script outputs. I also save the output from the regular bash to a file.

### TestCommand Test

`TestCommand`: I just have one test that pretty much just checks if the program is working properly, I added a getter function inside the test command class that just returns true or false if a file or directory exists (depending on the flag used). This is why when we run ./test there's a (True) outputted.

### ParenthesisCommand Test

`ParenthesisCommand`: Since the parenthesis command really just acts as a grouping of regular commands, I just make sure that the commands are added properly to the class's vector member.

### RedirectionParser
`RedirectionParserTest`: In this test, we ensure that if a string containes `>`,`>>`, `<` or `|` we ensure that they are added to a vector which we will then use to grab the different components.
Example: a string like `tr a-z A-Z < file.txt` with the parser will give us `tr a-z A-Z` at possition 0 and `file.txt` at position 1

