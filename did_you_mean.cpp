/**
 * This file contains implementations of the functions that provide
 * did you mean functionality for the shell.
 */

#include "did_you_mean.h"
#include "common.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void dym::all_commands( vector<string>& commands )
{
    string path = getenv( "PATH" );
    string dir;

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
}

void dym::near_matches( vector<string>& commands, string line, vector<string>& near_match )
{
  char spurious;
  string temp;
  size_t l_size=line.size( );
  size_t c_size=commands.size( );
  for( size_t i=0; i< c_size; i++ )
  {
    for( int j=32; j<127; j++ )
    {
      spurious=j;
      for( size_t k=0; k < l_size+1; k++ )
      {
        temp=line;
        if( commands[i].compare( line.insert( k, 1, spurious ) ) == 0 )
        {
          near_match.push_back( commands[i] );
        }
        line=temp;
      }
    }
  }
 
  for( size_t i=0; i< c_size; i++ )
  {
    for( int j=32; j<127; j++ )
    {
      spurious=j;
      for( size_t k=0; k < l_size; k++ )
      {
        temp=line;
        line[k]=j;
        if( commands[i].compare( line ) == 0 )
        {
          near_match.push_back( commands[i] );
        }
        line=temp;
      }
    }
  }

  for( size_t i=0; i< c_size; i++ )
  {
    for( size_t j=0; j < l_size; j++ )
    {
      temp=line;
      if( commands[i].compare( line.erase(j,1) ) == 0 )
      {
        near_match.push_back( commands[i] );
      }
      line=temp;
    }
  }
}

void dym::display_results( vector<string>& near_matches )
{
  if( near_matches.size( ) > 0 )
  {
    cout << endl << "Did you mean:" << endl;
    for( size_t i=0; i<near_matches.size( ); i++ )
    {
      cout << near_matches[i] << endl;
    }
    cout << endl;
  }
}

void dym::do_you_mean( string& line )
{
  vector<string> commands;
  vector<string> near_match;
  all_commands( commands );
  near_matches( commands, line, near_match );
  display_results( near_match );
}
