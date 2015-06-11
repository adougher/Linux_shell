/**
 * This file contains the implementations of methods that are common to multiple
 * files and that have no better home. It also includes the actual instantiation
 * of shared variables.
 *
 * Feel free to shared methods that have no better home to this file.
 */

#include "common.h"
#include <unistd.h>
#include <sys/param.h>
#include <dirent.h>
#include <iostream>
#include <string.h>

using namespace std;


// Instantiations of the shared variables declared in common.h
map<string, builtin_t> builtins;
map<string, string> localvars;
map<string, string> aliases;
map<int, string> commands;


string get_current_working_directory() {
  char buffer[ MAXPATHLEN ];
  if( getcwd( buffer, MAXPATHLEN ) == NULL )
  {
    cout << "getcwd error...";
  }

  return string(buffer);
}

int list_directory_contents(string& directory, vector<string>& results) {
  // Attempt to open the specified directory and check for errors.
  DIR* dir = opendir(directory.c_str());

  if (!dir) {
    return 1;
  }

  // Add each entry in the directory to the given results vector
  for (dirent* current = readdir(dir); current; current = readdir(dir)) {
    results.push_back(current->d_name);
  }

  // Close the directory and free all corresponding resources.
  if (closedir(dir) == -1) {
    return 2;
  }

  return 0;
}

char** strvec_to_cstrarray( vector<string>& tokens )
{
  char** argv = new char*[ tokens.size( ) + 1 ];
  for( size_t i = 0; i < tokens.size( ); i++ )
  {
    argv[i]=new char[ tokens[i].size( ) + 1 ];
    strcpy( argv[i], tokens[i].c_str( ) );
  }
  argv[tokens.size( )]=NULL;
  return argv;
}
