/**
 * This file declares utility functions for reading, parsing, and processing
 * user input.
 *
 * You should not have to modify this file to satisfy the project requirements.
 */

#pragma once
#include <string>
#include <vector>


/**
 * Initializes the starting state of the shell, populating the builtins command
 * map and setting various Readline variables.
 */
void init_shell();


/**
 * Returns a string representing the prompt to display to the user. It needs to
 * include the current working directory and should also use the return value to
 * indicate the result (success or failure) of the last command.
 *
 * @param return_value the return value from the previously executed command
 * @return the string to display as the shell's prompt
 */
std::string get_prompt(int return_value);


/**
 * Transforms a C-style string into a C++ vector of string tokens, delimited by
 * whitespace.
 *
 * @param line a single line of text entered by the user
 * @return the whitespace-delimited tokens from the given line
 */
std::vector<std::string> tokenize(const char* line);


/**
 * Examines each token and sets an env variable for any that are in the form
 * of key=value. Stops at the first token not in the form of key=value.
 *
 * @param tokens the tokens corresponding to the most recently entered command
 */
void local_variable_assignment(std::vector<std::string>& tokens);


/**
 * Replaces any command token that matches an alias with that alias' value.
 *
 * @param tokens the tokens corresponding to the most recently entered command
 */
void alias_substitution(std::vector<std::string>& tokens);


/**
 * Substitutes any tokens that start with a '$' with their appropriate value, or
 * with an empty string if no match is found.
 *
 * @param tokens the tokens corresponding to the most recently entered command
 */
void variable_substitution(std::vector<std::string>& tokens);


/**
 * Executes a line of input by either calling execute_external_command or
 * directly invoking the built-in command.
 *
 * @param tokens the tokens corresponding to the most recently entered command
 */
int execute_line(std::vector<std::string>& tokens);
