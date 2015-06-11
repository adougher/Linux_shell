Andrew Dougherty
CSCI442, Spring 2014
Project 1

auto_completion.h:
This is the header file for the TAB completion functions.

auto_completion.cpp:
This file contains the implementation of functions that populate the command
and alias lists that represent possible completions for the TAB completion
functionality.

builtins.h:
This is the header file for the available built-in functions.

builtins.cpp:
This file contains the implementations for all the builtin functions:
	ls
	cd
	pwd
	alias
	unalias
	echo
	exit
	history

cmd_execution.h:
This is the header file for the command execution function and a myriad of
its helper functions.

cmd_execution.cpp:
This file contains the external command execution function and commands for 
parsing the commmand, validating the input, handling pipes and redirections,
and memory management.

common.h:
This is the header file for three general purpose functions that are used 
in other files.

common.cpp:
This file contains the implementations for three functions. One retrives the 
current working directory.  One lists the contents of a direcory.  The last
converts a vector of strings to a char**.

shell.h:
Header file for the main driver file for the shell.

shell.cpp:
This file contains main( ) and implementations for the main shell helper functions.
These include functions for initializing the shell, assigning shell variables,
tokenizing commands, establishing the prompt, substituting aliases and variables,
and sending the command off for execution.

************************
SPECIAL FEATURES
************************
I have implemented support for infinite pipes.

I have implemented persistent history, so that commands that were input in 
previous shell sessions are retained in the ghistory list.

I have implemented a basic "did you mean:" utility for almost correctly input
commands. It checks if:

  1. Inserting any single character at any position in the input gives a valid command.
  2. Deleting any single character from the input gives a valid command.
  3. Replacing any single character in the command with any other character
     gives a valid command.

It then outputs all such possibilities.  For example, input ">cath" has output:

  cath: command not found

  Did you mean:
  math
  cat


*****
Time Spent: 20 hours
