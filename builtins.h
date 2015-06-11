/**
 * This file contains definitions for the builtin functions provided by the
 * shell.
 *
 * You should not have to modify this file to satisfy the project requirements.
 */

#pragma once
#include <string>
#include <vector>


/**
 * Lists all the files in the specified directory (argv[1]). If no argument is
 * given, the current working directory is used instead.
 *
 * @param argv the vector of arguments
 * @return the return code of the operation
 */
int com_ls(std::vector<std::string>& argv);


/**
 * Changes the current working directory to the specified directory (argv[1]),
 * or to the user's home directory if no argument is provided.
 *
 * @param argv the vector of arguments
 * @return the return code of the operation
 */
int com_cd(std::vector<std::string>& argv);


/**
 * Displays the current working directory.
 *
 * @param argv the vector of arguments
 * @return the return code of the operation
 */
int com_pwd(std::vector<std::string>& argv);


/**
 * If called without an argument, then any existing aliases are displayed.
 * Otherwise, the argument is assumed to be a new alias and an entry is made in
 * the alias map.
 *
 * @param argv the vector of arguments
 * @return the return code of the operation
 */
int com_alias(std::vector<std::string>& argv);


/**
 * Removes aliases. If "-a" is provided as an argument, then all existing
 * aliases are removed. Otherwise, the argument is assumed to be a specific
 * alias to remove and if it exists, that alias is deleted.
 *
 * @param argv the vector of arguments
 * @return the return code of the operation
 */
int com_unalias(std::vector<std::string>& argv);


/**
 * Prints all arguments to the terminal.
 *
 * @param argv the vector of arguments
 * @return the return code of the operation
 */
int com_echo(std::vector<std::string>& argv);


/**
 * Exits the program.
 *
 * @param argv the vector of arguments
 * @return the return code of the operation
 */
int com_exit(std::vector<std::string>& argv);


/**
 * Displays all previously entered commands, as well as their associated line
 * numbers in history.
 *
 * @param argv the vector of arguments
 * @return the return code of the operation
 */
int com_history(std::vector<std::string>& argv);
