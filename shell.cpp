/**
 * This file contains the main() function for the shell, as well as various
 * utility methods for reading and parsing user input and for setting variables.
 *
 * You should only have to make minor modifications to this file to satisfy the
 * project requirements.
 */

#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <readline/readline.h>
#include <readline/history.h>
#include "shell.h"
#include "auto_completion.h"
#include "builtins.h"
#include "cmd_execution.h"
#include "common.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;


/**
 * The entry point to the shell.
 */
int main() {
  init_shell();

  // The return value of the last command executed
  int return_value = 0;
  int cmd_count=commands.size( );
  string curdir=get_current_working_directory( );

  while (true) {
    // Get the prompt to show, based on the return value of the last command
    string prompt = get_prompt(return_value);

    // Read a line of input from the user
    char* line = readline(prompt.c_str());

    // String used for parsing history expansion commands
    string s_line( line );

    // If the pointer is null, then EOF has been received (ctrl-d) and the shell
    // should exit.
    if (!line) {
      break;
    }

    // If the command is non-empty, attempt to execute it
    if (line[0]) {

      // Check for history expansions and manipulate the input string to reflect the expansion
      bool valid_cmd=1;		// Failed history expansions are not added to the history list
      if( s_line.find( "!!" ) == 0  )
      {
        // Check for !! error
        if( previous_history( ) == NULL )
        {
          cout << "!!: event not found" << endl;
          valid_cmd=0;
          return_value=-1;
        }
        else
        {
          // Expand according to !! and re-allocate and set line
          s_line.erase( 0, s_line.find_first_not_of( "!!" ));
          free(line);
          HIST_ENTRY* cur_hist=current_history( );
          line = new char[ (string(cur_hist->line)).size( ) + s_line.size( ) + 1 ];
          strcpy( line, (string(cur_hist->line) + s_line).c_str( ) );
	  valid_cmd=1;
        }
      }
      else
      {
        if( s_line.find( "!1" ) == 0 || s_line.find( "!2" ) == 0 || s_line.find( "!3" ) == 0 || s_line.find( "!4" ) == 0 || s_line.find( "!5" ) == 0 || 
            s_line.find( "!6" ) == 0 || s_line.find( "!7" ) == 0 || s_line.find( "!8" ) == 0 || s_line.find( "!9" ) == 0 )
        {
          // extract number
          int n=atoi((s_line.substr( 1, s_line.find_first_not_of( "123456789" )-1 ) ).c_str( ) );
          // Check for !n error
          if( where_history( ) < n )
          {
            cout << "!!: event not found" << endl;
            valid_cmd=0;
            return_value=-1;
          }
          else
          {
            // Expand according to !n and re-allocate and set line
            s_line.erase( 0, s_line.find_first_not_of( "!1234567890" ));
            free(line);
            HIST_ENTRY* n_hist=history_get( n );
            line = new char[ (string(n_hist->line)).size( ) + s_line.size( ) + 1 ];
            strcpy( line, (string(n_hist->line) + s_line).c_str( ) );
            valid_cmd=1;
          }
        }
        if( s_line.find( "!-1" ) == 0 || s_line.find( "!-2" ) == 0 || s_line.find( "!-3" ) == 0 || s_line.find( "!-4" ) == 0 || s_line.find( "!-5" ) == 0 ||
            s_line.find( "!-6" ) == 0 || s_line.find( "!-7" ) == 0 || s_line.find( "!-8" ) == 0 || s_line.find( "!-9" ) == 0 )
        {
          // extract number
          int n=atoi((s_line.substr( 2, s_line.find_first_not_of( "123456789" )-1 ) ).c_str( ) );
          // Check for !-n error
          if( where_history( ) < n )
          {
            cout << "!!: event not found" << endl;
            valid_cmd=0;
            return_value=-1;
          }
          else
          {
            // Expand according to !n and re-allocate and set line
            s_line.erase( 0, s_line.find_first_not_of( "!-1234567890" ));
            free(line);
            HIST_ENTRY* mn_hist=history_get( where_history( ) - n + 1 );
            line = new char[ (string(mn_hist->line)).size( ) + s_line.size( ) + 1 ];
            strcpy( line, (string(mn_hist->line) + s_line).c_str( ) );
            valid_cmd=1;
          }
        }
      }         

      string hist_file_add;  // For persistent history
      if( valid_cmd == 1 )
      {
        cmd_count++;
        // Add this command to readline's history
        add_history(line);

        // Add this command to history map and file
        commands[cmd_count]=line;
        hist_file_add=(static_cast<ostringstream*>( &(ostringstream() << cmd_count) )->str())+" "+line+"\n";
        ofstream fs;
        fs.open((curdir + "/" + "history.txt").c_str( ), std::ios_base::app);
        fs << hist_file_add;
        fs.close( ); 

        // Break the raw input line into tokens
        vector<string> tokens = tokenize(line);

        // Handle local variable declarations
        local_variable_assignment(tokens);

        if( tokens.size( ) > 0 && tokens[0].compare( "unalias" ) != 0 )
        {
          // Handle the use of an alias
          alias_substitution(tokens);
        }

        // Substitute variable references
        variable_substitution(tokens);

        // Execute the line
        return_value = execute_line(tokens);
      }

      // Free the memory for the input string
      free(line);
    }
  }
  return 0;
}


