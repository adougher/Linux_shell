/**
 * This file contains implementations of the functions that provide
 * tab-completion for the shell.
 *
 * You will need to finish the implementations of the completion functions.
 */

#include "auto_completion.h"
#include "common.h"
#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;


char* environment_completion_generator(const char* text, int state) {
  // A list of all the matches;
  // Must be static because this function is called repeatedly
  static vector<string> matches;

  // If this is the first time called, construct the matches list with
  // all possible matches
  if (state == 0) {
    // List of all possibilities
    vector<string> vars;

    char ** env = environ;
    // Parse and retrieve variable names from environ
    while( *env != NULL )
    {
      string env_var( *env );
      env_var=env_var.substr( 0, env_var.find_first_of( "=" ));
      vars.push_back( "$" + env_var );
      env++;
    }
    
    // Retrieve variable names from localvars map
    for( map<string, string>::iterator it = localvars.begin( ); it != localvars.end( ); it++ )
    {
      vars.push_back( "$" + it->first );
    }

    // Iterate through all possibilities and check if input is a suffix of any
    for( size_t i=0; i<vars.size( ); i++ )
    {
      if( vars[i].find( text ) == 0 )
      {
        matches.push_back( vars[i] );
      }
    }
  }

  // Return a single match (one for each time the function is called)
  return pop_match(matches);
}


char* command_completion_generator(const char* text, int state) {
  // A list of all the matches;
  // Must be static because this function is called repeatedly
  static vector<string> matches;

  // If this is the first time called, construct the matches list with
  // all possible matches
  if (state == 0) {
    // Here we will retrieve all possibilities from $PATH environment variable
    string path = getenv( "PATH" );
    string dir;
    vector<string> commands;

    // Parse and retrieve all path directories using ':' delimiter
    while( path.find_first_of( ":" ) != string::npos )
    {
      dir=path.substr( 0, path.find_first_of( ":" ) );
      path.erase( 0, path.find_first_of( ":" )+1 );
      // populate commands with available executables
      int return_code=list_directory_contents( dir, commands );
      return_code++;
    }
    // final iteration
    int return_code=list_directory_contents( dir, commands );
    return_code++;

    // Retrieve all builtin commands from builtins map
    for( map<string, builtin_t>::iterator it = builtins.begin( ); it != builtins.end( ); it++ )
    {
      commands.push_back( it->first );
    }

    // Retrieve all aliases for commands from alias map
    for( map<string, string>::iterator it = aliases.begin( ); it != aliases.end( ); it++ )
    {
      commands.push_back( it->first );
    }

    // Iterate through all possibilities and check if input is a suffix of any
    for( size_t i=0; i<commands.size( ); i++ )
    {
      if( commands[i].find( text ) == 0 )
      {
        matches.push_back( commands[i] );
      }
    }
  }

  // Return a single match (one for each time the function is called)
  return pop_match(matches);
}


char** word_completion(const char* text, int start, int end) {
  char** matches = NULL;

  if (text[0] == '$') {
    matches = rl_completion_matches(text, environment_completion_generator);
  } else if (start == 0) {
    matches = rl_completion_matches(text, command_completion_generator);
  } else {
    // We get directory matches for free (thanks, readline!)
  }

  return matches;
}


char* pop_match(vector<string>& matches) {
  if (matches.size() > 0) {
    const char* match = matches.back().c_str();

    // Delete the last element
    matches.pop_back();

    // We need to return a copy, because readline deallocates when done
    char* copy = (char*) malloc(strlen(match) + 1);
    strcpy(copy, match);

    return copy;
  }

  // No more matches
  return NULL;
}
