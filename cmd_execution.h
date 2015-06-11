/**
 * This file contains definitions for the functions that are responsible for
 * executing commands that are not builtins. This involves spawning new
 * processes and having them execute external programs.
 *
 * You should add function definitions for any helper functions you make to
 * execute, pipe, or redirect commands (though you are not required to use this
 * file).
 */

#pragma once
#include <vector>
#include <string>

/**
 * Converts piped command to vector of commands
 */
int piped_to_commands(std::vector<std::string>& tokens, std::vector< std::vector<std::string> >& commands);

/**
 * Helper function to enforce proper file redirection syntax in piped commands
 */
int pipe_redirection_syntax( std::vector< std::vector<std::string> >& commands);

/**
 * Helper function to clean up dynamically allocated char** when dealing
 * with piped commands.
 */
void delete_2d( char** argv, std::vector<std::string>& tokens );

/**
 * Executes an external command (rather than a builtin). You will need to fork()
 * off a new process and use one of the exec() functions to execute the external
 * program. If the command contains pipes, you will have to fork() and exec()
 * multiple times. Finally, you must handle pipes and redirects as by
 * redirecting the I/O as required.
 *
 * @param tokens the tokens representing the command to execute
 */
int execute_external_command(std::vector<std::string>& tokens);

/**
 * Verifies that the file, given by its absolute path, exists
 */ 
bool file_exists( char* filename );

/**
 * Processes the $PATH string, assuming ':' delimiters, and verifies
 * whether any such directory contain the executable given by 'exec'.
 * If so, the full path for the executable is placed in result, 
 * and '0' is returned.  Otherwise '-1' is returned. 
 */
int check_for_exec( std::string path, std::string exec, std::string& result );

/**
 * Checks tokens for file redirection actions. Then ensures they are the
 * next to last token in the input. Modifies the test and file ints
 * for use with dup2( ) call.
 */
int setup_redirection( std::vector<std::string>& tokens, int &test, int &file );
