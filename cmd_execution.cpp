/**
 * This file contains implementations of the functions that are responsible for
 * executing commands that are not builtins.
 *
 * Much of the code you write will probably be in this file.
 */

#include "cmd_execution.h"
#include "common.h"
#include "did_you_mean.h"
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/param.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <sys/stat.h>
#include <algorithm>
#include <fcntl.h>

using namespace std;

bool file_exists(char* filename) 
{
    struct stat fileInfo;
    return stat(filename, &fileInfo) == 0;
}

int check_for_exec( string path, string exec, string& result )
{
  string dir;
  int check=-1;
  // tests each directory in path string for executable
  while( path.find_first_of( ":" ) != string::npos )
  {
    dir=path.substr( 0, path.find_first_of( ":" ) );
    path.erase( 0, path.find_first_of( ":" )+1 );
    // Check for the executable.  If it's there break the loop
    // and return the full path
    if( file_exists( const_cast<char*>( (dir+"/"+exec).c_str( ) ) ) )
    {
      result=dir+"/"+exec;
      check=1;
    }
  }
  if( file_exists( const_cast<char*>( (path+"/"+exec).c_str( ) ) ) )
  {
    result=dir+"/"+exec;
    check=1;
  }
  return check;
}

int setup_redirection( vector<string>& tokens, int &test, int &file )
{
  // check for redirection tokens
  if( find( tokens.begin(), tokens.end(), ">" ) != tokens.end() || find( tokens.begin(), tokens.end(), "<" ) != tokens.end() ||
      find( tokens.begin(), tokens.end(), ">>" ) != tokens.end() )
  {
    // Verify correct position of tokens
    if( tokens[ tokens.size( ) - 2 ] != ">" && tokens[ tokens.size( ) - 2 ] != "<" && tokens[ tokens.size( ) - 2 ] != ">>" )
    {
      return -1;
    }
    else
    {
      // Set test int and attempt to open file based on redirection action
      // int file is file descriptor
      // test is read or write or write append indicator
      if( tokens[ tokens.size( ) - 2 ] == ">" )
      {
        file = open( tokens[tokens.size( )-1].c_str(), O_CREAT | O_RDWR, S_IRWXU );
        if( file < 0 )
        {
          return -1;
        }
        else
        {
          test=0; 		// signifies dup2 stdout to file
          tokens.pop_back( );
          tokens.pop_back( );
          return 0;
        }
      }
      else
      {
        if( tokens[ tokens.size( ) - 2 ] == "<" )
        {
          file = open( tokens[tokens.size( )-1].c_str(), O_RDONLY );
          if( file < 0 )
          {
            return -1;
          }
          else
          {
            test=2;		// signifies dup2 stdin from file
            tokens.pop_back( );
            tokens.pop_back( );
            return 0;
          }
        }
        else
        {
          file = open( tokens[tokens.size( )-1].c_str(), O_APPEND | O_RDWR );
          if( file < 0 )
          {
            return -1;
          }
          else
          {
            test=1;		// signifies dup2 stdout append to file
            tokens.pop_back( );
            tokens.pop_back( );
            return 0;
          }
        }
      }
    }
  }
  else 
  {
    return 0;
  }
}

int piped_to_commands(vector<string>& tokens, vector< vector<string> >& commands)
{
  // test for bad pipe syntax
  if( tokens[0] == "|" || tokens[ tokens.size( )-1 ] == "|" )
  {
    return -1;
  }
  vector<string> command;
  while( tokens.size( ) > 0 )
  {
    command.push_back( tokens[0] );
    tokens.erase( tokens.begin( ) );
    if( tokens[0] == "|" )
    {
      tokens.erase( tokens.begin( ) );
      commands.push_back( command );
      command.clear( );
    }
  }
  commands.push_back( command );
  return 0;
}

