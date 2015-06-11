/**
 * This file contains the implementations of the builtin functions provided by
 * the shell.
 *
 * You will need to finish the implementations of all functions but the first
 * (com_ls), whose implementation is provided for you.
 */

#include "builtins.h"
#include "common.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <readline/history.h>
#include <pwd.h>

using namespace std;


int com_ls(vector<string>& argv) {
  // If no directory is given, use the current working directory.
  if (argv.size() < 2) {
    argv.push_back(".");
  }

  vector<string> entries;

  // Add the contents of the given directory to the vector.
  // HINT: This same function will likely be useful to you when doing command
  // completion. It is impelmented in common.cpp so you can easily reuse it.
  int return_code = list_directory_contents(argv[1], entries);

  if (return_code != 0) {
    perror("ls error");
    return return_code;
  }

  for (size_t i = 0; i < entries.size(); i++) {
    cout << entries[i] << endl;
  }

  return 0;
}


int com_cd(vector<string>& argv) {
  string dir;
  // cd to home directory
  if( argv.size() == 1 )
  {
    // retrieve this user's home directory
    struct passwd *pw = getpwuid(getuid());
    dir = pw->pw_dir;
  }
  else
  {
    // cd to first argument if possible
    if( argv.size( ) > 1 )
    {
      dir = argv[1];
    }
  }
  int return_code = chdir( dir.c_str() );


  // report failure if chdir error occurred
  if (return_code != 0) {
    perror("cd error");
    return return_code;
  }
  return 0;
}


int com_pwd(vector<string>& argv) {
  // HINT: you should implement the actual fetching of the current directory in
  // get_current_working_directory(), since this information is also used for
  // your prompt
  string current_directory = get_current_working_directory();
  
  cout << current_directory << endl;  

  return 0;
}


int com_alias(vector<string>& argv) {
  
  // Output the data from the alias map
  if( argv.size() == 1 )
  {
    for (map<string, string>::iterator itr_a = aliases.begin( ); itr_a != aliases.end( ); itr_a++ ) {
      cout << "alias " << itr_a->first << "='" << itr_a->second << "'\n";
    }
    return 0;
  }
 
  // Attempts to parse argv[1]
  string alias="";
  string command="";
  if( argv.size() > 1 )
  {
    // argv[1] needs to contain at least three characters: "*=*"
    if( argv[1].size() > 2 && argv[1].find_first_of("=") != string::npos )
    {
      // assign the command and alias
      command=argv[1].substr( argv[1].find_first_of( "=" )+1, argv[1].string::npos-1 );
      alias=argv[1].substr( 0, argv[1].find_first_of( "=" ) );
    }
  }

  // if argv[1] had insufficient characters, no "=" or "=" in its first or last
  // posistion, this condition will be true, alias will fail 
  if ( alias.compare( "" ) == 0 || command.compare( "" ) == 0 ) {
    cout << "Invalid alias syntax" << endl;
    return -1;
  }

  aliases[alias]=command;
  return 0;
}


int com_unalias(vector<string>& argv) {
  
  // This condition is the only supported case
  if( argv.size() > 1 )
  {
    // clear all aliases from the map
    if( argv[1].compare( "-a" ) == 0 )
    {
      aliases.erase( aliases.begin( ), aliases.end( ) );
    }
    else
    {
      // handle case of name not found
      if( aliases.find( argv[1] ) == aliases.end( ) )
      {
        cout << "Unalias Error. " << argv[1] << " not found." << endl;
        return -1;
      }
      // Success!
      else
      {
        aliases.erase( aliases.find( argv[1] ) );
      }
    }
    return 0;
  }
  else
  {
    cout << "Unalias Error." << endl;
    return -1;
  }
}


int com_echo(vector<string>& argv) {

  // Print a blank line for single argument 
  if( argv.size( ) == 1 )
  {
    cout << endl;
  }
  else
  {
    for( unsigned int i=1; i < argv.size( ); i++ )
    {
      cout << argv[ i ] << " ";
    }
    cout << endl;
  }
  return 0;
}


int com_exit(vector<string>& argv) {
  exit( EXIT_SUCCESS );
  return 0;
}


int com_history(vector<string>& argv) {

  // Print the entire history map in order 
  if( argv.size() == 1 )
  {
    for (map<int, string>::iterator itr_c = commands.begin( ); itr_c != commands.end( ); itr_c++ ) {
      cout << itr_c->first << " " << itr_c->second << endl;
    }
    return 0;
  }
  return -1;
}
