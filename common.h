/**
 * This file contains shared variables and methods that are used in multiple
 * files.
 *
 * Feel free to add any common code that you find yourself unsure where to put
 * in this file or in the corresponding implementation file.
 */

#pragma once
#include <string>
#include <vector>
#include <map>


// Define 'builtin_t' as a type for built-in functions.
typedef int (*builtin_t)(std::vector<std::string>&);

/**
 * An external reference to the execution environment. For more info, run
 * 'man environ' in a terminal.
 */
extern char** environ;

/**
 * A mapping of internal commands to their corresponding functions.
 */
extern std::map<std::string, builtin_t> builtins;

/**
 * A mapping of variables (local to the shell) and their corresponding values.
 */
extern std::map<std::string, std::string> localvars;

/**
 * A mapping of aliases and their corresponding values.
 */
extern std::map<std::string, std::string> aliases;

/**
 * A mapping of natural numbers to commands in command history.
 */
extern std::map<int, std::string> commands;

/**
 * Returns the current working directory.
 */
std::string get_current_working_directory();


/**
 * Lists the contents of the given directory.
 *
 * @param directory the full path of the directory to list
 * @param results a vector to which the directory contents will be added
 * @return zero on success, or error code on failure
 */
int list_directory_contents(
    std::string& directory,
    std::vector<std::string>& results);


/**
 * Converts a vector of strings into an array of dynamically allocated
 * c-strings.
 */
char ** strvec_to_cstrarray( std::vector<std::string>& tokens );