int pipe_redirection_syntax( vector< vector<string> >& commands )
{
  for( size_t i=0; i < commands.size( ); i++ )
  {
    if( (find(commands[i].begin( ), commands[i].end( ), "<" ) != commands[i].end( ) && i != 0) ||
        (find(commands[i].begin( ), commands[i].end( ), ">" ) != commands[i].end( ) && i != commands.size( )-1) ||
        (find(commands[i].begin( ), commands[i].end( ), ">>" ) != commands[i].end( ) && i != commands.size( )-1) )
    {
      return -1;
    }
  }
  return 0;
}

void delete_2d( char** argv, vector<string>& tokens )
{
  for( size_t i = 0; i < tokens.size( ); i++ )
  {
    delete [ ] argv[i];
  }
  delete [] argv;
}

int execute_external_command(vector<string>& tokens) {

  vector< vector<string> > commands;
  int return_value=piped_to_commands( tokens, commands );

  // Error checks for invalid pipe and/or redirection syntax
  if( return_value == -1 )
  {
    cout << "Invalid pipe syntax." << endl;
    return -1;
  }
  if( commands.size( ) > 1 )
  {
    return_value=pipe_redirection_syntax( commands ); 
    if( return_value == -1 )
    {
      cout << "Invalid use of redirection in piped command." << endl;
      return -1;
    }
  }

  // commence with pipes and forks
  int file_descriptors[commands.size()-1][2]; // Number of pipes needed for n commands
  int pid;

  size_t i=0;
  while( i<commands.size( ) )
  {
    string path = getenv( "PATH" );
    string full_path="";

    // check for executable in PATH
    int check = check_for_exec( path, commands[i][0], full_path );
    if( check == -1 )
    {
      cout <<  commands[i][0] << ": command not found" << endl;
      dym::do_you_mean( commands[i][0] );
      return -1;
    }

    // Variables to setup dup2( ) action and file descriptor
    int test=-1;
    int file=1;

    int return_code = setup_redirection( commands[i], test, file );

    if( return_code < 0 )
    {
      if( file < 0 )
      {
        cout << "File failed to open." << endl;
        return -1;
      }
      else
      {
        cout << "File redirection error." << endl;
        return -1;
      }
    }
   
    // conversion for exec( ) command
    char ** argv=strvec_to_cstrarray( commands[i] );

    // This is true if the command is piped, and the current command 
    // is not the last in the pipeline.
    if( i < commands.size( )-1 )
    {
      // create pipe between command i and command i+1
      if (pipe(file_descriptors[i]) < 0) {
        perror("opening pipe");
        return(-1);
      }
    }
 
    // Attempt to fork off a new process.
    if ((pid = fork()) == -1) 
    {
      perror("fork failed");
      return(-1);
    }

    if (pid == 0) 
    {
      // Attempt to execute the command.
      if( test > -1 && test < 2 )
      {
        if( dup2(file,1) < 0 )
        {
          perror("File redirection failure.");
          return -1;
        }
      }
      if( test == 2 )
      {
        if( dup2(file,0) < 0 )
        {
          perror("File redirection failure.");
          return -1;
        }
      }
      if( i < commands.size( )-1 )
      {
        // assign stdout to write end of pipe for current command
        if( dup2(file_descriptors[i][1],1) < 0 )
        {
          perror("Redirection failure.");
          return -1;
        }
      }
      if( i > 0 )
      {
        // assign stdin to read end of previous pipe for command i+1
        if( dup2(file_descriptors[i-1][0],0) < 0 )
        {
          perror("Redirection failure.");
          return -1;
        }
      }
      // execute the command
      execvp(full_path.c_str( ), argv);
      perror(argv[0]);
      return -1;
    } 
    else 
    {
      // Wait for the child process to finish executing.
      if( commands.size( )>1 )
      {
        // close write end of pipe for last executed command
        close(file_descriptors[i][1]);
        delete_2d( argv, commands[i] );  // clean up for next command
      }
      if( i > 0 )
      {
        // close read end of previous pipe.  We're done with it.
        close(file_descriptors[i-1][0]);
      }
      i++;
      waitpid(pid, NULL, 0);
    }
  }
  return EXIT_SUCCESS;
}
