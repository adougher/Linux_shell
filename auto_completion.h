/**
 * This file contains definitions for the functions that provide tab-completion
 * for the shell.
 *
 * You should not need to modify this file to satisfy the project requirements,
 * though you may of course add helper functions if you want.
 */

#pragma once
#include <vector>
#include <string>


/**
 * Generates environment variables for readline completion. This function will
 * be called multiple times by readline and will return a single cstring each
 * time.
 *
 * @param text the text entered by the user
 * @param state zero the first time this function is called; otherwise, non-zero
 * @return a single matching environment variable
 */
 char* environment_completion_generator(const char* text, int state);


/**
 * Generates commands for readline completion. This function will be called
 * multiple times by readline and will return a single cstring each time.
 *
 * @param text the text entered by the user
 * @param state zero the first time this function is called; otherwise, non-zero
 * @return a single matching command
 */
char* command_completion_generator(const char* text, int state);


/**
 * This is the function we registered as rl_attempted_completion_function. It
 * attempts to complete with a command, variable name, or filename.
 *
 * @param text the string for which to suggest completion values
 * @param start the start index of text within rl_line_buffer
 * @param end the end index of text within rl_line_buffer
 * @return an array of matching strings
 */
char** word_completion(const char* text, int start, int end);


/**
 * Pops the last value off the given vector and returns the result.
 *
 * @param matches the list of matches
 * @return the last value in matches, or NULL if empty
 */
char* pop_match(std::vector<std::string>& matches);