void init_shell() {
  // Populate the map of available built-in functions
  builtins["ls"] = &com_ls;
  builtins["cd"] = &com_cd;
  builtins["pwd"] = &com_pwd;
  builtins["alias"] = &com_alias;
  builtins["unalias"] = &com_unalias;
  builtins["echo"] = &com_echo;
  builtins["exit"] = &com_exit;
  builtins["history"] = &com_history;

  // Tell readline that we want its help managing history
  using_history();

  // Specify the characters that readline uses to delimit words
  rl_basic_word_break_characters = (char *) " \t\n\"\\'`@><=;|&{(";

  // Tell the completer that we want to try completion first
  rl_attempted_completion_function = word_completion;

  // Read-in the persistent history and populate lists
  // for current session
  ifstream fs;
  fs.open("history.txt");
  int index; 
  string command;
  stringstream strStream;
  strStream << fs.rdbuf();
  string history = strStream.str();
  if( history.size( ) > 0 )
  {
    while( history.size( ) > 0 )
    {
      command=history.substr( 0, history.find_first_of( " " ) );
      index=atoi(command.c_str( ) );
      history.erase( 0, history.find_first_of( " " )+1);
      command=history.substr( 0, history.find_first_of( "\n" ) );
      add_history( command.c_str( ) );
      history.erase( 0, history.find_first_of( "\n" )+1);
      commands[index]=command;      
    }
  }
  fs.close();
}


string get_prompt(int return_value) {
  
  // The prompt consists of USERNAME:~/directory_past_home
  string homedir;
  string home_base="";

  // get the home directory
  struct passwd *pw = getpwuid(getuid());
  homedir = pw->pw_dir;

  // get the CWD, and erase the home portion
  string curdir = get_current_working_directory( );
  if( curdir.substr( 0, homedir.size( ) ).compare( homedir ) == 0 )
  {
    curdir.erase( 0, homedir.size( ) );
    home_base="~";
  }

  // get the username
  string uname = getenv("USER");

  // last command a SUCCESS!
  if ( return_value == 0 ) 
  {
    return uname + ":" + home_base + curdir + "$ {:-) > ";
  }
  // :(
  else
  {
    return uname + ":" + home_base + curdir + "$ {:-( >  ";
  }
}


vector<string> tokenize(const char* line) {
  vector<string> tokens;
  string token;

  // istringstream allows us to treat the string like a stream
  istringstream token_stream(line);

  while (token_stream >> token) {
    tokens.push_back(token);
  }

  // Search for quotation marks, which are explicitly disallowed
  for (size_t i = 0; i < tokens.size(); i++) {

    if (tokens[i].find_first_of("\"'`") != string::npos) {
      cerr << "\", ', and ` characters are not allowed." << endl;

      tokens.clear();
    }
  }

  return tokens;
}


void local_variable_assignment(vector<string>& tokens) {
  vector<string>::iterator token = tokens.begin();

  while (token != tokens.end()) {
    string::size_type eq_pos = token->find("=");

    // Stop at the first token not in the form: key=value
    if (eq_pos == string::npos) {
      break;
    }

    string key = token->substr(0, eq_pos);
    string value = token->substr(eq_pos + 1);
    localvars[key] = value;

    // Erase the token and advance to the next one.
    token = tokens.erase(token);
  }
}


void alias_substitution(vector<string>& tokens) {

  // We will iterate through the alias map and if a key is found amongst
  // the tokens, we will replace the token with it's non-alias value
  map<string, string>::iterator cmd;  

  for( unsigned int i=0; i < tokens.size( ); i++ ) {
    cmd = aliases.find(tokens[i]);

    if (cmd != aliases.end()) {
      tokens[i] = (cmd->second);
    }
  }
}


void variable_substitution(vector<string>& tokens) {
  vector<string>::iterator token;

  for (token = tokens.begin(); token != tokens.end(); ++token) {

    if (token->at(0) == '$') {
      string var_name = token->substr(1);

      if (getenv(var_name.c_str()) != NULL) {
        *token = getenv(var_name.c_str());
      } else if (localvars.find(var_name) != localvars.end()) {
        *token = localvars.find(var_name)->second;
      } else {
        token = tokens.erase(token);
      }
    }
  }
}


int execute_line(vector<string>& tokens) {
  int return_value = 0;

  if (tokens.size() != 0) {
    map<string, builtin_t>::iterator cmd = builtins.find(tokens[0]);

    if (cmd == builtins.end()) {
      return_value = execute_external_command(tokens);
    } else {
      return_value = ((*cmd->second)(tokens));
    }
  }

  return return_value;
}
